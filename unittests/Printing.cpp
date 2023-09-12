#include "TestUtils.h"

TEST(TestPrinting, Floats) {
  EXPECT_EQ(print(Expr::Float(2.0)), "2.");
  EXPECT_EQ(print(Expr::Float(0)), "0.");
}

TEST(TestPrinting, Integers) {
  EXPECT_EQ(print(Expr::Integer(2)), "2");
  EXPECT_EQ(print(Expr::Integer(-1)), "-1");
  EXPECT_EQ(print(Expr::Integer(0)), "0");
}

TEST(TestPrinting, Strings) {
  EXPECT_EQ(print(Expr::String("")), "\"\"");
  EXPECT_EQ(print(Expr::String("a")), "\"a\"");
  EXPECT_EQ(print(Expr::String("\"")), "\"\\\"\"");
}

TEST(TestPrinting, Symbols) {
  EXPECT_EQ(print(Expr::Symbol("")), "");
  EXPECT_EQ(print(Expr::Symbol("a")), "a");
  EXPECT_EQ(print(Expr::Symbol("x\"")), "x\"");
}
