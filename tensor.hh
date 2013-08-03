// class representing a single tensor in the tensor network

#ifndef TENSOR_HH_
#define TENSOR_HH_

#include <complex>
#include <initializer_list>
#include <vector>
#include <gsl/gsl_matrix.h>

// A single tensor in the tensor network.  Note that, when the tensor
// represents a gate in a MERA the input should indicate the direction
// of *greater* renormalization flow.
class Tensor
{
public:
  virtual ~Tensor() {}
  // Get or set the entry corresponding to the defined inputs and
  // outputs.  These functions must ensure that the input list is the
  // proper length.
  virtual std::complex<double> entry(const std::vector<size_t>& in,
				     const std::vector<size_t>& out) = 0;
  virtual std::complex<double> entry(std::initializer_list<size_t>& in,
				     std::initializer_list<size_t>& out) = 0;
  virtual void set_entry(const std::vector<size_t>& in,
			 const std::vector<size_t>& out,
			 std::complex<double> val) = 0;
  virtual void set_entry(std::initializer_list<size_t>& in,
			 std::initializer_list<size_t>& out,
			 std::complex<double> val) = 0;
  // Set input (output) n to correspond to output (input) m on tensor g.
  // This function must ensure the tensors are compatible (built from
  // same-ranked vector spaces) and set the links in both directions.
  // If g is null, instead unset the input or output.
  virtual void set_input(size_t n, Tensor *g, size_t m) = 0;
  virtual void set_output(size_t n, Tensor *g, size_t m) = 0;
  // Get tensor and output (input) number associated with an input
  // (output).
  virtual Tensor* input_tensor(size_t n) = 0;
  virtual Tensor* output_tensor(size_t n) = 0;
  virtual size_t input_num(size_t n) = 0;
  virtual size_t output_num(size_t n) = 0;
protected:
  // Like set_(input|output) above, but setting only a single
  // direction.  The above should call these functions on both objects.
  virtual void _set_input_self(size_t n, Tensor *g, size_t m) = 0;
  virtual void _set_output_self(size_t n, Tensor *g, size_t m) = 0;
};

class ConcreteTensor : public Tensor
{
public:
  // Constructors and destructor.
  ConcreteTensor(size_t nin, size_t nout, size_t inrank, size_t outrank);
  ConcreteTensor(size_t nin, size_t nout, size_t rank)
    : ConcreteTensor(nin, nout, rank, rank) {}
  ConcreteTensor& operator=(const Tensor&) = delete;
  ConcreteTensor(const Tensor&) = delete;
  virtual ~ConcreteTensor();
  // From interface Tensor.
  static Tensor *copy_of(Tensor *T);
  static Tensor *conjugate_of(Tensor *T);
  virtual std::complex<double> entry(const std::vector<size_t>& in,
			const std::vector<size_t>& out);
  virtual std::complex<double> entry(std::initializer_list<size_t>& in,
			std::initializer_list<size_t>& out);
  virtual void set_entry(const std::vector<size_t>& in,
		 const std::vector<size_t>& out, std::complex<double> val);
  virtual void set_entry(std::initializer_list<size_t>& in,
		 std::initializer_list<size_t>& out, std::complex<double> val);
  virtual void set_input(size_t n, Tensor *g, size_t m);
  virtual void set_output(size_t n, Tensor *g, size_t m);
  virtual Tensor* input_tensor(size_t n);
  virtual Tensor* output_tensor(size_t n);
  virtual size_t input_num(size_t n);
  virtual size_t output_num(size_t n);
protected:
  // Methods interacting directly with underlying data.  Where these
  // messages are virtual, they should only be overridden by mock
  // objects.
  std::complex<double> _entry(const std::vector<size_t>& in,
			      const std::vector<size_t>& out);
  void _set_entry(const std::vector<size_t>& in,
		  const std::vector<size_t>& out, std::complex<double> val);
  // convert between tensor notation for the interface and matrix
  // notation for underlying storage
  size_t _pack_input(const std::vector<size_t>& in);
  std::vector<size_t> _unpack_input(size_t in);
  size_t _pack_output(const std::vector<size_t>& out);
  std::vector<size_t> _unpack_output(size_t out);
  // From interface Tensor.
  virtual void _set_input_self(size_t n, Tensor *g, size_t m) final;
  virtual void _set_output_self(size_t n, Tensor *g, size_t m) final;
private:
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
  // underlying matrix.
  bool _conjugate;
  // The following pointers represent arrays which encode the
  // placement of this tensor in the tensor network.  Specifically,
  // input n of this tensor is connected to output _indest[n] of tensor
  // _in[n].  _out and _outdest work analogously.
  Tensor* _in;
  Tensor* _out;
  std::vector<size_t> _indest;
  std::vector<size_t> _outdest;
  // The matrix itself.
  gsl_matrix_complex *_matrix;
};

#endif // TENSOR_HH_
