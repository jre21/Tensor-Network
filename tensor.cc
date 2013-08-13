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

#include "log_msg.hh"
#include "matrix.hh"
#include "tensor.hh"
#include "utils.hh"

using std::complex;
using std::initializer_list;
using std::shared_ptr;
using std::vector;

// ########################### constructor ###########################
ConcreteTensor::ConcreteTensor(size_t nin, size_t nout,
			       size_t inrank, size_t outrank)
  : _nin{nin}, _nout{nout}, _inrank{inrank}, _outrank{outrank},
    _conjugate{false}
{
  _initialize(true);
}

ConcreteTensor::ConcreteTensor(MatrixStruct m)
  : _nin{m.nin}, _nout{m.nout}, _inrank{m.inrank},
    _outrank{m.outrank}, _conjugate{m.conjugate}, _matrix{m.matrix}
{
  _initialize(false);
}

// ########################### destructor ############################
ConcreteTensor::~ConcreteTensor()
{
  if(nullptr != _matrix)
    {
      for(size_t i = 0; i < _nin; ++i)
	_unset_input(i);
      for(size_t i = 0; i < _nout; ++i)
	_unset_output(i);
    }
}

// ###################################################################
size_t ConcreteTensor::input_rank()
{
  return _inrank;
}

size_t ConcreteTensor::output_rank()
{
  return _outrank;
}

// ###################################################################

// ########################### entry #################################
complex<double> ConcreteTensor::entry(const vector<size_t>& in,
				      const vector<size_t>& out)
{
  return _entry(in, out);
}

complex<double> ConcreteTensor::entry(initializer_list<size_t> in,
				      initializer_list<size_t> out)
{
  return _entry( vector<size_t>{in}, vector<size_t>{out} );
}

// ########################### set_entry #############################
void ConcreteTensor::set_entry(const vector<size_t>& in,
			       const vector<size_t>& out, complex<double> val)
{
  _set_entry(in,out,val);
}

void ConcreteTensor::set_entry(initializer_list<size_t> in,
			       initializer_list<size_t> out,
			       complex<double> val)
{
  _set_entry(vector<size_t>{in}, vector<size_t>{out}, val);
}

// ########################### set_input #############################
void ConcreteTensor::set_input(size_t n, Tensor *T, size_t m)
{
  _set_input(n, T, m);
}

// ########################### set_output ############################
void ConcreteTensor::set_output(size_t n, Tensor *T, size_t m)
{
  _set_output(n, T, m);
}

// ########################### input_tensor ##########################
Tensor* ConcreteTensor::input_tensor(size_t n)
{
  // guard against out-of-bounds arguments
#ifndef NO_ERROR_CHECKING
  if(n >= _nin)
    LOG_MSG_(FATAL) << kErrBounds << "argument of "
      "ConcreteTensor::input_tensor(): " << n <<
      " exceeds input list length " << _nin;
#endif // NO_ERROR_CHECKING

  return _in[n];
}

// ########################### output_tensor #########################
Tensor* ConcreteTensor::output_tensor(size_t n)
{
#ifndef NO_ERROR_CHECKING
  // guard against out-of-bounds arguments
  if(n >= _nout)
    LOG_MSG_(FATAL) << kErrBounds << "argument of "
      "ConcreteTensor::output_tensor(): " << n <<
      " exceeds output list length " << _nout;
#endif

  return _out[n];
}

// ########################### input_num #############################
size_t ConcreteTensor::input_num(size_t n)
{
#ifndef NO_ERROR_CHECKING
  // guard against out-of-bounds arguments
  if(n >= _nin)
    LOG_MSG_(FATAL) << kErrBounds << "argument of "
      "ConcreteTensor::input_num(): " << n <<
      " exceeds input list length " << _nin;
#endif // NO_ERROR_CHECKING

  return _indest[n];
}

// ########################### output_num ############################
size_t ConcreteTensor::output_num(size_t n)
{
#ifndef NO_ERROR_CHECKING
  // guard against out-of-bounds arguments
  if(n >= _nout)
    LOG_MSG_(FATAL) << kErrBounds << "argument of "
      "ConcreteTensor::output_num(): " << n <<
      " exceeds output list length " << _nout;
#endif // NO_ERROR_CHECKING

  return _outdest[n];
}

// ########################### matrix ################################
MatrixStruct ConcreteTensor::matrix(bool conjugate)
{
  MatrixStruct m;
  // If conjugate is true, swap inputs and outputs, and invert
  // _conjugate flag.
  if(!conjugate)
    {
      m.nin = _nin;
      m.nout = _nout;
      m.inrank = _inrank;
      m.outrank = _outrank;
      m.conjugate = _conjugate;
    }
  else
    {
      m.nin = _nout;
      m.nout = _nin;
      m.inrank = _outrank;
      m.outrank = _inrank;
      m.conjugate = !_conjugate;
    }
  // Create a copy of the initial matrix which won't delete the
  // underlying data when destroyed.
  m.matrix = shared_ptr<Matrix>{_matrix};

  return m;
}

// ########################### _entry ################################
complex<double> ConcreteTensor::_entry(const vector<size_t>& in,
			       const vector<size_t>& out)
{
  if(!_conjugate)
    return  _matrix->get( _pack_input(in), _pack_output(out) );
  else
    // Exchange rows and columns and take the complex conjugate to
    // simulate retrieving data from the Hermitian conjugate of the
    // underlying matrix.
    return conjugate(_matrix->get( _pack_output(out), _pack_input(in) ));
}

// ########################### _set_entry ############################
void ConcreteTensor::_set_entry(const vector<size_t>& in,
				const vector<size_t>& out, complex<double> val)
{
  if(!_conjugate)
    _matrix->set(_pack_input(in), _pack_output(out), val );
  else
    // exchange rows and columns and take the complex conjugate to
    // simulate retrieving data from the Hermitian conjugate of the
    // underlying matrix
    _matrix->set( _pack_output(out), _pack_input(in), conjugate(val) );
}

// ########################### _pack_input ###########################
size_t ConcreteTensor::_pack_input(const vector<size_t>& in)
{
#ifndef NO_ERROR_CHECKING
  // guard against wrongly-sized argument lists
  if(in.size() != _nin)
    LOG_MSG_(FATAL) << kErrListLength << "argument of "
      "ConcreteTensor::_pack_input(): expected length " << _nin <<
      " but detected " << in.size();
#endif // NO_ERROR_CHECKING

  // convert from argument list to single argument for matrix
  size_t ret = 0;
  for(size_t i = 0, mult = 1; i < _nin; ++i, mult *= _inrank)
    {
#ifndef NO_ERROR_CHECKING
      // guard against out-of-bounds arguments
      if(in[_nin-i-1] >= _inrank)
	LOG_MSG_(FATAL) << kErrBounds << "argument of "
	  "ConcreteTensor::_pack_input(): element " << _nin-i-1 <<
	  " has value " << in[_nin-i-1] <<
	  " which exceeds vector space rank of " << _inrank;
#endif // NO_ERROR_CHECKING

      ret += mult * in[_nin-i-1];
    }

  return ret;
}

// ########################### _unpack_input #########################
vector<size_t> ConcreteTensor::_unpack_input(size_t in)
{
  // convert from matrix index to index list for tensor
  vector<size_t> ret{_nin};
  for(size_t i = 0; i < _nin; ++i, in /= _inrank)
    ret[_nin-i-1] = in % _inrank;

#ifndef NO_ERROR_CHECKING
  // guard against out-of-bounds arguments
  if(in > 0)
    LOG_MSG_(FATAL) << kErrBounds << "argument of "
      "ConcreteTensor::_unpack_input() exceeds vector space rank";
#endif // NO_ERROR_CHECKING

  return ret;
}

// ########################### _pack_output ##########################
size_t ConcreteTensor::_pack_output(const vector<size_t>& out)
{
#ifndef NO_ERROR_CHECKING
  // guard against wrongly-sized argument lists
  if(out.size() != _nout)
    LOG_MSG_(FATAL) << kErrListLength << "argument of "
      "ConcreteTensor::_pack_output(): expected length " << _nout <<
      " but detected " << out.size();
#endif // NO_ERROR_CHECKING

  // convert from argument list to single argument for matrix
  size_t ret = 0;
  for(size_t i = 0, mult = 1; i < _nout; ++i, mult *= _outrank)
    {
#ifndef NO_ERROR_CHECKING
      // guard against out-of-bounds arguments
      if(out[_nout-i-1] >= _outrank)
	LOG_MSG_(FATAL) << kErrBounds << "argument of "
	  "ConcreteTensor::_pack_output(): element " << _nout-i-1 <<
	  " has value " << out[_nout-i-1] <<
	  " which exceeds vector space rank of " << _outrank;
#endif // NO_ERROR_CHECKING

      ret += mult * out[_nout-i-1];
    }
  return ret;
}

// ########################### _unpack_output ########################
vector<size_t> ConcreteTensor::_unpack_output(size_t out)
{
  // convert from matrix index to index list for tensor
  vector<size_t> ret{_nout};
  for(size_t i = 0; i < _nout; ++i, out /= _outrank)
    ret[_nout-i-1] = out % _outrank;

#ifndef NO_ERROR_CHECKING
  // guard against out-of-bounds arguments
  if(out > 0)
    LOG_MSG_(FATAL) << kErrBounds << "argument of "
      "ConcreteTensor::_unpack_output() exceeds vector space rank";
#endif // NO_ERROR_CHECKING

  return ret;
}

// ########################### _set_input ############################
void ConcreteTensor::_set_input(size_t n, Tensor *T, size_t m)
{
  // ensure current input is cleared properly
  _unset_input(n);

  // if nullptr was passed, we're just unsetting the input
  if(nullptr == T) return;

#ifndef NO_ERROR_CHECKING
  // ensure tensors are formed from compatible vector spaces
  if(nullptr != T && input_rank() != T->output_rank())
    LOG_MSG_(FATAL) << kErrIncompatible << "tensor arguments passed to "
      "ConcreteTensor::_set_input() have differing vector space ranks: " <<
      input_rank() << " and " << T->output_rank();
#endif // NO_ERROR_CHECKING

  // link the two tensors
  _set_input_self(n, T, m);
  Tensor::_set_output(T, m, this, n);
}

// ########################### _set_output ###########################
void ConcreteTensor::_set_output(size_t n, Tensor *T, size_t m)
{
  // ensure that output is cleared properly
  _unset_output(n);

  // if a nullptr was passed, we're just unsetting the output
  if(nullptr == T) return;

#ifndef NO_ERROR_CHECKING  
  // ensure tensors are formed from compatible vector spaces
  if(output_rank() != T->input_rank())
    LOG_MSG_(FATAL) << kErrIncompatible << "tensor arguments passed to "
      "ConcreteTensor::_set_output() have differing vector space ranks: " <<
      output_rank() << " and " << T->input_rank();
#endif // NO_ERROR_CHECKING

  // link the two tensors
  _set_output_self(n, T, m);
  Tensor::_set_input(T, m, this, n);
}

// ########################### _unset_input ##########################
void ConcreteTensor::_unset_input(size_t n)
{
#ifndef NO_ERROR_CHECKING
  // guard against out-of-bounds arguments
  if(n >= _nin)
    LOG_MSG_(FATAL) << kErrBounds << "first argument of "
      "ConcreteTensor::_unset_input(): " << n <<
      " exceeds input list length " << _nin;
#endif // NO_ERROR_CHECKING

  // unlink tensors
  if(_in[n] != nullptr)
    Tensor::_set_output(_in[n], _indest[n], nullptr, 0);
  _set_input_self(n, nullptr, 0);
}

// ########################### _unset_output #########################
void ConcreteTensor::_unset_output(size_t n)
{
#ifndef NO_ERROR_CHECKING
  // guard against out-of-bounds arguments
  if(n >= _nout)
    LOG_MSG_(FATAL) << kErrBounds << "first argument of "
      "ConcreteTensor::_unset_output(): " << n <<
      " exceeds input list length " << _nout;
#endif // NO_ERROR_CHECKING

  // unlink tensors
  if(_out[n] != nullptr)
    Tensor::_set_input(_out[n], _outdest[n], nullptr, 0);
  _set_output_self(n, nullptr, 0);
}

// ########################### _set_input_self #######################
void ConcreteTensor::_set_input_self(size_t n, Tensor *T, size_t m)
{
#ifndef NO_ERROR_CHECKING
  // guard against out-of-bounds arguments
  if(n >= _nin)
    LOG_MSG_(FATAL) << kErrBounds << "first argument of "
      "ConcreteTensor::_set_input_self(): " << n <<
      " exceeds input list length " << _nin;
#endif // NO_ERROR_CHECKING

  // set input tensor and destination
  _in[n] = T;
  _indest[n] = T != nullptr ? m : 0;
}

// ########################### _set_output_self ######################
void ConcreteTensor::_set_output_self(size_t n, Tensor *T, size_t m)
{
#ifndef NO_ERROR_CHECKING
  // guard against out-of-bounds arguments
  if(n >= _nout)
    LOG_MSG_(FATAL) << kErrBounds << "first argument of "
      "ConcreteTensor::_set_output_self(): " << n <<
      " exceeds output list length " << _nout;
#endif // NO_ERROR_CHECKING

  // set output tensor and destination
  _out[n] = T;
  _outdest[n] = T != nullptr ? m : 0;
}

// ########################### _initialize ###########################
void ConcreteTensor::_initialize(bool init_matrix)
{
  // initialize variables
  _in = vector<Tensor*>(_nin, nullptr);
  _out = vector<Tensor*>(_nout, nullptr);
  _indest = vector<size_t>(_nin,0);
  _outdest = vector<size_t>(_nout,0);

  if(!init_matrix) return;

  // If the input (output) vector space is zero, the matrix is empty
  // unless the tensor takes no inputs (outputs).  In this case just
  // set the matrix to null.  For convenience in creating vectors,
  // allow setting both the number of inputs (outputs) and the rank of
  // the associated vector space to 0.
  if( (_inrank != 0 || _nin == 0) && (_outrank != 0 || _nout == 0) )
    {
      // calculate powers by hand to avoid cast to floating point
      size_t in = 1, out = 1;
      for (size_t i=0; i<_nin; i++) in *= _inrank;
      for (size_t i=0; i<_nout; i++) out *= _outrank;
      _matrix = shared_ptr<Matrix> {new GSLMatrix{in,out}};
    }
}
