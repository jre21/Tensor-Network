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

#pragma once

#include <gmock/gmock.h>
#include "../tensor.hh"

class MockTensor : public Tensor
{
public:
  MockTensor();
  ~MockTensor();
  MOCK_METHOD0(input_rank, size_t());
  MOCK_METHOD0(output_rank, size_t());
  MOCK_METHOD2(entry, std::complex<double>
	       (const std::vector<size_t>& in,
		const std::vector<size_t>& out));
  MOCK_METHOD2(entry, std::complex<double>
	       (std::initializer_list<size_t> in,
		std::initializer_list<size_t> out));
  MOCK_METHOD3(set_entry, void
	       (const std::vector<size_t>& in,
		const std::vector<size_t>& out,
		std::complex<double> val));
  MOCK_METHOD3(set_entry, void
	       (std::initializer_list<size_t> in,
		std::initializer_list<size_t> out,
		std::complex<double> val));
  MOCK_METHOD3(set_input, void(size_t n, Tensor *T, size_t m));
  MOCK_METHOD3(set_output, void(size_t n, Tensor *T, size_t m));
  MOCK_METHOD1(input_tensor, Tensor*(size_t n));
  MOCK_METHOD1(output_tensor, Tensor*(size_t n));
  MOCK_METHOD1(input_num, size_t(size_t n));
  MOCK_METHOD1(output_num, size_t(size_t n));
  MOCK_METHOD1(matrix, MatrixStruct(bool conjugate));
  MOCK_METHOD3(_set_input_self, void(size_t n, Tensor *T, size_t m));
  MOCK_METHOD3(_set_output_self, void(size_t n, Tensor *T, size_t m));
};
