/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <magic_enum.hpp>

#include <fmt/compile.h>
#include <fmt/format.h>

namespace roq {

//! Enumeration of order types
enum class OrderType : uint8_t {
  UNDEFINED = 0,
  MARKET,
  LIMIT,
};

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderType> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::OrderType const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, magic_enum::enum_name(value));
  }
};
