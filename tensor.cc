#include <math.h>
#include <gsl/gsl_complex.h>
#include "log_msg.hh"
#include "tensor.hh"
#include "utils.hh"

using std::complex;
using std::initializer_list;
using std::vector;

// ########################### constructor ###########################
ConcreteTensor::ConcreteTensor(size_t nin, size_t nout,
			       size_t inrank, size_t outrank)
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
ConcreteTensor::~ConcreteTensor()
{
  if(nullptr != _matrix)
    {
      gsl_matrix_complex_free(_matrix);
      _matrix = nullptr;
    }
}

// ########################### copy_of ###############################
Tensor *ConcreteTensor::copy_of(Tensor *T)
{
  return nullptr;
}

// ########################### conjugate_of ##########################
Tensor *ConcreteTensor::conjugate_of(Tensor *T)
{
  return nullptr;
}

// ########################### entry #################################
complex<double> ConcreteTensor::entry(const vector<size_t>& in,
			      const vector<size_t>& out)
{
  return _entry(in, out);
}

complex<double> ConcreteTensor::entry(initializer_list<size_t>& in,
			      initializer_list<size_t>& out)
{
  return _entry( vector<size_t>(in), vector<size_t>(out) );
}

// ########################### set_entry #############################
void ConcreteTensor::set_entry(const vector<size_t>& in,
		       const vector<size_t>& out, complex<double> val)
{
  _set_entry(in,out,val);
}

void ConcreteTensor::set_entry(initializer_list<size_t>& in,
		       initializer_list<size_t>& out, complex<double> val)
{
  _set_entry( vector<size_t>(in), vector<size_t>(out), val);
}

// ########################### set_input #############################
void ConcreteTensor::set_input(size_t n, Tensor *g, size_t m)
{
}

// ########################### set_output ############################
void ConcreteTensor::set_output(size_t n, Tensor *g, size_t m)
{
}

// ########################### input_tensor ##########################
Tensor* ConcreteTensor::input_tensor(size_t n)
{
  return nullptr;
}

// ########################### output_tensor #########################
Tensor* ConcreteTensor::output_tensor(size_t n)
{
  return nullptr;
}

// ########################### input_num #############################
size_t ConcreteTensor::input_num(size_t n)
{
  return 0;
}

// ########################### output_num ############################
size_t ConcreteTensor::output_num(size_t n)
{
  return 0;
}

// ########################### _entry ################################
complex<double> ConcreteTensor::_entry(const vector<size_t>& in,
			       const vector<size_t>& out)
{
  return complex_from_gsl(gsl_matrix_complex_get( _matrix,
						  _pack_input(in),
						  _pack_output(out)));
}

// ########################### _set_entry ############################
void ConcreteTensor::_set_entry(const vector<size_t>& in,
			const vector<size_t>& out, complex<double> val)
{
  gsl_matrix_complex_set( _matrix, _pack_input(in),
			  _pack_output(out), complex_to_gsl(val) );
}

// ########################### _pack_input ###########################
size_t ConcreteTensor::_pack_input(const vector<size_t>& in)
{
  if(in.size() != _nin)
    LOG_MSG_(FATAL) << kErrListLength << "input to ConcreteTensor::_pack_input(): "
      "expected length " << _nin << " but detected " << in.size();
  size_t ret = 0;
  for(size_t i = 0, mult = 1; i < _nin; i++, mult *= _inrank)
    {
      if(in[_nin-i-1] >= _inrank)
	LOG_MSG_(FATAL) << kErrBounds << "input to ConcreteTensor::_pack_input(): "
	  "element " << _nin-i-1 << " has value " << in[_nin-i-1] <<
	  " which exceeds vector space rank of " << _inrank;
      ret += mult * in[_nin-i-1];
    }
  return ret;
}

// ########################### _unpack_input #########################
vector<size_t> ConcreteTensor::_unpack_input(size_t in)
{
  vector<size_t> ret(_nin);
  for(size_t i = 0; i < _nin; i++, in /= _inrank)
    ret[_nin-i-1] = in % _inrank;
  if(in > 0)
    LOG_MSG_(FATAL) << kErrBounds << "input to ConcreteTensor::_unpack_input()"
      "exceeds vector space rank";
  return ret;
}

// ########################### _pack_output ##########################
size_t ConcreteTensor::_pack_output(const vector<size_t>& out)
{
  if(out.size() != _nout)
    LOG_MSG_(FATAL) << kErrListLength << "input to ConcreteTensor::_pack_output(): "
      "expected length " << _nout << " but detected " << out.size();
  size_t ret = 0;
  for(size_t i = 0, mult = 1; i < _nout; i++, mult *= _outrank)
    {
      if(out[_nout-i-1] >= _outrank)
	LOG_MSG_(FATAL) << kErrBounds << "input to ConcreteTensor::_pack_output(): "
	  "element " << _nout-i-1 << " has value " << out[_nout-i-1] <<
	  " which exceeds vector space rank of " << _outrank;
      ret += mult * out[_nout-i-1];
    }
  return ret;
}

// ########################### _unpack_output ########################
vector<size_t> ConcreteTensor::_unpack_output(size_t out)
{
  vector<size_t> ret(_nout);
  for(size_t i = 0; i < _nout; i++, out /= _outrank)
    ret[_nout-i-1] = out % _outrank;
  if(out > 0)
    LOG_MSG_(FATAL) << kErrBounds << "input to ConcreteTensor::_unpack_output()"
      "exceeds vector space rank";
  return ret;
}

// ########################### _set_input_self########################
void ConcreteTensor::_set_input_self(size_t n, Tensor *g, size_t m)
{
}

// ########################### _set_output_self ######################
void ConcreteTensor::_set_output_self(size_t n, Tensor *g, size_t m)
{
}
