#include "TestUtils.h"

TEST(TestParseSymbols, Basic) {
  EXPECT_EQ(parse(R"(a)"), Expr::Symbol("a"));
  EXPECT_EQ(parse(R"(ab)"), Expr::Symbol("ab"));
}

TEST(TestParseSymbols, WithWeirdChars) {
  EXPECT_EQ(parse(R"(a")"), Expr::Symbol("a\""));
}
