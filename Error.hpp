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

template <typename... Ts> [[noreturn]] void Error(Token token, Ts... message) {
  Error(token.line_id, message...);
}

template <typename... Ts> [[noreturn]] void ErrorNoLine(Ts... message) {
  std::cerr << "ERROR: ";
  (std::cerr << ... << message);
  std::cerr << std::endl;
  exit(1);
}
