// small utilities

#ifndef UTILS_HH_
#define UTILS_HH_
#include <gsl/gsl_complex.h>
#include <complex>

// convert between C++ and GSL representations of complex numbers
std::complex<double> complex_from_gsl(gsl_complex c);
gsl_complex complex_to_gsl(std::complex<double> c);

#endif // UTILS_HH_
