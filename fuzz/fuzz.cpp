#include <cstdint>

#include "SExxpr.hpp"

#include <iostream>
#include <sstream>
#include <string>

/// Compares two expressions, but allows minor differences in float values.
static bool isEqual(const SExxpr::Expr &lhs, const SExxpr::Expr &rhs) {
  if (lhs.getKind() != rhs.getKind())
    return false;

  switch (lhs.getKind()) {
  case SExxpr::Expr::Kind::Float: {
    float diff = lhs.getFloatValue() - rhs.getFloatValue();
    // FIXME: This could be done more reliable with float steps, but this is
    // good enough for fuzzing.
    return std::abs(diff) < 0.01;
  }
  case SExxpr::Expr::Kind::List: {
    if (lhs.size() != rhs.size())
      return false;
    for (std::size_t i = 0; i < lhs.size(); ++i) {
      if (!isEqual(lhs.at(i), rhs.at(i)))
        return false;
    }
    return true;
  }
  case SExxpr::Expr::Kind::Integer:
  case SExxpr::Expr::Kind::String:
  case SExxpr::Expr::Kind::Symbol:
    return lhs == rhs;
  }
}

/// libfuzzer API entry.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, std::size_t Size) {
  const std::string str(reinterpret_cast<const char *>(Data), Size);
  std::istringstream originalStream(str);

  // Parse the original input.
  SExxpr::Parser parser(originalStream);
  auto result = parser.parseTopLevel();
  if (result.isError())
    return 0;

  const SExxpr::Expr firstExpr = result.getExpr();

  // Dump the parsed expression back to a string.
  std::ostringstream outstream;
  firstExpr.dump(outstream);
  const std::string dumped = outstream.str();
  std::istringstream dumpedStream(dumped);

  // Parse back the dumped expression.
  SExxpr::Parser secondParser(dumpedStream);
  auto secondParsed = secondParser.parseTopLevel();
  if (secondParsed.isError()) {
    std::cerr << "failed to parse dumped output: '" << dumped << "'\n";
    abort();
  }

  // The parsing of the dump should be equal to what we originally parsed.
  // Otherwise either the parsing or dumping is broken.
  const SExxpr::Expr secondExpr = secondParsed.getExpr();
  if (isEqual(secondExpr, firstExpr))
    return 0;

  std::cerr << "parsed different output second time.\nFirst:\n" << dumped;
  std::cerr << "\nSecond:\n";
  secondParsed.getExpr().dump(std::cerr);
  std::cerr << "\n";
  abort();
}
