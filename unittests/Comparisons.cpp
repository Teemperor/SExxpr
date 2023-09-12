#include "TestUtils.h"

TEST(TestComparisons, Symbols) {
  EXPECT_EQ(Expr::Symbol("a"), Expr::Symbol("a"));
  EXPECT_NE(Expr::Symbol("a"), Expr::Symbol("b"));
}

TEST(TestComparisons, Strings) {
  EXPECT_EQ(Expr::String("a"), Expr::String("a"));
  EXPECT_EQ(Expr::String(""), Expr::String(""));
  EXPECT_NE(Expr::String("a"), Expr::String("b"));
  EXPECT_NE(Expr::String(""), Expr::String("b"));
}
TEST(TestComparisons, Integers) {
  EXPECT_EQ(Expr::Integer(1), Expr::Integer(1));
  EXPECT_EQ(Expr::Integer(0), Expr::Integer(0));
  EXPECT_NE(Expr::Integer(-1), Expr::Integer(1));
  EXPECT_NE(Expr::Integer(-1), Expr::Integer(0));
  EXPECT_NE(Expr::Integer(1), Expr::Integer(0));
}
