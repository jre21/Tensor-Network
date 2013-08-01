#ifndef _MOCK_TENSOR_HH
#define _MOCK_TENSOR_HH

#include <gmock/gmock.h>
#include "../tensor.hh"

class MockTensor : public Tensor
{
public:
  MockTensor() : Tensor(0,0,0,0) {}
  MOCK_METHOD2(entry, std::complex<double>
	       (const std::vector<int>& in, const std::vector<int>& out));
  MOCK_METHOD2(entry, std::complex<double>
	       (std::initializer_list<int>& in,
		std::initializer_list<int>& out));
  MOCK_METHOD3(set_entry, void
	       (const std::vector<int>& in,
		const std::vector<int>& out, std::complex<double> val));
  MOCK_METHOD3(set_entry, void
	       (std::initializer_list<int>& in,
		std::initializer_list<int>& out, std::complex<double> val));
  MOCK_METHOD3(set_input, void(int n, Tensor *g, int m));
  MOCK_METHOD3(set_output, void(int n, Tensor *g, int m));
  MOCK_METHOD1(input_tensor, Tensor*(int n));
  MOCK_METHOD1(output_tensor, Tensor*(int n));
  MOCK_METHOD1(input_num, int(int n));
  MOCK_METHOD1(output_num, int(int n));
};

#endif // _MOCK_TENSOR_HH
