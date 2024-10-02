#pragma once

#include <cmath>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "SymbolTable.hpp"
class ASTNode {

private:
  std::vector<ASTNode> children{};

public:
  enum Type {
    EMPTY = 0,
    SCOPE,
    PRINT,
    ASSIGN,
    CONDITIONAL,
    OPERATION,
    NUMBER,
    STRING
  };
  const Type type{EMPTY};
  std::string literal{""};
  double value{0};

  ASTNode(Type type = EMPTY) : type(type) {};
  ASTNode(Type type, std::string literal) : type(type), literal(literal) {};
  ASTNode(Type type, double value) : type(type), value(value) {};

  void AddChild(ASTNode node) { children.push_back(node); }

  std::optional<double> Run(SymbolTable &symbols) { ; }
};
