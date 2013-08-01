#include "tensor.hh"

// ########################### constructor ###########################
tensor::tensor(int nin, int nout, int inrank, int outrank)
{
}

// ########################### destructor ############################
tensor::~tensor()
{
}

// ########################### shallow_copy ##########################
tensor *tensor::shallow_copy()
{
  return nullptr;
}

// ########################### shallow_copy_conjugate ################
tensor *tensor::shallow_copy_conjugate()
{
  return nullptr;
}

// ########################### entry #################################
std::complex<double> tensor::entry(const std::vector<int>& in,
			      const std::vector<int>& out)
{
  return std::complex<double>();
}

std::complex<double> tensor::entry(std::initializer_list<int>& in,
			      std::initializer_list<int>& out)
{
  return std::complex<double>();
}

// ########################### set_entry #############################
void tensor::set_entry(const std::vector<int>& in,
		       const std::vector<int>& out,
		       std::complex<double> val)
{
}

void tensor::set_entry(std::initializer_list<int>& in,
		       std::initializer_list<int>& out,
		       std::complex<double> val)
{
}

// ########################### set_input #############################
void tensor::set_input(int n, tensor *g, int m)
{
}

// ########################### set_output ############################
void tensor::set_output(int n, tensor *g, int m)
{
}

// ########################### input_tensor ##########################
tensor* tensor::input_tensor(int n)
{
  return nullptr;
}

// ########################### output_tensor #########################
tensor* tensor::output_tensor(int n)
{
  return nullptr;
}

// ########################### input_num #############################
int tensor::input_num(int n)
{
  return 0;
}

// ########################### output_num ############################
int tensor::output_num(int n)
{
  return 0;
}
