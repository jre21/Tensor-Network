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
