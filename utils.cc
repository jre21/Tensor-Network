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

#include "utils.hh"

using std::complex;
// ########################### complex_from_gsl ######################
complex<double> complex_from_gsl(gsl_complex c)
{
  return complex<double>(GSL_REAL(c), GSL_IMAG(c));
}

// ########################### complex_to_gsl ########################
gsl_complex complex_to_gsl(complex<double> c)
{
  gsl_complex cc;
  GSL_SET_COMPLEX(&cc, c.real(), c.imag());
  return cc;
}
