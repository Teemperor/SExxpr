#include "TestUtils.h"

TEST(TestParseLists, Empty) { EXPECT_EQ(parse("()"), Expr::List({})); }

TEST(TestParseStrings, Basic) {
  EXPECT_EQ(parse("(a)"), Expr::List({Expr::Symbol("a")}));
  EXPECT_EQ(parse("(a b)"), Expr::List({Expr::Symbol("a"), Expr::Symbol("b")}));
}

TEST(TestParseStrings, Whitespace) {
  EXPECT_EQ(parse("( a )"), Expr::List({Expr::Symbol("a")}));
  EXPECT_EQ(parse("(a  b)"),
            Expr::List({Expr::Symbol("a"), Expr::Symbol("b")}));
  EXPECT_EQ(parse("(a\tb)"),
            Expr::List({Expr::Symbol("a"), Expr::Symbol("b")}));
  EXPECT_EQ(parse("(a\nb)"),
            Expr::List({Expr::Symbol("a"), Expr::Symbol("b")}));
  EXPECT_EQ(parse("(a\rb)"),
            Expr::List({Expr::Symbol("a"), Expr::Symbol("b")}));
  EXPECT_EQ(parse("(a\t b)"),
            Expr::List({Expr::Symbol("a"), Expr::Symbol("b")}));
  EXPECT_EQ(parse("(\ta\t b\t)\t"),
            Expr::List({Expr::Symbol("a"), Expr::Symbol("b")}));
}
