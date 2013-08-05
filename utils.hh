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

// small utilities

#ifndef UTILS_HH_
#define UTILS_HH_
#include <gsl/gsl_complex.h>
#include <complex>

// convert between C++ and GSL representations of complex numbers
std::complex<double> complex_from_gsl(gsl_complex c);
gsl_complex complex_to_gsl(std::complex<double> c);

#endif // UTILS_HH_
