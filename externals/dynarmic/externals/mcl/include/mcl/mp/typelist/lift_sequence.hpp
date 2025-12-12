// This file is part of the mcl project.
// Copyright (c) 2022 merryhime
// SPDX-License-Identifier: MIT

#pragma once

#include <type_traits>
#include <utility>

#include "mcl/mp/typelist/list.hpp"

namespace mcl::mp {

    namespace detail {

        template <class VL>
        struct lift_sequence_impl;  // Forward declaration

        // Original specialization (works for GCC/MSVC)
        template <class T, template <class, T...> class VLT, T... values>
        struct lift_sequence_impl<VLT<T, values...>> {
            using type = list<std::integral_constant<T, values>...>;
        };

        // Clang-specific fix: Add a more explicit specialization that Clang can match.
        // We check for __clang__ but not _MSC_VER, as clang-cl on Windows might not need this.
        #if defined(__clang__) && !defined(_MSC_VER)
        template <class T, T... Ints>
        struct lift_sequence_impl<std::integer_sequence<T, Ints...>> {
            using type = list<std::integral_constant<T, Ints>...>;
        };
        #endif

    }  // namespace detail

    /// Lifts values in value list VL to create a type list.
    template <class VL>
    using lift_sequence = typename detail::lift_sequence_impl<VL>::type;

}  // namespace mcl::mp
