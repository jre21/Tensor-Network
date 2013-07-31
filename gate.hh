// class representing a single gate in the tensor network

#ifndef _GATE_HH
#define _GATE_HH

#include <complex>
#include <initializer_list>
#include <vector>
#include <gsl/gsl_matrix.h>

// A single gate in the tensor network.  In a MERA this will be either
// a disentangler or an isometry gate.  Note that the input indicates
// the direction of *greater* renormalization flow.
class gate
{
public:
  // Constructors and destructor.
  gate(int nin, int nout, int inrank, int outrank);
  gate(int nin, int nout, int rank) : gate(nin, nout, rank, rank) {}
  gate& operator=(const gate&) = delete;
  gate(const gate&) = delete;
  virtual ~gate();
  // Create a copy which shares the underlying matrix.
  virtual gate *shallow_copy();
  // Create a shallow copy using the matrix' Hermitian conjugate.
  virtual gate *shallow_copy_conjugate();
  // Get or set the entry corresponding to the defined inputs and
  // outputs.  These functions must ensure that the input list is the
  // proper length.
  virtual complex<double> entry(const std::vector<int>& in,
			const std::vector<int>& out);
  virtual complex<double> entry(std::initializer_list<int>& in,
			std::initializer_list<int>& out);
  virtual void set_entry(const std::vector<int>& in,
		 const std::vector<int>& out, complex<double> val);
  virtual void set_entry(std::initializer_list<int>& in,
		 std::initializer_list<int>& out, complex<double> val);
  // Set input (output) n to correspond to output (input) m on gate g.
  // This function must ensure the gates are compatible (built from
  // same-ranked vector spaces) and set the links in both directions.
  virtual void set_input(int n, gate *g, int m);
  virtual void set_output(int n, gate *g, int m);
  // Get gate and output (input) number associated with an input
  // (output).
  virtual gate* input_gate(int n);
  virtual gate* output_gate(int n);
  virtual int input_num(int n);
  virtual int output_num(int n);
protected:

private:
  // Number of input and output sites.
  int _nin;
  int _nout;
  // The following pointers represent arrays which encode the
  // placement of this gate in the tensor network.  Specifically,
  // input n of this gate is connected to output _indest[n] of gate
  // _in[n].  _out and _outdest work analogously.
  gate* _in;
  gate* _out;
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

#endif // _GATE_HH
