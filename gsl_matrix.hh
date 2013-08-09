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

#include <complex>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_matrix_complex_double.h>

class Matrix
{
public:
  virtual ~Matrix() {};
  virtual std::complex<double> get(size_t i, size_t j) = 0;
  virtual void set(size_t i, size_t j, const std::complex<double>& c) = 0;
};

class GSLMatrix : public Matrix
{
public:
  GSLMatrix(size_t n1, size_t n2);
  GSLMatrix(const GSLMatrix&) = delete;
  GSLMatrix operator= (const GSLMatrix&) = delete;
  ~GSLMatrix();
  std::complex<double> get(size_t i, size_t j) override;
  void set(size_t i, size_t j, const std::complex<double>& c) override;
protected:
  // Convert between C++ and GSL representations of complex numbers.
  std::complex<double> _complex_from_gsl(const gsl_complex& c);
  gsl_complex _complex_to_gsl(const std::complex<double>& c);
private:
  gsl_matrix_complex* _matrix;
};
