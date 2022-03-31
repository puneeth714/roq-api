/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <initializer_list>
#include <type_traits>
#include <utility>

namespace roq {

// implements a bitmap mask using an underlying enumeration
// some limitations:
// - flag (enum value) should not be 0 -- we don't currently check
// - flag (enum value) should be powers of two, or combinations -- we don't currently check

template <typename T, typename = typename std::enable_if<std::is_enum<T>::value>::type>
class Mask final {
 public:
  using value_type = typename std::underlying_type<T>::type;

  constexpr Mask() = default;

  constexpr explicit Mask(value_type mask) : value_{mask} {}

  constexpr explicit Mask(T flag) : value_{static_cast<value_type>(flag)} {}

  // cppcheck-suppress noExplicitConstructor
  constexpr Mask(std::initializer_list<T> flags) {
    for (auto &flag : flags) {
      // cppcheck-suppress useStlAlgorithm
      value_ |= static_cast<value_type>(flag);
    }
  }

  template <typename... Args>
  constexpr Mask(const Mask &other, Args &&...args) : value_(other.value_ | Mask{std::forward<Args>(args)...}.get()) {}

  constexpr bool operator==(const Mask &rhs) const { return value_ == rhs.value_; }
  constexpr bool operator!=(const Mask &rhs) const { return value_ != rhs.value_; }
  constexpr bool operator<(const Mask &rhs) const { return value_ < rhs.value_; }
  constexpr bool operator<=(const Mask &rhs) const { return value_ <= rhs.value_; }
  constexpr bool operator>(const Mask &rhs) const { return value_ > rhs.value_; }
  constexpr bool operator>=(const Mask &rhs) const { return value_ >= rhs.value_; }

  constexpr bool empty() const { return value_ == value_type{}; }

  constexpr void reset() { value_ = value_type{}; }

  constexpr value_type get() const { return value_; }

  constexpr bool has(T flag) const { return value_ & static_cast<value_type>(flag); }

  constexpr bool has_any(T flag) const { return (value_ & static_cast<value_type>(flag)) != value_type{}; }

  constexpr bool has_any(std::initializer_list<T> flags) const {
    value_type value = {};
    for (auto &flag : flags) {
      // cppcheck-suppress useStlAlgorithm
      value |= static_cast<value_type>(flag);
    }
    return value_ & value;
  }

  constexpr bool has_any(Mask rhs) const { return (value_ & rhs.value_) != value_type{}; }

  constexpr bool has_all(T flag) const {
    return (value_ & static_cast<value_type>(flag)) == static_cast<value_type>(flag);
  }

  constexpr bool has_all(std::initializer_list<T> flags) const {
    value_type value = {};
    for (auto &flag : flags) {
      // cppcheck-suppress useStlAlgorithm
      value |= static_cast<value_type>(flag);
    }
    return (value_ & value) == value;
  }

  constexpr bool has_all(Mask rhs) const { return (value_ & rhs.value_) == rhs.value_; }

  constexpr Mask &set(T flag) {
    value_ |= static_cast<value_type>(flag);
    return *this;
  }

  constexpr Mask &set(Mask rhs) {
    value_ |= rhs.value_;
    return *this;
  }

  constexpr Mask &remove(T flag) {
    value_ &= ~static_cast<value_type>(flag);
    return *this;
  }

  constexpr Mask &remove(Mask rhs) {
    value_ &= ~rhs.value_;
    return *this;
  }

  constexpr bool has_none(T flag) const { return !has_any(flag); }

  constexpr bool has_none(std::initializer_list<T> flags) const { return !has_any(flags); }

  constexpr bool has_none(Mask rhs) const { return !has_any(rhs); }

  constexpr Mask negate() const { return Mask(~value_); }

  constexpr Mask logical_and(Mask rhs) const { return Mask(value_ & rhs.value_); }

  constexpr Mask &set(std::initializer_list<T> flags) {
    for (auto &flag : flags) {
      // cppcheck-suppress useStlAlgorithm
      value_ |= static_cast<value_type>(flag);
    }
  }

  constexpr Mask operator~() const { return negate(); }

  constexpr Mask operator&(Mask rhs) const { return logical_and(rhs); }

  constexpr Mask &operator|=(T flag) {
    set(flag);
    return *this;
  }

  constexpr Mask &operator|=(Mask rhs) {
    set(rhs);
    return *this;
  }

 private:
  value_type value_ = {};
};

}  // namespace roq

template <typename T>
struct std::underlying_type<roq::Mask<T>> {
  using type = typename roq::Mask<T>::value_type;
};

template <typename T>
struct fmt::formatter<roq::Mask<T>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Mask<T> &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(context.out(), R"({:#x})"sv, value.get());
  }
};