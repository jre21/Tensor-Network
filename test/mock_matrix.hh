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
#include "../matrix.hh"

class MockMatrix : public Matrix
{
public:
  MockMatrix();
  ~MockMatrix();
  MOCK_METHOD0(die, void()); // called in destructor
  MOCK_METHOD2(get, std::complex<double>(size_t i, size_t j));
  MOCK_METHOD3(set, void(size_t i, size_t j, const std::complex<double>& c));
};
