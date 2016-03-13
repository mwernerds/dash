#ifndef DASH__INTERNAL__STREAM_CONVERSION_H_
#define DASH__INTERNAL__STREAM_CONVERSION_H_

#include <dash/internal/Macro.h>

#include <array>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <cstring>

namespace dash {

// To print std::array to ostream
template <typename T, std::size_t N>
std::ostream & operator<<(
  std::ostream & o,
  const std::array<T, N> & arr)
{
  std::ostringstream ss;
  auto nelem = arr.size();
  ss << "{ ";
  int i = 1;
  for (auto e : arr) {
    ss << e
       << (i++ < nelem ? "," : "");
  }
  ss << " }";
  operator<<(o, ss.str());
  return o;
}
// To print std::vector to ostream
template <typename T>
std::ostream & operator<<(
  std::ostream & o,
  const std::vector<T> & vec)
{
  std::ostringstream ss;
  auto nelem = vec.size();
  ss << "{ ";
  int i = 1;
  for (auto e : vec) {
    ss << e
       << (i++ < nelem ? "," : "");
  }
  ss << " }";
  operator<<(o, ss.str());
  return o;
}
// To print std::set to ostream
template <typename T>
std::ostream & operator<<(
  std::ostream & o,
  const std::set<T> & set)
{
  std::ostringstream ss;
  auto nelem = set.size();
  ss << "{ ";
  int i = 1;
  for (auto e : set) {
    ss << e
       << (i++ < nelem ? "," : "");
  }
  ss << " }";
  operator<<(o, ss.str());
  return o;
}
// To print std::map to ostream
template <typename T1, typename T2>
std::ostream & operator<<(
  std::ostream & o,
  const std::map<T1,T2> & map)
{
  std::ostringstream ss;
  auto nelem = map.size();
  ss << "{ ";
  int i = 1;
  for (auto kv : map) {
    ss << "(" << kv.first
       << ":" << kv.second << ")"
       << (i++ < nelem ? "," : "");
  }
  ss << " }";
  operator<<(o, ss.str());
  return o;
}


} // namespace dash

#endif // DASH__INTERNAL__STREAM_CONVERSION_H_