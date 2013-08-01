// class representing a single tensor in the tensor network

#ifndef _TENSOR_HH
#define _TENSOR_HH

#include <complex>
#include <initializer_list>
#include <vector>
#include <gsl/gsl_matrix.h>

// A single tensor in the tensor network.  Note that, when the tensor
// represents a gate in a MERA the input should indicate the direction
// of *greater* renormalization flow.
class tensor
{
public:
  // Constructors and destructor.
  tensor(int nin, int nout, int inrank, int outrank);
  tensor(int nin, int nout, int rank) : tensor(nin, nout, rank, rank) {}
  tensor& operator=(const tensor&) = delete;
  tensor(const tensor&) = delete;
  virtual ~tensor();
  // Create a copy which shares the underlying matrix.
  virtual tensor *shallow_copy();
  // Create a shallow copy using the matrix' Hermitian conjutensor.
  virtual tensor *shallow_copy_conjugate();
  // Get or set the entry corresponding to the defined inputs and
  // outputs.  These functions must ensure that the input list is the
  // proper length.
  virtual std::complex<double> entry(const std::vector<int>& in,
			const std::vector<int>& out);
  virtual std::complex<double> entry(std::initializer_list<int>& in,
			std::initializer_list<int>& out);
  virtual void set_entry(const std::vector<int>& in,
		 const std::vector<int>& out, std::complex<double> val);
  virtual void set_entry(std::initializer_list<int>& in,
		 std::initializer_list<int>& out, std::complex<double> val);
  // Set input (output) n to correspond to output (input) m on tensor g.
  // This function must ensure the tensors are compatible (built from
  // same-ranked vector spaces) and set the links in both directions.
  virtual void set_input(int n, tensor *g, int m);
  virtual void set_output(int n, tensor *g, int m);
  // Get tensor and output (input) number associated with an input
  // (output).
  virtual tensor* input_tensor(int n);
  virtual tensor* output_tensor(int n);
  virtual int input_num(int n);
  virtual int output_num(int n);
protected:

private:
  // Number of input and output sites.
  int _nin;
  int _nout;
  // The following pointers represent arrays which encode the
  // placement of this tensor in the tensor network.  Specifically,
  // input n of this tensor is connected to output _indest[n] of tensor
  // _in[n].  _out and _outdest work analogously.
  tensor* _in;
  tensor* _out;
  int* _indest;
  int* _outdest;
  // Rank of input and output vector spaces.  In a MERA, the bottom
  // level has output rank equal to that of the underlying vector
  // space, and ranks grow when proceeding up through the network
  // until saturating the rank of the tensor network itself.  As a
  // result, the two ranks will often be the same.
  int _inrank;
  int _outrank;
  // Flag which is set if this is the Hermitian conjugate of the
  // underlying matrix.
  bool _conjugate;
  // The matrix itself.
  gsl_matrix_complex *matrix;

};

#endif // _TENSOR_HH
