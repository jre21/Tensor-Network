// Copyright 2013 Jacob Emmert-Aronson
// This file is part of Tensor Network.
//
// Tensor Network is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// Tensor Network is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tensor Network.  If not, see
// <http://www.gnu.org/licenses/>.

// small utilities useful for unit tests

#ifndef UTILS_TEST_HH_
#define UTILS_TEST_HH_

#include <complex.h>
#include <gtest/gtest.h>

void expect_complex_eq(const std::complex<double>& expected,
		       const std::complex<double>& actual,
		       const char *file, int line);

#define TN_EXPECT_COMPLEX_EQ(expected, actual) \
  expect_complex_eq(expected, actual, __FILE__, __LINE__)

#endif // UTILS_TEST_HH_
