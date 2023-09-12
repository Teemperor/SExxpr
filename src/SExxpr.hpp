#ifndef SEXXPR_H
#define SEXXPR_H

#include <cassert>
#include <cstdint>
#include <istream>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace SExxpr {

struct Expr {
  enum class Kind { Symbol, String, List, Integer, Float };

  typedef std::vector<Expr> ChildList;

  static Expr String(std::string content) {
    Expr result;
    result.kind = Kind::String;
    result.str = content;
    return result;
  }

  static Expr Symbol(std::string content) {
    Expr result;
    result.kind = Kind::Symbol;
    result.str = content;
    return result;
  }

  static Expr List(ChildList children) {
    Expr result;
    result.kind = Kind::List;
    result.children = children;
    return result;
  }

  static Expr Integer(std::int64_t value) {
    Expr result;
    result.kind = Kind::Integer;
    result.intValue = value;
    return result;
  }

  static Expr Float(double value) {
    Expr result;
    result.kind = Kind::Float;
    result.floatValue = value;
    return result;
  }

  Kind getKind() const { return kind; }

  auto begin() {
    assertIsKind(Kind::List);
    return children.begin();
  }
  auto end() {
    assertIsKind(Kind::List);
    return children.end();
  }
  auto begin() const {
    assertIsKind(Kind::List);
    return children.begin();
  }
  auto end() const {
    assertIsKind(Kind::List);
    return children.end();
  }

  const Expr &at(std::size_t index) const {
    assertIsKind(Kind::List);
    return children.at(index);
  }

  std::size_t size() const {
    assertIsKind(Kind::List);
    return children.size();
  }

  std::string getStringValue() const {
    assertIsKind(Kind::String);
    return str;
  }

  std::string getSymbolName() const {
    assertIsKind(Kind::Symbol);
    return str;
  }

  std::int64_t getIntValue() const {
    assertIsKind(Kind::Integer);
    return intValue;
  }

  double getFloatValue() const {
    assertIsKind(Kind::Float);
    return floatValue;
  }

  template <typename T> void dump(T &out) const {
    switch (getKind()) {
    case Kind::Float: {
      std::ostringstream ss;
      ss << getFloatValue();

      std::string buffer = ss.str();
      if (buffer.find('.') == std::string::npos)
        buffer += ".";
      out << buffer;
      break;
    }
    case Kind::Symbol:
      out << getSymbolName();
      break;
    case Kind::String:
      out << "\"";
      for (char c : getStringValue()) {
        switch (c) {
        case '"':
          out << "\\\"";
          break;
        case '\\':
          out << "\\\\";
          break;
        default:
          out << c;
        }
      }
      out << "\"";
      break;
    case Kind::List: {
      out << " ( ";
      for (const Expr &child : *this)
        child.dump(out);
      out << " ) ";
      break;
    }
    case Kind::Integer:
      out << getIntValue();
      break;
    }
  }

private:
  auto tie() const {
    return std::tie(kind, str, children, intValue, floatValue);
  }

public:
  bool operator==(const Expr &other) const { return tie() == other.tie(); }
  bool operator!=(const Expr &other) const { return tie() != other.tie(); }

private:
  void assertIsKind(Kind k) const {
    if (kind != k)
      throw std::bad_variant_access();
  }
  Kind kind = Kind::Symbol;
  std::string str;
  ChildList children;
  std::int64_t intValue = 0;
  double floatValue = 0;
};

struct Parser {
  struct Error {
    Error(std::string msg, std::size_t line, std::size_t column)
        : message(msg), line(line), column(column) {}

    std::string format() const {
      return std::to_string(line) + ":" + std::to_string(column) + ": " +
             message;
    }

  private:
    std::string message;
    std::size_t line;
    std::size_t column;
  };

  struct ParseResult {
    ParseResult(Expr e) : expr(e) {}
    ParseResult(Error e) : error(e) {}

    bool isExpr() const { return static_cast<bool>(expr); }
    bool isError() const { return static_cast<bool>(error); }

    Expr getExpr() const {
      if (!isExpr())
        throw std::bad_variant_access();
      return *expr;
    }
    Error getError() const {
      if (!isError())
        throw std::bad_variant_access();
      return *error;
    }

  private:
    std::optional<Expr> expr;
    std::optional<Error> error;
  };

  explicit Parser(std::istream &s) : stream(s) {}

  ParseResult parseTopLevel() {
    ParseResult result = parse();

    if (result.isError())
      return result;

    skipWhitespace();
    if (!reachedEnd())
      return error("Unexpected data at end of input");

    return result;
  }

private:
  ParseResult parse() {
    skipWhitespace();
    if (reachedEnd())
      return error("Unexpected end of input.");
    const char next = peek();
    if (next == '(')
      return parseList();
    if (next == '\"')
      return parseString();
    if (std::string("-0123456789").find(next) != std::string::npos)
      return parseIntOrFloat();
    return parseSymbol();
  }

  ParseResult parseList() {
    char openParen = pop();
    assert(openParen == '(');

    std::vector<Expr> children;

    while (true) {
      skipWhitespace();
      if (reachedEnd())
        return error("Missing closing parenthesis.");

      const char next = peek();
      if (next == ')') {
        (void)pop();
        break;
      }
      ParseResult child = parse();
      if (child.isError())
        return child;
      children.push_back(child.getExpr());
    }

    return Expr::List(children);
  }

  ParseResult parseString() {
    char openQuote = pop();
    assert(openQuote == '\"');

    std::string contents;

    while (true) {
      if (reachedEnd())
        return error("Reached end while parsing string: " + contents);
      const char next = pop();
      // Reached end of string.
      if (next == '\"')
        break;

      // Backslash escapes, so just copy the next character as is.
      if (next == '\\') {
        if (reachedEnd())
          return error("Reached end of input while finding escaped character");

        const char escaped = pop();
        contents.push_back(escaped);
        continue;
      }

      // All other characters are added to the string contents.
      contents.push_back(next);
    }

    return Expr::String(contents);
  }

  ParseResult parseIntOrFloat() {
    std::string contents;

    while (true) {
      if (reachedEnd())
        break;
      if (peekIsWhitespace())
        break;
      const char next = pop();
      if (next == ')')
        break;
      contents.push_back(next);
    }

    try {
      if (contents.find('.') == std::string::npos) {
        std::size_t end = 0;
        std::int64_t parsed = std::stoll(contents.data(), &end, /*base*/ 0);
        if (end != contents.size())
          return error("Failed to parse integer: " + contents);
        return Expr::Integer(parsed);
      }

      std::size_t end = 0;
      double parsed = std::stod(contents.data(), &end);
      if (end != contents.size())
        return error("Failed to parse float: " + contents);
      return Expr::Float(parsed);
    } catch (const std::invalid_argument &e) {
      return error("Invalid float/int value: " + contents);
    } catch (const std::out_of_range &e) {
      return error("Too large/small float or int value: " + contents);
    }
  }

  ParseResult parseSymbol() {
    std::string contents;

    while (true) {
      if (reachedEnd())
        break;
      if (peekIsWhitespace())
        break;
      const char next = peek();
      if (next == ')')
        break;
      contents.push_back(pop());
    }

    return Expr::Symbol(contents);
  }

  ParseResult error(std::string message) const {
    return Error(message, line, column);
  }

  bool reachedEnd() const { return atEnd; }

  void assertStreamGood() const {
    if (stream.fail())
      throw std::ios_base::failure("Failed to read input stream");
  }

  bool peekIsWhitespace() {
    const char p = peek();
    if (p == ' ' || p == '\t' || p == '\r' || p == '\n')
      return true;
    return false;
  }

  void skipWhitespace() {
    while (true) {
      if (reachedEnd())
        return;
      if (!peekIsWhitespace())
        break;
      (void)pop();
    }
  }

  char peek() {
    assert(!reachedEnd());
    char result = stream.peek();
    atEnd |= stream.eof();
    assertStreamGood();
    return result;
  }

  char pop() {
    char result = peek();
    if (result == '\n') {
      column = 1;
      line += 1;
    } else {
      column += 1;
    }
    // Advance stream.
    (void)stream.get();
    atEnd |= stream.eof();
    if (!reachedEnd())
      peek();

    return result;
  }

  std::istream &stream;
  std::size_t column = 1;
  std::size_t line = 1;
  bool atEnd = false;
};
}

#endif
