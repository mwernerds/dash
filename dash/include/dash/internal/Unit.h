#ifndef DASH__UNIT_H_
#define DASH__UNIT_H_

/**
 * Implementation of typed unit IDs.
 */


#include <dash/dart/if/dart_types.h>

namespace dash {
namespace internal {
namespace unit {

enum unit_scope {
  local_unit,
  global_unit
};

template <unit_scope IdScope>
struct unit_id {
public:

  template<unit_scope U>
  friend std::ostream & operator<<(std::ostream& os, unit_id<U> id);

  template <unit_scope U>
  friend constexpr unit_id<U> operator+(unit_id<U> lhs, unit_id<U> rhs);

  template <unit_scope U>
  friend constexpr unit_id<U> operator-(unit_id<U> lhs, unit_id<U> rhs);

  template <unit_scope U>
  friend constexpr bool operator==(unit_id<U> lhs, unit_id<U> rhs);

  template <unit_scope U>
  friend constexpr bool operator<(unit_id<U> lhs, unit_id<U> rhs);

  template <unit_scope U>
  friend constexpr bool operator>(unit_id<U> lhs, unit_id<U> rhs);


  /** default initialization to zero */
  constexpr explicit unit_id() noexcept : id(0) {}

  /**
   * initialization using a dart_unit_t
   * Note that it is explicit to prevent implicit conversion from existing dart_unit_t variables.
   * However, it prevents the following from working:
   * dash::global_unit_t unit = 0;
   * Instead, use:
   * dash::global_unit_t unit{0};
   */
  constexpr explicit unit_id(dart_unit_t id) noexcept : id(id) {}

  /** copy-ctor: explicit conversion from another unit_id type */
  template<unit_scope S>
  constexpr explicit unit_id<IdScope>(const unit_id<S>& uid) noexcept : id(uid) {}

  /** TODO [JS]: the following are not constexpr since C++11 does not allow
   *             for multi-statement constexpr functions (C++14 does).
   */
  const unit_id<IdScope> operator=(unit_id<IdScope> uid) noexcept {
    this->id = uid.id;
    return *this;
  }
  const unit_id operator=(dart_unit_t id) noexcept {
    this->id = id;
    return *this;
  }
  template<
    typename T,
    typename std::enable_if<
               std::is_integral<T>::value >::type = 0>
  const unit_id operator+=(T id) noexcept {
    this->id += id;
    return *this;
  }
  template<
    typename T,
    typename std::enable_if<
               std::is_integral<T>::value >::type = 0>
  const unit_id operator-=(T id) noexcept {
    this->id -= id;
    return *this;
  }
  template<
    typename T,
    typename std::enable_if<
               std::is_integral<T>::value >::type = 0>
  const unit_id operator*=(T id) noexcept {
    this->id *= id;
    return *this;
  }
  template<
    typename T,
    typename std::enable_if<
               std::is_integral<T>::value >::type = 0>
  const unit_id operator/=(T id) noexcept {
    this->id /= id;
    return *this;
  }

  const unit_id operator++() noexcept {
    this->id++;
    return *this;
  }
  const unit_id operator--() noexcept {
    this->id--;
    return *this;
  }
  const unit_id operator++(int) noexcept {
    this->id++;
    return *this;
  }
  const unit_id operator--(int) noexcept {
    this->id--;
    return *this;
  }

  /** allow cast to dart_unit_t */
  constexpr operator dart_unit_t() noexcept { return id; }

  /** address-of is the address of the unit ID */
  dart_unit_t* operator&() {
    return &this->id;
  }

  /** address-of is the address of the unit ID */
  dart_unit_t const* operator&() const {
    return &this->id;
  }

private:
  dart_unit_t id;
};

template <unit_scope IdScope>
constexpr unit_id<IdScope> operator+(unit_id<IdScope> lhs, unit_id<IdScope> rhs) {
  return unit_id<IdScope>(lhs.id + rhs.id);
}

template <unit_scope IdScope>
constexpr unit_id<IdScope> operator-(unit_id<IdScope> lhs, unit_id<IdScope> rhs) {
  return unit_id<IdScope>(lhs.id - rhs.id);
}

template <unit_scope IdScope>
constexpr bool operator==(unit_id<IdScope> lhs, unit_id<IdScope> rhs) {
  return lhs.id == rhs.id;
}


template <unit_scope IdScope>
constexpr bool operator!=(unit_id<IdScope> lhs, unit_id<IdScope> rhs) {
  return !(lhs == rhs);
}

template <unit_scope IdScope>
constexpr bool operator<(unit_id<IdScope> lhs, unit_id<IdScope> rhs) {
  return lhs.id < rhs.id;
}

template <unit_scope IdScope>
constexpr bool operator>(unit_id<IdScope> lhs, unit_id<IdScope> rhs) {
  return lhs.id > rhs.id;
}

template<unit_scope IdScope>
std::ostream & operator<<(std::ostream& os, unit_id<IdScope> id)
{
  return os<<id.id;
}

} // namespace unit
} // namespace internal
} // namespace dash

#endif