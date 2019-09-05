// nanorange/detail/type_traits.hpp
//
// Copyright (c) 2018 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NANORANGE_DETAIL_TYPE_TRAITS_HPP_INCLUDED
#define NANORANGE_DETAIL_TYPE_TRAITS_HPP_INCLUDED

#include <nanorange/detail/macros.hpp>

#include <type_traits>

NANO_BEGIN_NAMESPACE

template <typename T>
struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

namespace detail {

struct error_t {
    error_t() = delete;
    error_t(error_t const&) = delete;
    error_t& operator=(const error_t&) = delete;
    ~error_t() = delete;
};

template <typename Void, template <class...> class Trait, typename... Args>
struct test_ {
    using type = error_t;
};

template <template <class...> class Trait, typename... Args>
struct test_<std::void_t<Trait<Args...>>, Trait, Args...> {
    using type = Trait<Args...>;
};

template <template <class...> class Trait, typename... Args>
using test_t = typename test_<void, Trait, Args...>::type;

// Work around GCC5 bug that won't let us specialise variable templates
template <typename Void, template <class...> class AliasT, typename... Args>
struct exists_helper : std::false_type{};

template <template <class...> class AliasT, typename... Args>
struct exists_helper<std::void_t<AliasT<Args...>>, AliasT, Args...>
    : std::true_type{};

template <template <class...> class AliasT, typename... Args>
constexpr bool exists_v = exists_helper<void, AliasT, Args...>::value;

template <typename R, typename... Args,
          typename = decltype(&R::template requires_<Args...>)>
auto test_requires(R&) -> void;

template <typename R, typename... Args>
using test_requires_t = decltype(test_requires<R, Args...>(std::declval<R&>()));

template <typename R, typename... Args>
constexpr bool requires_ = exists_v<test_requires_t, R, Args...>;

template <bool Expr>
using requires_expr = std::enable_if_t<Expr, int>;

template <std::size_t I>
struct priority_tag : priority_tag<I - 1> {
};
template <>
struct priority_tag<0> {
};

template <typename T>
struct type_identity {
    using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type;

} // namespace detail

NANO_END_NAMESPACE

#endif
