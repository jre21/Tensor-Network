#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "mock_tensor.hh"
#include "../tensor.hh"

class TensorTest : public ::testing::Test {
protected:
  virtual void SetUp()
  {
    t1 = new Tensor(2, 3, 6, 6);
    t2 = new MockTensor();
  }

  virtual void TearDown()
  {
    delete t1;
    delete t2;
  }
  Tensor *t1;
  MockTensor *t2;
};

TEST(TensorTest,Succeed) {
  SUCCEED();
}

TEST(TensorTest,Fail) {
  FAIL();
}

