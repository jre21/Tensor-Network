#ifndef MOCK_TENSOR_HH_
#define MOCK_TENSOR_HH_

#include <gmock/gmock.h>
#include "../tensor.hh"

class MockTensor : public Tensor
{
public:
  MockTensor() : Tensor(0,0,0,0) {}
  MOCK_METHOD2(entry, std::complex<double>
	       (const std::vector<size_t>& in,
		const std::vector<size_t>& out));
  MOCK_METHOD2(entry, std::complex<double>
	       (std::initializer_list<size_t>& in,
		std::initializer_list<size_t>& out));
  MOCK_METHOD3(set_entry, void
	       (const std::vector<size_t>& in,
		const std::vector<size_t>& out,
		std::complex<double> val));
  MOCK_METHOD3(set_entry, void
	       (std::initializer_list<size_t>& in,
		std::initializer_list<size_t>& out,
		std::complex<double> val));
  MOCK_METHOD3(set_input, void(size_t n, Tensor *g, size_t m));
  MOCK_METHOD3(set_output, void(size_t n, Tensor *g, size_t m));
  MOCK_METHOD1(input_tensor, Tensor*(size_t n));
  MOCK_METHOD1(output_tensor, Tensor*(size_t n));
  MOCK_METHOD1(input_num, size_t(size_t n));
  MOCK_METHOD1(output_num, size_t(size_t n));
};

#endif // MOCK_TENSOR_HH_
