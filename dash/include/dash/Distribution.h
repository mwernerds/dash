#ifndef DASH__DISTRIBUTION_H_
#define DASH__DISTRIBUTION_H_

#include <dash/Enums.h>
#include <dash/internal/Logging.h>

namespace dash {

/**
 * Specifies how a Pattern distributes elements to units
 * in a specific dimension.
 *
 * Predefined configurations are 
 *   * dash::BLOCKED
 *   * dash::BLOCKCYCLIC
 *   * dash::CYCLIC
 *   * dash::TILE
 *   * dash::NONE
 *
 * \see Pattern
 */
class Distribution {
public:
  dash::internal::DistributionType type;
  size_t blocksz;

  /**
   * Constructor, initializes Distribution with distribution
   * type NONE.
   */
  Distribution()
  : type(dash::internal::DIST_NONE),
    blocksz(-1) {
  }

  /**
   * Constructor, initializes Distribution with a
   * distribution type and a block size.
   */
  Distribution(
    dash::internal::DistributionType distType,
    size_t blockSize)
  : type(distType),
    blocksz(blockSize) {
  }

  /**
   * Resolve the block coordinate for a given local index
   * in the distribution's dimension.
   */
  template< typename IndexType, typename SizeType>
  IndexType local_index_to_block_coord(
    // The unit's offset in the distribution's dimension
    // within the global team specification
    IndexType unit_teamspec_coord,
    // Local index of the element
    IndexType local_index,
    // Number of units in the distribution's dimension
    SizeType num_units_in_dim,
    // Number of blocks in the distribution's dimension
    SizeType num_blocks_in_dim,
    // Number of elements in the distribution's dimension in a single block
    SizeType blocksize) const {
    // NOTE: blocksize should be this->blocksz
    SizeType local_block_offset = 0;
    switch (type) {
      case dash::internal::DIST_NONE:
        // There is only one block in this dimension, so
        // block coordinate is 0:
        return 0;
      case dash::internal::DIST_BLOCKED:
        // Same as blockcyclic, but local block offset is
        // always 0:
        return unit_teamspec_coord;
      case dash::internal::DIST_TILE:
        // Same as blockcyclic
      case dash::internal::DIST_BLOCKCYCLIC:
        // Number of blocks local to the unit that are
        // in front of the given local index:
        local_block_offset = local_index / blocksz;
        // Number of blocks of any unit that are in front
        // of the given local index. Unit's coordinate in team
        // spec is equivalent to the number of units in front of
        // the unit.
        return (local_block_offset * num_units_in_dim) +
                  unit_teamspec_coord;
      case dash::internal::DIST_CYCLIC:
        // Like blockcyclic, but with blocksize 1:
        DASH_LOG_TRACE("Distribution.local_index_to_block_coord",
                       "unit_teamspec_coord", unit_teamspec_coord,
                       "local_index", local_index,
                       "num_units_in_dim", num_units_in_dim);
        return unit_teamspec_coord + local_index * num_units_in_dim;
      default:
        DASH_THROW(
          dash::exception::InvalidArgument,
          "Distribution type undefined in " <<
          "local_index_to_block_coord");
    }
  }

  /**
   * The maximum number of blocks local to a single unit within an
   * extent for a given total number of units.
   */
  template<typename SizeType>
  SizeType max_local_elements_in_range(
    /// Number of elements to distribute
    SizeType range,
    /// Number of units to which elements are distributed
    SizeType num_units) const {
    SizeType num_blocks;
    switch (type) {
      case dash::internal::DIST_NONE:
        return range;
      case dash::internal::DIST_BLOCKED:
        return dash::math::div_ceil(range, num_units);
      case dash::internal::DIST_CYCLIC:
        // same as block cyclic with blocksz = 1
        return dash::math::div_ceil(range, num_units);
      case dash::internal::DIST_BLOCKCYCLIC:
        // extent to blocks:
        return dash::math::div_ceil(range, num_units);
      case dash::internal::DIST_TILE:
        // same as block cyclic
        return dash::math::div_ceil(range, num_units);
      default:
        DASH_THROW(
          dash::exception::InvalidArgument,
          "Distribution type undefined in max_local_blocks_in_range");
    }
  }

  /**
   * The maximum size of a single block within an extent for
   * a given total number of units.
   */
  template< typename IndexType, typename SizeType>
  IndexType max_blocksize_in_range(
    /// Number of elements to distribute
    IndexType range,
    /// Number of units to which elements are distributed
    SizeType num_units) const {
    switch (type) {
      case dash::internal::DIST_NONE:
        return range;
      case dash::internal::DIST_BLOCKED:
        return dash::math::div_ceil(range, num_units);
      case dash::internal::DIST_CYCLIC:
        return 1;
      case dash::internal::DIST_BLOCKCYCLIC:
      case dash::internal::DIST_TILE:
        // Shrink block size in dimension if it exceeds the number
        // of elements in dimension:
        return std::min<SizeType>(range, blocksz);
      default:
        DASH_THROW(
          dash::exception::InvalidArgument,
          "Distribution type undefined in max_blocksize_in_range");
    }
  }

  /**
   * Resolve the associated unit id of the given block offset.
   */
  template< typename IndexType, typename SizeType>
  SizeType block_coord_to_unit_offset(
    IndexType block_coord,
    unsigned int dimension,
    SizeType num_units) const {
    switch (type) {
      case dash::internal::DIST_NONE:
        // Unit id is unchanged:
        return 0;
      case dash::internal::DIST_BLOCKED:
      case dash::internal::DIST_CYCLIC:
      case dash::internal::DIST_BLOCKCYCLIC:
        // Advance one unit id per block coordinate:
        return block_coord % num_units;
      case dash::internal::DIST_TILE:
        // Advance one unit id per block coordinate and
        // one unit id per dimension:
        return (block_coord % num_units) + dimension;
      default:
        DASH_THROW(
          dash::exception::InvalidArgument,
          "Distribution type undefined in block_coord_to_unit_offset");
    }
  }

  /**
   * Equality comparison operator.
   */
  bool operator==(const Distribution & other) const {
    return (this->type == other.type &&
            this->blocksz == other.blocksz);
  }
  /**
   * Inequality comparison operator.
   */
  bool operator!=(const Distribution & other) const {
    return !(*this == other);
  }
};

/**
 * Distribution specifying that elements in a Pattern's
 * dimension shall be distributed to units in even-sized
 * blocks.
 *
 * \relates Distribution
 */
extern const Distribution BLOCKED;
/**
 * Distribution specifying that elements in a Pattern's
 * dimension shall be distributed by cycling among units.
 * Semantically equivalent to BLOCKCYCLIC(1) but with slight
 * performance improvement.
 *
 * \relates Distribution
 */
extern const Distribution CYCLIC;
/**
 * Distribution specifying that elements in a Pattern's
 * dimension shall not be distributed.
 *
 * \relates Distribution
 */
extern const Distribution NONE;

/**
 * Distribution specifying that elements in a Pattern's
 * dimension shall be distributed to units in a tiled blocks of
 * the given size.
 *
 * \relates Distribution
 */
Distribution TILE(int blockSize);
/**
 * Distribution specifying that elements in a Pattern's
 * dimension shall be distributed to units in blocks of the
 * given size.
 *
 * \relates Distribution
 */
Distribution BLOCKCYCLIC(int blockSize);

} // namespace dash

#endif // DASH__DISTRIBUTION_H_
