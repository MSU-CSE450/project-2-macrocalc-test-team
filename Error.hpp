#pragma once
#include "lexer.hpp"

using namespace emplex;

// From WordLang Error
template <typename... Ts>
[[noreturn]] void Error(size_t line_num, Ts... message) {
  std::cerr << "ERROR (line " << line_num << "): ";
  (std::cerr << ... << message);
  std::cerr << std::endl;
  exit(1);
}

template <typename... Ts>
[[noreturn]] void Error(Token const &token, Ts... message) {
  Error(token.line_id, message...);
}

// TODO: move into impl file
// TODO: no expected token version of this function (also maybe multiple
// expected tokens?)
// TODO: replace bespoke unexpected token error with this error
[[noreturn]] void ErrorUnexpected(Token const &token, int expected) {
  std::cerr << "ERROR (line " << token.line_id << "): ";
  std::cerr << "Expected token '" << Lexer::TokenName(expected)
            << "', got token '" << Lexer::TokenName(token) << std::endl;
  std::cerr << "Unexpected token: '" << token.lexeme << "'"
            << std::endl; // TODO: better wording
  exit(1);
}

template <typename... Ts> [[noreturn]] void ErrorNoLine(Ts... message) {
  std::cerr << "ERROR: ";
  (std::cerr << ... << message);
  std::cerr << std::endl;
  exit(1);
}

// TODO: add an "Unexpected token" error
