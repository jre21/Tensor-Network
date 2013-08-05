#ifndef MOCK_TENSOR_HH_
#define MOCK_TENSOR_HH_

#include <gmock/gmock.h>
#include "../tensor.hh"

class MockTensor : public Tensor
{
public:
  MockTensor();
  virtual ~MockTensor();
  MOCK_METHOD0(input_rank, size_t());
  MOCK_METHOD0(output_rank, size_t());
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
  MOCK_METHOD3(set_input, void(size_t n, Tensor *T, size_t m));
  MOCK_METHOD3(set_output, void(size_t n, Tensor *T, size_t m));
  MOCK_METHOD1(input_tensor, Tensor*(size_t n));
  MOCK_METHOD1(output_tensor, Tensor*(size_t n));
  MOCK_METHOD1(input_num, size_t(size_t n));
  MOCK_METHOD1(output_num, size_t(size_t n));
  MOCK_METHOD1(matrix, Matrix(bool conjugate));
  MOCK_METHOD3(_set_input_self, void(size_t n, Tensor *T, size_t m));
  MOCK_METHOD3(_set_output_self, void(size_t n, Tensor *T, size_t m));
};

#endif // MOCK_TENSOR_HH_
