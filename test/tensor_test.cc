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
#include "../tensor.hh"
#include "../utils.hh"
#include "mock_matrix.hh"
#include "mock_tensor.hh"
#include "utils_test.hh"

using std::complex;
using std::shared_ptr;
using std::vector;

using ::testing::AtLeast;
using ::testing::Mock;
using ::testing::Return;
using ::testing::_;

class TensorTest : public ::testing::Test {
protected:
  virtual void SetUp()
  {
    t = new ConcreteTensor(2, 3, 6, 6);
  }

  virtual void TearDown()
  {
    delete t;
  }
  Tensor *t;
  MockTensor mock;
};
typedef TensorTest TensorDeathTest;

// test setting and retrieving entries
TEST_F(TensorTest,EntryInitialState) {
  // test initialized values; the tensor should be nearly an identity
  // matrix
  TN_EXPECT_COMPLEX_EQ(0, t->entry( {0,2}, {0,0,3} ) );
  TN_EXPECT_COMPLEX_EQ(1, t->entry( {0,2}, {0,0,2} ) );
  TN_EXPECT_COMPLEX_EQ(1, t->entry( vector<size_t>{0,2},
				 vector<size_t>{0,0,2} ) );
  TN_EXPECT_COMPLEX_EQ(0, t->entry( {5,5}, {5,5,5} ) );
}

TEST_F(TensorTest,EntryManipulations) {
  complex<double> c1{2.4, 2.6}, c2{3.14, 2.718};
  // test setting and retrieving values, exercizing both method signatures
  t->set_entry( vector<size_t>{3,5}, vector<size_t>{2,0,4}, c1 );
  TN_EXPECT_COMPLEX_EQ(c1, t->entry( vector<size_t>{3,5},
				     vector<size_t>{2,0,4} ));

  t->set_entry( {4,1}, {3,5,2}, c2 );
  TN_EXPECT_COMPLEX_EQ(c2, t->entry( {4,1}, {3,5,2} ));
}

TEST_F(TensorDeathTest,Entry) {
  // crash when fetching entry with wrong number of arguments
  EXPECT_DEATH( t->entry( {3,5}, {2,0} ), "");
  EXPECT_DEATH( t->entry( vector<size_t>{3}, vector<size_t>{2,0,4} ), "");
  // crash when fetching entry with argument out of range
  EXPECT_DEATH( t->entry( {3,5}, {2,0,6} ), "");
  EXPECT_DEATH( t->entry( vector<size_t>{3,6},
			   vector<size_t>{2,0,4} ), "");

  // same tests, but when setting entry
  EXPECT_DEATH( t->set_entry( {3,5}, {2,0}, complex<double>{} ), "");
  EXPECT_DEATH( t->set_entry( vector<size_t>{3}, vector<size_t>{2,0,4},
			      complex<double>{} ), "");
  EXPECT_DEATH( t->set_entry( {3,5}, {2,0,6}, complex<double>{} ), "");
  EXPECT_DEATH( t->set_entry( vector<size_t>{3,6},
			      vector<size_t>{2,0,4}, complex<double>{} ), "");
}

// ensure consistency for vectors (tensors with either no inputs or no
// outputs)
TEST_F(TensorTest,Vector) {
  complex<double> c = complex<double> {2.3, 1.5};
  Tensor *vec = new ConcreteTensor {2, 0, 6, 0};
  vec->set_entry( {1,0}, {}, c );
  TN_EXPECT_COMPLEX_EQ( c, vec->entry( {1,0}, {} ));

  delete vec;
}

TEST_F(TensorDeathTest,Vector) {
  Tensor *vec = new ConcreteTensor {2, 0, 6, 0};
  EXPECT_DEATH(vec->set_entry( {0,0}, {0}, complex<double>{} ), "");
  EXPECT_DEATH(vec->entry( {0,0}, {0} ), "");
}

// test linking tensors together
TEST_F(TensorTest,Linking) {
  // verify input link is set correctly
  EXPECT_CALL(mock, _set_output_self(2,t,1));
  EXPECT_CALL(mock, output_rank())
    .WillOnce(Return(6));
  t->set_input(1,&mock,2);
  Mock::VerifyAndClear(&mock);
  EXPECT_EQ(&mock, t->input_tensor(1));
  EXPECT_EQ(2, t->input_num(1));

  // verify result of unset input link
  EXPECT_EQ(nullptr, t->input_tensor(0));
  EXPECT_EQ(0, t->input_num(0));

  // verify unsetting link works correctly
  EXPECT_CALL(mock, _set_output_self(2,nullptr,0));
  t->set_input(1,nullptr,2);
  Mock::VerifyAndClear(&mock);
  EXPECT_EQ(nullptr, t->input_tensor(1));
  EXPECT_EQ(0, t->input_num(1));

  // verify output link is set correctly
  EXPECT_CALL(mock, _set_input_self(0, t, 1));
  EXPECT_CALL(mock, input_rank())
    .WillOnce(Return(6));
  t->set_output(1,&mock,0);
  Mock::VerifyAndClear(&mock);

  // verify result of unset output link
  EXPECT_EQ(nullptr, t->output_tensor(2));
  EXPECT_EQ(0, t->output_tensor(2));

  // verify unsetting link works correctly
  EXPECT_CALL(mock, _set_input_self(0,nullptr,0));
  t->set_output(1,nullptr,0);
  Mock::VerifyAndClear(&mock);
  EXPECT_EQ(nullptr, t->output_tensor(1));
  EXPECT_EQ(0, t->input_num(1));

  // verify setting links from the tensor to itself
  t->set_input(0,t,1);
  EXPECT_EQ(t, t->input_tensor(0));
  EXPECT_EQ(t, t->output_tensor(1));
  EXPECT_EQ(1, t->input_num(0));
  EXPECT_EQ(0, t->output_num(1));
}


TEST_F(TensorDeathTest,Linking) {
  // crash when exceeding list length
  EXPECT_DEATH(t->input_num(2), "");
  EXPECT_DEATH(t->output_num(3), "");
  EXPECT_DEATH(t->set_input(2,t,0), "");
  EXPECT_DEATH(t->set_input(0,t,3), "");
  EXPECT_DEATH(t->set_output(3,t,0), "");
  EXPECT_DEATH(t->set_output(0,t,2), "");

  // crash when linking to a tensor with different vector space ranks
  EXPECT_CALL(mock, input_rank())
    .Times(0);
  EXPECT_CALL(mock, output_rank())
    .Times(AtLeast(0)).WillRepeatedly(Return(5));
  EXPECT_DEATH(t->set_input(0,&mock,0), "");

  // same, but linking on output rather than input
  EXPECT_CALL(mock, input_rank())
    .Times(AtLeast(0)).WillRepeatedly(Return(5));
  EXPECT_CALL(mock, output_rank())
    .Times(0);
  EXPECT_DEATH(t->set_output(0,&mock,0), "");
}

// test making copies of a tensor
TEST_F(TensorTest,Copying) {
  // some complex numbers to use
  complex<double> c1{2.7,5.3}, c2{8.2,1.6}, c3{1.8,7.3};

  // set a few entries in t
  t->set_entry( {2,4}, {1,5,3}, c1 );
  t->set_entry( {0,3}, {5,2,4}, c2 );

  // create a copy of t
  Tensor *t0 = new ConcreteTensor{t->matrix()};
  // ensure entries exist in both copies
  TN_EXPECT_COMPLEX_EQ(c1, t->entry( {2,4}, {1,5,3} ));
  TN_EXPECT_COMPLEX_EQ(c1, t0->entry( {2,4}, {1,5,3} ));

  // set entries in one tensor and ensure the other updates
  t->set_entry( {0,3}, {5,2,4}, c2 );
  t0->set_entry( {4,3}, {2,2,5}, c3 );
  TN_EXPECT_COMPLEX_EQ(c2, t->entry( {0,3}, {5,2,4} ));
  TN_EXPECT_COMPLEX_EQ(c2, t0->entry( {0,3}, {5,2,4} ));
  TN_EXPECT_COMPLEX_EQ(c3, t->entry( {4,3}, {2,2,5} ));
  TN_EXPECT_COMPLEX_EQ(c3, t0->entry( {4,3}, {2,2,5} ));
  delete t0;
}

TEST_F(TensorTest,Conjugate) {
  complex<double> c1{2.7,5.3}, c2{5.2,9.0}, c3{3.7,4.2};

  t->set_entry( {2,4}, {1,5,3}, c1 );

  // create a Hermitian conjugate copy of t
  Tensor *tc = new ConcreteTensor{t->matrix(true)};
  // ensure entries exist in both copies
  TN_EXPECT_COMPLEX_EQ(c1, t->entry( {2,4}, {1,5,3} ));
  TN_EXPECT_COMPLEX_EQ(conjugate(c1), tc->entry( {1,5,3}, {2,4} ));

  // set entries in one tensor and ensure the other updates
  t->set_entry( {2,0}, {1,0,3}, c2 );
  tc->set_entry( {2,1,5}, {3,1}, c3 );
  TN_EXPECT_COMPLEX_EQ(c2, t->entry( {2,0}, {1,0,3} ));
  TN_EXPECT_COMPLEX_EQ(conjugate(c2), tc->entry( {1,0,3}, {2,0} ));
  TN_EXPECT_COMPLEX_EQ(conjugate(c3), t->entry( {3,1}, {2,1,5} ));
  TN_EXPECT_COMPLEX_EQ(c3, tc->entry( {2,1,5}, {3,1} ));

  // create a double-conjugate copy and ensure it is equivalent to the
  // original
  Tensor *tcc = new ConcreteTensor{tc->matrix(true)};
  TN_EXPECT_COMPLEX_EQ(t->entry( {3,1}, {2,1,5} ),
		       tcc->entry( {3,1}, {2,1,5} ));

  delete tc;
  delete tcc;
}

// When multiple tensors share a matrix, ensure the matrix is retained
// until all tensors are deleted.
TEST_F(TensorTest,MatrixDeletion) {
  // inject a mock matrix into the struct used to construct new
  // tensors
  MockMatrix *mock = new MockMatrix;
  EXPECT_CALL(*mock, die())
    .Times(0);
  MatrixStruct mock_struct = t->matrix();
  mock_struct.matrix = shared_ptr<Matrix> {mock};

  // generate a few tensors and check for deletion at appropriate time
  Tensor *t1 = new ConcreteTensor(mock_struct);
  Tensor *t2 = new ConcreteTensor(mock_struct);
  mock_struct.matrix.reset();

  delete t1;
  EXPECT_CALL(*mock, die());
  delete t2;

  // test again with deletion in reverse order
  mock = new MockMatrix;
  EXPECT_CALL(*mock, die())
    .Times(0);
  mock_struct.matrix.reset(mock);
  t1 = new ConcreteTensor(mock_struct);
  t2 = new ConcreteTensor(mock_struct);

  delete t2;
  EXPECT_CALL(*mock, die());
  delete t1;
}
