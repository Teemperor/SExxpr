#include "TestUtils.h"

TEST(TestParseStrings, Empty) { EXPECT_EQ(parse(R"("")"), Expr::String("")); }

TEST(TestParseStrings, Basic) {
  EXPECT_EQ(parse(R"("a")"), Expr::String("a"));
  EXPECT_EQ(parse(R"("ab")"), Expr::String("ab"));
}

TEST(TestParseStrings, Escaped) {
  EXPECT_EQ(parse(R"("\\")"), Expr::String("\\"));
  EXPECT_EQ(parse(R"("\"")"), Expr::String("\""));
  EXPECT_EQ(parse(R"("\a")"), Expr::String("a"));
}
