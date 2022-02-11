
#include <glog/logging.h>
#include <gtest/gtest.h>

class SimpleTests : public ::testing::Test {
 protected:

  void SetUp() override {
  }
  void TearDown() override {
  }
};

TEST_F(SimpleTests, test) {
  ASSERT(true);
}
