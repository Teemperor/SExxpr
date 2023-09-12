#ifndef SEXXPR_TEST_UTILS
#define SEXXPR_TEST_UTILS

#include "SExxpr.hpp"
#include "gtest/gtest.h"

using namespace SExxpr;

namespace SExxpr {
void PrintTo(const Expr &e, std::ostream *osPtr) {
  e.dump<std::ostream>(*osPtr);
}
} // namespace SExxpr

inline Expr parse(std::string str) {
  std::istringstream iss(str);
  Parser parser(iss);
  auto result = parser.parseTopLevel();
  if (result.isError()) {
    ADD_FAILURE() << "Unexpected error: " << result.getError().format() << "\n";
    return Expr::Symbol("Invalid");
  }
  return result.getExpr();
}

inline std::string print(Expr e) {
  std::ostringstream ss;
  e.dump(ss);
  return ss.str();
}

#endif
