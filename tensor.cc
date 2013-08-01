#include <cmath>
#include <gsl/gsl_complex.h>
#include "tensor.hh"
#include "utils.hh"

using std::complex;
using std::initializer_list;
using std::vector;

// ########################### constructor ###########################
Tensor::Tensor(size_t nin, size_t nout, size_t inrank, size_t outrank)
{
  // initialize variables
  _nin = nin;
  _nout = nout;
  _inrank = inrank;
  _outrank = outrank;
  _in = _out = nullptr;
  _indest = vector<size_t>(nin,0);
  _outdest = vector<size_t>(nout,0);
  _conjugate = false;

  // If any argument is 0, the matrix is empty.  We leave it null in this case.
  if(nin != 0 && nout != 0 && inrank != 0 && outrank != 0)
    {
      // calculate powers by hand to avoid cast to floating point
      int in = 1, out = 1;
      for (int i=0; i<nin; i++) in *= inrank;
      for (int i=0; i<nout; i++) out *= outrank;
      _matrix = gsl_matrix_complex_alloc(in, out);
      gsl_matrix_complex_set_identity(_matrix);
    }
  else 
    _matrix = nullptr;
}

// ########################### destructor ############################
Tensor::~Tensor()
{
  gsl_matrix_complex_free(_matrix);
  _matrix = nullptr;
}

// ########################### copy_of ###############################
Tensor *Tensor::copy_of(Tensor *T)
{
  return nullptr;
}

// ########################### conjugate_of ##########################
Tensor *Tensor::conjugate_of(Tensor *T)
{
  return nullptr;
}

// ########################### entry #################################
complex<double> Tensor::entry(const vector<size_t>& in,
			      const vector<size_t>& out)
{
  return complex<double>();
}

complex<double> Tensor::entry(initializer_list<size_t>& in,
			      initializer_list<size_t>& out)
{
  return complex<double>();
}

// ########################### set_entry #############################
void Tensor::set_entry(const vector<size_t>& in,
		       const vector<size_t>& out, complex<double> val)
{
}

void Tensor::set_entry(initializer_list<size_t>& in,
		       initializer_list<size_t>& out, complex<double> val)
{
}

// ########################### set_input #############################
void Tensor::set_input(size_t n, Tensor *g, size_t m)
{
}

// ########################### set_output ############################
void Tensor::set_output(size_t n, Tensor *g, size_t m)
{
}

// ########################### input_tensor ##########################
Tensor* Tensor::input_tensor(size_t n)
{
  return nullptr;
}

// ########################### output_tensor #########################
Tensor* Tensor::output_tensor(size_t n)
{
  return nullptr;
}

// ########################### input_num #############################
size_t Tensor::input_num(size_t n)
{
  return 0;
}

// ########################### output_num ############################
size_t Tensor::output_num(size_t n)
{
  return 0;
}
