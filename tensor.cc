#include "tensor.hh"

using std::complex;
using std::initializer_list;
using std::vector;

// ########################### constructor ###########################
Tensor::Tensor(size_t nin, size_t nout, size_t inrank, size_t outrank)
{
}

// ########################### destructor ############################
Tensor::~Tensor()
{
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
