#include "TestUtils.h"

TEST(TestParseIntegers, Decimal) {
  EXPECT_EQ(parse(R"(1)"), Expr::Integer(1));
  EXPECT_EQ(parse(R"(2)"), Expr::Integer(2));
  EXPECT_EQ(parse(R"(-1)"), Expr::Integer(-1));
  EXPECT_EQ(parse(R"(-2)"), Expr::Integer(-2));
}

TEST(TestParseIntegers, Hex) {
  EXPECT_EQ(parse(R"(0x1)"), Expr::Integer(1));
  EXPECT_EQ(parse(R"(0xA)"), Expr::Integer(0xA));
  EXPECT_EQ(parse(R"(-0x1)"), Expr::Integer(-1));
  EXPECT_EQ(parse(R"(-0xA)"), Expr::Integer(-0xA));
}
