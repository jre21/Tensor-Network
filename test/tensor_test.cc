// Copyright 2013 Jacob Emmert-Aronson
// This file is part of Tensor Network.
//
// Tensor Network is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// Tensor Network is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tensor Network.  If not, see
// <http://www.gnu.org/licenses/>.

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "mock_tensor.hh"
#include "../tensor.hh"
#include "../utils.hh"
#include "utils_test.hh"

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
    T = new ConcreteTensor(2, 3, 6, 6);
  }

  virtual void TearDown()
  {
    delete T;
  }
  Tensor *T;
  MockTensor mock;
};
typedef TensorTest TensorDeathTest;

// test setting and retrieving entries
TEST_F(TensorTest,EntryInitialState) {
  // test initialized values; the tensor should be nearly an identity
  // matrix
  TN_EXPECT_COMPLEX_EQ(0, T->entry( {0,2}, {0,0,3} ) );
  TN_EXPECT_COMPLEX_EQ(1, T->entry( {0,2}, {0,0,2} ) );
  TN_EXPECT_COMPLEX_EQ(1, T->entry( vector<size_t>{0,2},
				 vector<size_t>{0,0,2} ) );
  TN_EXPECT_COMPLEX_EQ(0, T->entry( {5,5}, {5,5,5} ) );
}

TEST_F(TensorTest,EntryManipulations) {
  complex<double> c1{2.4, 2.6}, c2{3.14, 2.718};
  // test setting and retrieving values, exercizing both method signatures
  T->set_entry( vector<size_t>{3,5}, vector<size_t>{2,0,4}, c1 );
  TN_EXPECT_COMPLEX_EQ(c1, T->entry( vector<size_t>{3,5},
				     vector<size_t>{2,0,4} ));

  T->set_entry( {4,1}, {3,5,2}, c2 );
  TN_EXPECT_COMPLEX_EQ(c2, T->entry( {4,1}, {3,5,2} ) );
}

TEST_F(TensorDeathTest,Entry) {
  // crash when fetching entry with wrong number of arguments
  EXPECT_DEATH( T->entry( {3,5}, {2,0} ), "");
  EXPECT_DEATH( T->entry( vector<size_t>{3}, vector<size_t>{2,0,4} ), "");
  // crash when fetching entry with argument out of range
  EXPECT_DEATH( T->entry( {3,5}, {2,0,6} ), "");
  EXPECT_DEATH( T->entry( vector<size_t>{3,6},
			   vector<size_t>{2,0,4} ), "");

  // same tests, but when setting entry
  EXPECT_DEATH( T->set_entry( {3,5}, {2,0}, complex<double>{} ), "");
  EXPECT_DEATH( T->set_entry( vector<size_t>{3}, vector<size_t>{2,0,4},
			      complex<double>{} ), "");
  EXPECT_DEATH( T->set_entry( {3,5}, {2,0,6}, complex<double>{} ), "");
  EXPECT_DEATH( T->set_entry( vector<size_t>{3,6},
			      vector<size_t>{2,0,4}, complex<double>{} ), "");
}


// test linking tensors together
TEST_F(TensorTest,Linking) {
  // verify input link is set correctly
  EXPECT_CALL(mock, _set_output_self(2,T,1));
  EXPECT_CALL(mock, output_rank())
    .WillOnce(Return(6));
  T->set_input(1,&mock,2);
  Mock::VerifyAndClear(&mock);
  EXPECT_EQ(&mock, T->input_tensor(1));
  EXPECT_EQ(2, T->input_num(1));

  // verify result of unset input link
  EXPECT_EQ(nullptr, T->input_tensor(0));
  EXPECT_EQ(0, T->input_num(0));

  // verify unsetting link works correctly
  EXPECT_CALL(mock, _set_output_self(2,nullptr,0));
  T->set_input(1,nullptr,2);
  Mock::VerifyAndClear(&mock);
  EXPECT_EQ(nullptr, T->input_tensor(1));
  EXPECT_EQ(0, T->input_num(1));

  // verify output link is set correctly
  EXPECT_CALL(mock, _set_input_self(0, T, 1));
  EXPECT_CALL(mock, input_rank())
    .WillOnce(Return(6));
  T->set_output(1,&mock,0);
  Mock::VerifyAndClear(&mock);

  // verify result of unset output link
  EXPECT_EQ(nullptr, T->output_tensor(2));
  EXPECT_EQ(0, T->output_tensor(2));

  // verify unsetting link works correctly
  EXPECT_CALL(mock, _set_input_self(0,nullptr,0));
  T->set_output(1,nullptr,0);
  Mock::VerifyAndClear(&mock);
  EXPECT_EQ(nullptr, T->output_tensor(1));
  EXPECT_EQ(0, T->input_num(1));

  // verify setting links from the tensor to itself
  T->set_input(0,T,1);
  EXPECT_EQ(T, T->input_tensor(0));
  EXPECT_EQ(T, T->output_tensor(1));
  EXPECT_EQ(1, T->input_num(0));
  EXPECT_EQ(0, T->output_num(1));
}


TEST_F(TensorDeathTest,Linking) {
  // crash when exceeding list length
  EXPECT_DEATH(T->input_num(2), "");
  EXPECT_DEATH(T->output_num(3), "");
  EXPECT_DEATH(T->set_input(2,T,0), "");
  EXPECT_DEATH(T->set_input(0,T,3), "");
  EXPECT_DEATH(T->set_output(3,T,0), "");
  EXPECT_DEATH(T->set_output(0,T,2), "");

  // crash when linking to a tensor with different vector space ranks
  EXPECT_CALL(mock, input_rank())
    .Times(0);
  EXPECT_CALL(mock, output_rank())
    .Times(AtLeast(0)).WillOnce(Return(5));
  EXPECT_DEATH(T->set_input(0,&mock,0), "");

  // same, but linking on output rather than input
  EXPECT_CALL(mock, input_rank())
    .Times(AtLeast(0)).WillOnce(Return(5));
  EXPECT_CALL(mock, output_rank())
    .Times(0);
  EXPECT_DEATH(T->set_output(0,&mock,0), "");
}

// test making copies of a tensor
TEST_F(TensorTest,Copying) {
  // some complex numbers to use
  complex<double> c1{2.7,5.3}, c2{8.2,1.6}, c3{1.8,7.3},
		  c4{5.2,9.0}, c5{3.7,4.2};

  // set a few entries in T
  T->set_entry( {2,4}, {1,5,3}, c1 );
  T->set_entry( {0,3}, {5,2,4}, c2 );

  // create a copy of T
  Tensor *T0 = new ConcreteTensor{T->matrix()};
  // ensure entries exist in both copies
  TN_EXPECT_COMPLEX_EQ(c1, T->entry( {2,4}, {1,5,3} ));
  TN_EXPECT_COMPLEX_EQ(c1, T0->entry( {2,4}, {1,5,3} ));

  // set entries in one tensor and ensure the other updates
  T->set_entry( {0,3}, {5,2,4}, c2 );
  T0->set_entry( {4,3}, {2,2,5}, c3 );
  TN_EXPECT_COMPLEX_EQ(c2, T->entry( {0,3}, {5,2,4} ));
  TN_EXPECT_COMPLEX_EQ(c2, T0->entry( {0,3}, {5,2,4} ));
  TN_EXPECT_COMPLEX_EQ(c3, T->entry( {4,3}, {2,2,5} ));
  TN_EXPECT_COMPLEX_EQ(c3, T0->entry( {4,3}, {2,2,5} ));

  // create a Hermitian conjugate copy of T
  Tensor *T1 = new ConcreteTensor{T->matrix(true)};
  // ensure entries exist in both copies
  TN_EXPECT_COMPLEX_EQ(c1, T->entry( {2,4}, {1,5,3} ));
  TN_EXPECT_COMPLEX_EQ(conjugate(c1), T1->entry( {2,4}, {1,5,3} ));

  // set entries in one tensor and ensure the other updates
  T->set_entry( {2,0}, {1,0,3}, c4 );
  T1->set_entry( {2,1,5}, {3,1}, c5 );
  TN_EXPECT_COMPLEX_EQ(c4, T->entry( {2,0}, {1,0,3} ));
  TN_EXPECT_COMPLEX_EQ(conjugate(c4), T1->entry( {1,0,3}, {2,0} ));
  TN_EXPECT_COMPLEX_EQ(conjugate(c5), T->entry( {3,1}, {1,1,3} ));
  TN_EXPECT_COMPLEX_EQ(c5, T1->entry( {1,1,5}, {3,1} ));

  delete T0;
  delete T1;
}
