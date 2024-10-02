#pragma once

#include <cmath>
#include <sstream>
#include <string>
#include <vector>

#include "SymbolTable.hpp"

class ASTNode {
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
private:
  // PLACE AST NODE INFO HERE.
  Type type{EMPTY};
  std::string literal{""};
  double value{0};
  std::vector<ASTNode> children{};
public:
  // CONSTRUCTORS, ETC HERE.
  // CAN SPECIFY NODE TYPE AND ANY NEEDED VALUES HERE OR USING OTHER FUNCTIONS.
  ASTNode(Type type = EMPTY) : type(type) {};
  ASTNode(Type type, std::string literal) : type(type), literal(literal) {};
  ASTNode(Type type, double value) : type(type), value(value) {};
  // CODE TO ADD CHILDREN AND SETUP AST NODE HERE.
  void AddChild(ASTNode node) { 
    children.push_back(node); 
  }
  
  // CODE TO EXECUTE THIS NODE (AND ITS CHILDREN, AS NEEDED).
  double Run(SymbolTable & symbols) { ; }
  
};
