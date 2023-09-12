#include "TestUtils.h"

TEST(TestParseFloats, Positive) {
  EXPECT_EQ(parse(R"(1.0)"), Expr::Float(1));
  EXPECT_EQ(parse(R"(2.0)"), Expr::Float(2));
  EXPECT_EQ(parse(R"(0.5)"), Expr::Float(0.5));
  EXPECT_EQ(parse(R"(0.)"), Expr::Float(0));
  EXPECT_EQ(parse(R"(0.0)"), Expr::Float(0));
}

TEST(TestParseFloats, Negative) {
  EXPECT_EQ(parse(R"(-1.0)"), Expr::Float(-1));
  EXPECT_EQ(parse(R"(-2.0)"), Expr::Float(-2));
  EXPECT_EQ(parse(R"(-0.5)"), Expr::Float(-0.5));
  EXPECT_EQ(parse(R"(-0.)"), Expr::Float(0));
  EXPECT_EQ(parse(R"(-0.0)"), Expr::Float(0));
}
