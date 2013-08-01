#include "tensor.hh"

// ########################### constructor ###########################
Tensor::Tensor(int nin, int nout, int inrank, int outrank)
{
}

// ########################### destructor ############################
Tensor::~Tensor()
{
}

// ########################### shallow_copy ##########################
Tensor *Tensor::shallow_copy()
{
  return nullptr;
}

// ########################### shallow_copy_conjugate ################
Tensor *Tensor::shallow_copy_conjugate()
{
  return nullptr;
}

// ########################### entry #################################
std::complex<double> Tensor::entry(const std::vector<int>& in,
			      const std::vector<int>& out)
{
  return std::complex<double>();
}

std::complex<double> Tensor::entry(std::initializer_list<int>& in,
			      std::initializer_list<int>& out)
{
  return std::complex<double>();
}

// ########################### set_entry #############################
void Tensor::set_entry(const std::vector<int>& in,
		       const std::vector<int>& out,
		       std::complex<double> val)
{
}

void Tensor::set_entry(std::initializer_list<int>& in,
		       std::initializer_list<int>& out,
		       std::complex<double> val)
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
