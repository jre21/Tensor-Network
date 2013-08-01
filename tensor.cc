#include "tensor.hh"

using std::complex;
using std::initializer_list;
using std::vector;

// ########################### constructor ###########################
Tensor::Tensor(int nin, int nout, int inrank, int outrank)
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
complex<double> Tensor::entry(const vector<int>& in,
			      const vector<int>& out)
{
  return complex<double>();
}

complex<double> Tensor::entry(initializer_list<int>& in,
			      initializer_list<int>& out)
{
  return complex<double>();
}

// ########################### set_entry #############################
void Tensor::set_entry(const vector<int>& in,
		       const vector<int>& out, complex<double> val)
{
}

void Tensor::set_entry(initializer_list<int>& in,
		       initializer_list<int>& out, complex<double> val)
{
}

// ########################### set_input #############################
void Tensor::set_input(int n, Tensor *g, int m)
{
}

// ########################### set_output ############################
void Tensor::set_output(int n, Tensor *g, int m)
{
}

// ########################### input_tensor ##########################
Tensor* Tensor::input_tensor(int n)
{
  return nullptr;
}

// ########################### output_tensor #########################
Tensor* Tensor::output_tensor(int n)
{
  return nullptr;
}

// ########################### input_num #############################
int Tensor::input_num(int n)
{
  return 0;
}

// ########################### output_num ############################
int Tensor::output_num(int n)
{
  return 0;
}
