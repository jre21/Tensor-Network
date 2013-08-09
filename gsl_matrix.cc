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

// if error checking is disabled, propagate it to gsl code
#ifdef NO_ERROR_CHECKING
#define GSL_RANGE_CHECK_OFF
#endif

#include "gsl_matrix.hh"

using std::complex;

// ########################### constructor ###########################
GSLMatrix::GSLMatrix(size_t n1, size_t n2)
{
  _matrix = gsl_matrix_complex_alloc(n1, n2);
  gsl_matrix_complex_set_identity(_matrix);
}

// ########################### destructor ############################
GSLMatrix::~GSLMatrix()
{
  gsl_matrix_complex_free(_matrix);
}

// ########################### get ###################################
complex<double> GSLMatrix::get(size_t i, size_t j)
{
  return _complex_from_gsl(gsl_matrix_complex_get( _matrix, i, j ));
}

// ########################### set ###################################
void GSLMatrix::set(size_t i, size_t j, const complex<double>& c)
{
    gsl_matrix_complex_set( _matrix, i, j, _complex_to_gsl(c) );
}

// ########################### complex_from_gsl ######################
complex<double> GSLMatrix::_complex_from_gsl(const gsl_complex& c)
{
  return complex<double>(GSL_REAL(c), GSL_IMAG(c));
}

// ########################### complex_to_gsl ########################
gsl_complex GSLMatrix::_complex_to_gsl(const complex<double>& c)
{
  gsl_complex cc;
  GSL_SET_COMPLEX(&cc, c.real(), c.imag());
  return cc;
}
