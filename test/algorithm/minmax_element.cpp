// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <nanorange/algorithm/minmax_element.hpp>
#include <memory>
#include <numeric>
#include <random>
#include <algorithm>
#include "../catch.hpp"
#include "../test_utils.hpp"
#include "../test_iterators.hpp"

namespace stl2 = nano;

namespace {

std::mt19937 gen;

template <class Iter, class Sent = Iter>
void
test_iter(Iter first, Sent last)
{
	stl2::minmax_result<Iter> p = stl2::minmax_element(first, last);
	if (first != last) {
		for (Iter j = first; j != last; ++j) {
			CHECK(!(*j < *p.min));
			CHECK(!(*p.max < *j));
		}
	}
	else {
		CHECK(p.min == last);
		CHECK(p.max == last);
	}

	auto rng = stl2::make_subrange(first, last);
	p = stl2::minmax_element(rng);
	if (first != last) {
		for (Iter j = first; j != last; ++j) {
			CHECK(!(*j < *p.min));
			CHECK(!(*p.max < *j));
		}
	}
	else {
		CHECK(p.min == last);
		CHECK(p.max == last);
	}

	auto res = stl2::minmax_element(std::move(rng));
	if (first != last) {
		for (Iter j = first; j != last; ++j) {
			CHECK(!(*j < *res.min));
			CHECK(!(*p.max < *j));
		}
	}
	else {
		CHECK(res.min == last);
		CHECK(res.max == last);
	}
}

template <class Iter, class Sent = Iter>
void
test_iter(unsigned N)
{
	std::unique_ptr<int[]> a{new int[N]};
	std::iota(a.get(), a.get() + N, 0);
	std::shuffle(a.get(), a.get() + N, gen);
	test_iter(Iter(a.get()), Sent(a.get() + N));
}

template <class Iter, class Sent = Iter>
void
test_iter()
{
	test_iter<Iter, Sent>(0);
	test_iter<Iter, Sent>(1);
	test_iter<Iter, Sent>(2);
	test_iter<Iter, Sent>(3);
	test_iter<Iter, Sent>(10);
	test_iter<Iter, Sent>(1000);
	{
		const unsigned N = 100;
		std::unique_ptr<int[]> a{new int[N]};
		std::fill_n(a.get(), N, 5);
		std::shuffle(a.get(), a.get() + N, gen);
		stl2::minmax_result<Iter> p = stl2::minmax_element(Iter(a.get()),
													   Sent(a.get() + N));
		CHECK(base(p.min) == a.get());
		CHECK(base(p.max) == a.get() + N - 1);
	}
}

template <class Iter, class Sent = Iter>
void
test_iter_comp(Iter first, Sent last)
{
	typedef std::greater<int> Compare;
	Compare comp;
	stl2::minmax_result<Iter> p = stl2::minmax_element(first, last, comp);
	if (first != last) {
		for (Iter j = first; j != last; ++j) {
			CHECK(!comp(*j, *p.min));
			CHECK(!comp(*p.max, *j));
		}
	}
	else {
		CHECK(p.min == last);
		CHECK(p.max == last);
	}

	auto rng = stl2::make_subrange(first, last);
	p = stl2::minmax_element(rng, comp);
	if (first != last) {
		for (Iter j = first; j != last; ++j) {
			CHECK(!comp(*j, *p.min));
			CHECK(!comp(*p.max, *j));
		}
	}
	else {
		CHECK(p.min == last);
		CHECK(p.max == last);
	}

	auto res = stl2::minmax_element(std::move(rng), comp);
	if (first != last) {
		for (Iter j = first; j != last; ++j) {
			CHECK(!comp(*j, *res.get_unsafe().min));
			CHECK(!comp(*res.get_unsafe().max, *j));
		}
	}
	else {
		CHECK(res.get_unsafe().min == last);
		CHECK(res.get_unsafe().max == last);
	}
}

template <class Iter, class Sent = Iter>
void
test_iter_comp(unsigned N)
{
	std::unique_ptr<int[]> a{new int[N]};
	std::iota(a.get(), a.get() + N, 0);
	std::shuffle(a.get(), a.get() + N, gen);
	test_iter_comp(Iter(a.get()), Sent(a.get() + N));
}

template <class Iter, class Sent = Iter>
void
test_iter_comp()
{
	test_iter_comp<Iter, Sent>(0);
	test_iter_comp<Iter, Sent>(1);
	test_iter_comp<Iter, Sent>(2);
	test_iter_comp<Iter, Sent>(3);
	test_iter_comp<Iter, Sent>(10);
	test_iter_comp<Iter, Sent>(1000);
	{
		const unsigned N = 100;
		std::unique_ptr<int[]> a{new int[N]};
		std::fill_n(a.get(), N, 5);
		std::shuffle(a.get(), a.get() + N, gen);
		typedef std::greater<int> Compare;
		Compare comp;
		stl2::minmax_result<Iter> p = stl2::minmax_element(Iter(a.get()),
													   Sent(a.get() + N), comp);
		CHECK(base(p.min) == a.get());
		CHECK(base(p.max) == a.get() + N - 1);
	}
}

struct S {
	int i;
};

}

TEST_CASE("alg.minmax_element")
{
	test_iter<forward_iterator<const int*> >();
	test_iter<bidirectional_iterator<const int*> >();
	test_iter<random_access_iterator<const int*> >();
	test_iter<const int*>();
	test_iter<forward_iterator<const int*>, sentinel<const int*>>();
	test_iter<bidirectional_iterator<const int*>, sentinel<const int*>>();
	test_iter<random_access_iterator<const int*>, sentinel<const int*>>();

	test_iter<forward_iterator<const int*> >();
	test_iter<bidirectional_iterator<const int*> >();
	test_iter<random_access_iterator<const int*> >();
	test_iter<const int*>();
	test_iter<forward_iterator<const int*>, sentinel<const int*>>();
	test_iter<bidirectional_iterator<const int*>, sentinel<const int*>>();
	test_iter<random_access_iterator<const int*>, sentinel<const int*>>();

	// Works with projections?
	S const s[] = {S{1},S{2},S{3},S{4},S{-4},S{5},S{6},S{40},S{7},S{8},S{9}};
	stl2::minmax_result<S const *> ps = stl2::minmax_element(s, std::less<int>{}, &S::i);
	CHECK(ps.min->i == -4);
	CHECK(ps.max->i == 40);
}
