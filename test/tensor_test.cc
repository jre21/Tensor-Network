#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "mock_tensor.hh"
#include "../tensor.hh"

using std::vector;
using std::complex;

using ::testing::Return;

class TensorTest : public ::testing::Test {
protected:
  virtual void SetUp()
  {
    T1 = new ConcreteTensor(2, 3, 6, 6);
    T2 = new MockTensor();
  }

  virtual void TearDown()
  {
    delete T1;
    delete T2;
  }
  Tensor *T1;
  MockTensor *T2;
};
typedef TensorTest TensorDeathTest;

// test setting and retrieving entries
TEST_F(TensorTest,Entry) {
  // test initialized values; the tensor should be nearly an identity
  // matrix
  EXPECT_DOUBLE_EQ(0, T1->entry( {0,2}, {0,0,3} ).real() );
  EXPECT_DOUBLE_EQ(0, T1->entry( vector<size_t>({1,2}),
				 vector<size_t>({1,0,3}) ).imag() );
  EXPECT_DOUBLE_EQ(1, T1->entry( {0,2}, {0,0,2} ).real() );
  EXPECT_DOUBLE_EQ(0, T1->entry( {0,2}, {0,0,2} ).imag() );
  EXPECT_DOUBLE_EQ(1, T1->entry( vector<size_t>({0,2}),
				 vector<size_t>({0,0,2}) ).real() );
  EXPECT_DOUBLE_EQ(0, T1->entry( vector<size_t>({0,2}),
				 vector<size_t>({0,0,2}) ).imag() );

  // test setting and retrieving values, exercizing both methods
  T1->set_entry( vector<size_t>({3,5}), vector<size_t>({2,0,4}),
		 complex<double>(2.4,3.6) );
  EXPECT_DOUBLE_EQ(2.4,T1->entry( vector<size_t>({3,5}),
				  vector<size_t>({2,0,4})
				  ).real() );
  EXPECT_DOUBLE_EQ(3.6, T1->entry( {3,5}, {2,0,4} ).imag() );

  T1->set_entry( {4,1}, {3,5,2}, complex<double>(3.14,2.718) );
  EXPECT_DOUBLE_EQ(3.14, T1->entry( {4,1}, {3,5,2} ).real() );
  EXPECT_DOUBLE_EQ(2.718, T1->entry( {4,1}, {3,5,2} ).imag() );
}

TEST_F(TensorDeathTest,Entry) {
  // crash when fetching entry with wrong number of arguments
  EXPECT_DEATH( T1->entry( {3,5}, {2,0} ), "");
  EXPECT_DEATH( T1->entry( vector<size_t>({3}), vector<size_t>({2,0,4}) ), "");
  // crash when fetching entry with argument out of range
  EXPECT_DEATH( T1->entry( {3,5}, {2,0,6} ), "");
  EXPECT_DEATH( T1->entry( vector<size_t>({3,6}),
			   vector<size_t>({2,0,4}) ), "");

  // same tests, but when setting entry
  EXPECT_DEATH( T1->set_entry( {3,5}, {2,0}, complex<double>() ), "");
  EXPECT_DEATH( T1->set_entry( vector<size_t>({3}), vector<size_t>({2,0,4}),
			       complex<double>() ), "");
  EXPECT_DEATH( T1->set_entry( {3,5}, {2,0,6}, complex<double>() ), "");
  EXPECT_DEATH( T1->set_entry( vector<size_t>({3,6}),
			   vector<size_t>({2,0,4}), complex<double>() ), "");
}

// test linking tensors together
TEST_F(TensorTest,Linking) {
  // verify input link is set correctly
  EXPECT_CALL(*T2, _set_output_self(2,T1,1))
    .Times(1);
  EXPECT_CALL(*T2, output_rank())
    .WillOnce(Return(6));
  T1->set_input(1,T2,2);
  ::testing::Mock::VerifyAndClear(T2);
  EXPECT_EQ(T2, T1->input_tensor(1));
  EXPECT_EQ(2, T1->input_num(1));

  // verify result of unset input link
  EXPECT_EQ(nullptr, T1->input_tensor(0));
  EXPECT_EQ(0, T1->input_num(0));

  // verify unsetting link works correctly
  EXPECT_CALL(*T2, _set_output_self(2,nullptr,0))
    .Times(1);
  T1->set_input(1,nullptr,2);
  ::testing::Mock::VerifyAndClear(T2);
  EXPECT_EQ(nullptr, T1->input_tensor(1));
  EXPECT_EQ(0, T1->input_num(1));

  // verify output link is set correctly
  EXPECT_CALL(*T2, _set_input_self(0, T1, 1))
    .Times(1);
  EXPECT_CALL(*T2, input_rank())
    .WillOnce(Return(6));
  T1->set_output(1,T2,0);
  ::testing::Mock::VerifyAndClear(T2);

  // verify result of unset output link
  EXPECT_EQ(nullptr, T1->output_tensor(2));
  EXPECT_EQ(0, T1->output_tensor(2));

  // verify unsetting link works correctly
  EXPECT_CALL(*T2, _set_input_self(0,nullptr,0))
    .Times(1);
  T1->set_output(1,nullptr,0);
  ::testing::Mock::VerifyAndClear(T2);
  EXPECT_EQ(nullptr, T1->output_tensor(1));
  EXPECT_EQ(0, T1->input_num(1));

  // verify setting links from the tensor to itself
  T1->set_input(0,T1,1);
  EXPECT_EQ(T1, T1->input_tensor(0));
  EXPECT_EQ(T1, T1->output_tensor(1));
  EXPECT_EQ(1, T1->input_num(0));
  EXPECT_EQ(0, T1->output_num(1));
}

TEST_F(TensorDeathTest,Linking) {
  // crash when exceeding list length
  EXPECT_DEATH(T1->input_num(2), "");
  EXPECT_DEATH(T1->output_num(3), "");
  EXPECT_DEATH(T1->set_input(2,T1,0), "");
  EXPECT_DEATH(T1->set_input(0,T1,3), "");
  EXPECT_DEATH(T1->set_output(3,T1,0), "");
  EXPECT_DEATH(T1->set_output(0,T1,2), "");

  // crash when linking to a tensor with different vector space ranks
  ON_CALL(*T2, input_rank())
    .WillByDefault(Return(6));
  ON_CALL(*T2, output_rank())
    .WillByDefault(Return(5));
  EXPECT_DEATH(T1->set_input(0,T2,0), "");

  ON_CALL(*T2, input_rank())
    .WillByDefault(Return(5));
  ON_CALL(*T2, output_rank())
    .WillByDefault(Return(6));
  EXPECT_DEATH(T1->set_output(0,T2,0), "");
}

// test making copies of a tensor
TEST_F(TensorTest,Copying) {
  FAIL();
}

TEST_F(TensorDeathTest,Copying) {
  FAIL();
}
