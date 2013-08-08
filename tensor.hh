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

// class representing a single tensor in the tensor network

#ifndef TENSOR_HH_
#define TENSOR_HH_

#include <complex>
#include <initializer_list>
#include <vector>
#include <gsl/gsl_matrix.h>

// Data format storing the information needed to reconstruct a tensor.
// Note that setting conjugate indicates only that the complex
// conjugate of matrix should be used.  Other variables should still
// be used as-is.
struct Matrix
{
  size_t nin;
  size_t nout;
  size_t inrank;
  size_t outrank;
  bool conjugate;
  gsl_matrix_complex *matrix;
};

// A single tensor in the tensor network.  Note that, when the tensor
// represents a gate in a MERA the input should indicate the direction
// of *greater* renormalization flow.
class Tensor
{
public:
  virtual ~Tensor() {}
  // Get the rank of the input and output vector spaces
  virtual size_t input_rank() = 0;
  virtual size_t output_rank() = 0;
  // Get or set the entry corresponding to the defined inputs and
  // outputs.  These functions must ensure that the input list is the
  // proper length.
  virtual std::complex<double> entry(const std::vector<size_t>& in,
				     const std::vector<size_t>& out) = 0;
  virtual std::complex<double> entry(std::initializer_list<size_t> in,
				     std::initializer_list<size_t> out) = 0;
  virtual void set_entry(const std::vector<size_t>& in,
			 const std::vector<size_t>& out,
			 std::complex<double> val) = 0;
  virtual void set_entry(std::initializer_list<size_t> in,
			 std::initializer_list<size_t> out,
			 std::complex<double> val) = 0;
  // Set input (output) n to correspond to output (input) m on tensor g.
  // This function must ensure the tensors are compatible (built from
  // same-ranked vector spaces) and set the links in both directions.
  // If g is null, instead unset the input or output.
  virtual void set_input(size_t n, Tensor *T, size_t m) = 0;
  virtual void set_output(size_t n, Tensor *T, size_t m) = 0;
  // Get tensor and output (input) number associated with an input
  // (output).  If unset, return null and 0, respectively.
  virtual Tensor* input_tensor(size_t n) = 0;
  virtual Tensor* output_tensor(size_t n) = 0;
  virtual size_t input_num(size_t n) = 0;
  virtual size_t output_num(size_t n) = 0;
  // return a struct representing the matrix or its conjugate.  Note
  // that this is a shallow copy, but that a tensor object constructed
  // from it will not delete the underlying data when destructed.
  virtual Matrix matrix(bool conjugate = false) = 0;
protected:
  // Like set_(input|output) above, but setting only a single
  // direction.  The above should call these functions on both objects.
  virtual void _set_input_self(size_t n, Tensor *T, size_t m) = 0;
  virtual void _set_output_self(size_t n, Tensor *T, size_t m) = 0;
  static void _set_input(Tensor *T, size_t n, Tensor *TT, size_t m)
  { T->_set_input_self(n,TT,m); }
  static void _set_output(Tensor *T, size_t n, Tensor *TT, size_t m)
  { T->_set_output_self(n,TT,m); }
};

// The actual implementation of a tensor.  Most interface functions
// simply call a protected function which provides the actual
// implementation.  This is so that subclasses can easily override the
// interface function without code duplication.
class ConcreteTensor : public Tensor
{
public:
  // Constructors and destructor.
  ConcreteTensor(size_t nin, size_t nout, size_t inrank, size_t outrank);
  ConcreteTensor(size_t nin, size_t nout, size_t rank)
    : ConcreteTensor(nin, nout, rank, rank) {}
  ConcreteTensor(Matrix m);
  ConcreteTensor& operator=(const Tensor&) = delete;
  ConcreteTensor(const Tensor&) = delete;
  ~ConcreteTensor();
  // From interface Tensor.
  size_t input_rank() override;
  size_t output_rank() override;
  std::complex<double> entry(const std::vector<size_t>& in,
			     const std::vector<size_t>& out) override;
  std::complex<double> entry(std::initializer_list<size_t> in,
			     std::initializer_list<size_t> out) override;
  void set_entry(const std::vector<size_t>& in,
		 const std::vector<size_t>& out,
		 std::complex<double> val) override;
  void set_entry(std::initializer_list<size_t> in,
		 std::initializer_list<size_t> out,
		 std::complex<double> val) override;
  void set_input(size_t n, Tensor *T, size_t m) override;
  void set_output(size_t n, Tensor *T, size_t m) override;
  Tensor* input_tensor(size_t n) override;
  Tensor* output_tensor(size_t n) override;
  size_t input_num(size_t n) override;
  size_t output_num(size_t n) override;
  Matrix matrix(bool conjugate = false) override;
protected:
  // Methods interacting directly with underlying data.
  std::complex<double> _entry(const std::vector<size_t>& in,
			      const std::vector<size_t>& out);
  void _set_entry(const std::vector<size_t>& in,
		  const std::vector<size_t>& out, std::complex<double> val);
  // Convert between tensor notation for the interface and matrix
  // notation for underlying storage.
  size_t _pack_input(const std::vector<size_t>& in);
  std::vector<size_t> _unpack_input(size_t in);
  size_t _pack_output(const std::vector<size_t>& out);
  std::vector<size_t> _unpack_output(size_t out);
  // Check that the two tensors are compatible, and link them for
  // multiplication.
  void _set_input(size_t n, Tensor *T, size_t m);
  void _set_output(size_t n, Tensor *T, size_t m);
  // Unlink the tensor connected on site n.
  void _unset_input(size_t n);
  void _unset_output(size_t n);
  // From interface Tensor.
  void _set_input_self(size_t n, Tensor *T, size_t m) override final;
  void _set_output_self(size_t n, Tensor *T, size_t m) override final;
private:
  void _initialize(bool init_matrix);
  // Number of input and output sites.
  size_t _nin;
  size_t _nout;
  // Rank of input and output vector spaces.  In a MERA, the bottom
  // level has output rank equal to that of the underlying vector
  // space, and ranks grow when proceeding up through the network
  // until saturating the rank of the tensor network itself.  As a
  // result, the two ranks will often be the same.
  size_t _inrank;
  size_t _outrank;
  // Flag which is set if this is the Hermitian conjugate of the
  // underlying matrix.  Functions which directly interact with matrix
  // elements must check this flag and alter their behavior appropriately.
  bool _conjugate;
  // The following pointers represent arrays which encode the
  // placement of this tensor in the tensor network.  Specifically,
  // input n of this tensor is connected to output _indest[n] of tensor
  // _in[n].  _out and _outdest work analogously.
  std::vector<Tensor*> _in;
  std::vector<Tensor*> _out;
  std::vector<size_t> _indest;
  std::vector<size_t> _outdest;
  // The matrix itself.
  gsl_matrix_complex *_matrix;
};

#endif // TENSOR_HH_
