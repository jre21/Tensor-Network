#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "mock_tensor.hh"
#include "../tensor.hh"

using std::vector;
using std::complex;

class TensorTest : public ::testing::Test {
protected:
  virtual void SetUp()
  {
    T1 = new Tensor(2, 3, 6, 6);
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
  FAIL();
}

TEST_F(TensorDeathTest,Linking) {
  FAIL();
}

// test making copies of a tensor
TEST_F(TensorTest,Copying) {
  FAIL();
}

TEST_F(TensorDeathTest,Copying) {
  FAIL();
}
