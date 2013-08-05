#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "mock_tensor.hh"
#include "../tensor.hh"

using std::vector;
using std::complex;

using ::testing::AtLeast;
using ::testing::Mock;
using ::testing::Return;
using ::testing::_;

class TensorTest : public ::testing::Test {
protected:
  virtual void SetUp()
  {
    T1 = new ConcreteTensor(2, 3, 6, 6);
  }

  virtual void TearDown()
  {
    delete T1;
  }
  Tensor *T1;
  MockTensor mock;
};
typedef TensorTest TensorDeathTest;

// test setting and retrieving entries
TEST_F(TensorTest,EntryInitialState) {
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
}

TEST_F(TensorTest,EntryManipulations) {
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

// for debugging purposes
TEST_F(TensorTest,IsolateCrash) {
  EXPECT_CALL(mock, _set_output_self(_,nullptr,_))
    .Times(AtLeast(0));
  EXPECT_CALL(mock, _set_output_self(_,T1,_))
    .Times(AtLeast(0));
  EXPECT_CALL(mock, output_rank())
    .WillRepeatedly(Return(6));
  printf("crash marker 1\n");
  mock._set_output_self(0,nullptr,0);
  printf("crash marker 2\n");
  mock._set_output_self(0,T1,0);
  printf("crash marker 3\n");
  mock.set_input(1,&mock,2);
  printf("crash marker 4\n");
}


// test linking tensors together
TEST_F(TensorTest,Linking) {
  // verify input link is set correctly
  EXPECT_CALL(mock, _set_output_self(2,T1,1));
  EXPECT_CALL(mock, output_rank())
    .WillOnce(Return(6));
  T1->set_input(1,&mock,2);
  Mock::VerifyAndClear(&mock);
  EXPECT_EQ(&mock, T1->input_tensor(1));
  EXPECT_EQ(2, T1->input_num(1));

  // verify result of unset input link
  EXPECT_EQ(nullptr, T1->input_tensor(0));
  EXPECT_EQ(0, T1->input_num(0));

  // verify unsetting link works correctly
  EXPECT_CALL(mock, _set_output_self(2,nullptr,0));
  T1->set_input(1,nullptr,2);
  Mock::VerifyAndClear(&mock);
  EXPECT_EQ(nullptr, T1->input_tensor(1));
  EXPECT_EQ(0, T1->input_num(1));

  // verify output link is set correctly
  EXPECT_CALL(mock, _set_input_self(0, T1, 1));
  EXPECT_CALL(mock, input_rank())
    .WillOnce(Return(6));
  T1->set_output(1,&mock,0);
  Mock::VerifyAndClear(&mock);

  // verify result of unset output link
  EXPECT_EQ(nullptr, T1->output_tensor(2));
  EXPECT_EQ(0, T1->output_tensor(2));

  // verify unsetting link works correctly
  EXPECT_CALL(mock, _set_input_self(0,nullptr,0));
  T1->set_output(1,nullptr,0);
  Mock::VerifyAndClear(&mock);
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
  EXPECT_CALL(mock, input_rank())
    .Times(0);
  EXPECT_CALL(mock, output_rank())
    .Times(AtLeast(0)).WillOnce(Return(5));
  EXPECT_DEATH(T1->set_input(0,&mock,0), "");

  EXPECT_CALL(mock, input_rank())
    .Times(AtLeast(0)).WillOnce(Return(5));
  EXPECT_CALL(mock, output_rank())
    .Times(0);
  EXPECT_DEATH(T1->set_output(0,&mock,0), "");
}

// test making copies of a tensor
TEST_F(TensorTest,Copying) {
  FAIL();
}

TEST_F(TensorDeathTest,Copying) {
  FAIL();
}
