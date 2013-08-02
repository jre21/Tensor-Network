#include <cmath>
#include <gsl/gsl_complex.h>
#include "messages.hh"
#include "tensor.hh"
#include "utils.hh"

using std::complex;
using std::initializer_list;
using std::vector;

// ########################### constructor ###########################
Tensor::Tensor(size_t nin, size_t nout, size_t inrank, size_t outrank)
  : _nin(nin), _nout(nout), _inrank(inrank), _outrank(outrank)
{
  // initialize variables
  _in = _out = nullptr;
  _indest = vector<size_t>(nin,0);
  _outdest = vector<size_t>(nout,0);
  _conjugate = false;

  // If any argument is 0, the matrix is empty.  We leave it null in this case.
  if(nin != 0 && nout != 0 && inrank != 0 && outrank != 0)
    {
      // calculate powers by hand to avoid cast to floating point
      int in = 1, out = 1;
      for (size_t i=0; i<nin; i++) in *= inrank;
      for (size_t i=0; i<nout; i++) out *= outrank;
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
  return _entry(in, out);
}

complex<double> Tensor::entry(initializer_list<size_t>& in,
			      initializer_list<size_t>& out)
{
  return _entry( vector<size_t>(in), vector<size_t>(out) );
}

// ########################### set_entry #############################
void Tensor::set_entry(const vector<size_t>& in,
		       const vector<size_t>& out, complex<double> val)
{
  _set_entry(in,out,val);
}

void Tensor::set_entry(initializer_list<size_t>& in,
		       initializer_list<size_t>& out, complex<double> val)
{
  _set_entry( vector<size_t>(in), vector<size_t>(out), val);
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

// ########################### _entry ################################
complex<double> Tensor::_entry(const vector<size_t>& in,
			       const vector<size_t>& out)
{
  return complex_from_gsl(gsl_matrix_complex_get( _matrix,
						  _pack_input(in),
						  _pack_output(out)));
}

// ########################### _set_entry ############################
void Tensor::_set_entry(const vector<size_t>& in,
			const vector<size_t>& out, complex<double> val)
{
  gsl_matrix_complex_set( _matrix, _pack_input(in),
			  _pack_output(out), complex_to_gsl(val) );
}

// ########################### _pack_input ###########################
size_t Tensor::_pack_input(const vector<size_t>& in)
{
  if(in.size() != _nin)
    ERROR(kErrListLength, "input to Tensor::_pack_input()");
  size_t ret = 0;
  for(size_t i = 0, mult = 1; i < _nin; i++, mult *= _inrank)
    {
      if(in[_nin-i-1] >= _inrank)
	ERROR(kErrBounds, "input to Tensor::_pack_input()");
      ret += mult * in[_nin-i-1];
    }
  return ret;
}

// ########################### _unpack_input #########################
vector<size_t> Tensor::_unpack_input(size_t in)
{
  vector<size_t> ret(_nin);
  for(size_t i = 0; i < _nin; i++, in /= _inrank)
    ret[_nin-i-1] = in % _inrank;
  if(in > 0)
    ERROR(kErrBounds, "input to Tensor::_unpack_input");
  return ret;
}

// ########################### _pack_output ##########################
size_t Tensor::_pack_output(const vector<size_t>& out)
{
  if(out.size() != _nout)
    ERROR(kErrListLength, "input to Tensor::_pack_output()");
  size_t ret = 0;
  for(size_t i = 0, mult = 1; i < _nout; i++, mult *= _outrank)
    {
      if(out[_nout-i-1] >= _outrank)
	ERROR(kErrBounds, "input to Tensor::_pack_output()");
      ret += mult * out[_nout-i-1];
    }
  return ret;
}

// ########################### _unpack_output ########################
vector<size_t> Tensor::_unpack_output(size_t out)
{
  vector<size_t> ret(_nout);
  for(size_t i = 0; i < _nout; i++, out /= _outrank)
    ret[_nout-i-1] = out % _outrank;
  if(out > 0)
    ERROR(kErrBounds, "input to Tensor::_unpack_input");
  return ret;
}
