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
    IDENTIFIER,
    CONDITIONAL,
    OPERATION,
    NUMBER,
    STRING
  };
  const Type type;
  double value{};
  // can also serve as variable name when the node is of type
  // IDENTIFIER, or an operation name if of type OPERATION. Might also
  // change things so we have another enum of operator types.
  std::string literal{};

  ASTNode(Type type = EMPTY) : type(type) {};
  ASTNode(Type type, std::string literal) : type(type), literal(literal) {};
  ASTNode(Type type, double value) : type(type), value(value) {};

  operator int() const { return type; }

  void AddChild(ASTNode node) {
    if (node) {
      children.push_back(node);
    }
  }

  std::optional<double> Run(SymbolTable &symbols) {
    switch (type) {
    case EMPTY:
      return std::nullopt;
    case SCOPE:
      RunScope(symbols);
      return std::nullopt;
    case PRINT:
      RunPrint(symbols);
      return std::nullopt;
    case ASSIGN:
      RunAssign(symbols);
      return std::nullopt;
    case IDENTIFIER:
      return RunIdentifier(symbols);
    case CONDITIONAL:
      RunConditional(symbols);
      return std::nullopt;
    case OPERATION:
      return RunOperation(symbols);
    case STRING:
      // not really sure if we need to run these?
      // the only context in which we need to worry about string nodes is when
      // handling prints and we can just have RunPrint access the "literal" of
      // any string node
      return std::nullopt;
    default:
      return std::nullopt;
    };
  }

  // note: I would have made some of these SymbolTable references constant, but
  // they'll be making recursive calls to Run, some of which will need to make
  // changes to the symbol table, so none of them are constant except the one in
  // RunIdentifier
  void RunScope(SymbolTable &symbols) {
    // push a new scope
    // run each child node in order
    // pop scope
  }
  void RunPrint(SymbolTable &symbols) {
    // iterate over children
    // if child is an expression or number, run it and print the value it
    // returns if it's a string literal, print it need to do something about
    // identifiers in curly braces
  }
  void RunAssign(SymbolTable &symbols) {
    // get variable name from first child
    // possibly get a value from last child (which may be an expression to
    // evaluate or a variable node) if it's a declaration, call AddVar if it's a
    // declaration with initialization, call AddVar, then SetVar if it's just an
    // assignment, only call SetVar
  }
  double RunIdentifier(const SymbolTable &symbols) {
    // just need to call GetValue on the node's literal (which should be the
    // name of the variable), then return the result
    return 0;
  }
  void RunConditional(SymbolTable &symbols) {
    // conditional statement is of the form "if (expression1) statment1 else
    // statement2" so a conditional node should have 2 or 3 children: an
    // expression, a statement, and possibly another statement run the first
    // one; if it gives a nonzero value, run the second; otherwise, run the
    // third, if it exists
  }
  double RunOperation(SymbolTable &symbols) {
    // node will have an operator (e.g. +, *, etc.) specified somewhere (maybe
    // in the "literal"?) and one or two children run the child or children,
    // apply the operator to the returned value(s), then return the result
    return 0;
  }
};
