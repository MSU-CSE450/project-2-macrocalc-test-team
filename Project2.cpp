#include <cassert>
#include <fstream>
#include <string>

#include <vector>

#include "ASTNode.hpp"
#include "Error.hpp"
#include "SymbolTable.hpp"
#include "lexer.hpp"

using namespace emplex;

class MacroCalc {
private:
  std::vector<Token> tokens{};
  emplex::Lexer lexer{};
  SymbolTable table{};
  size_t token_idx{0};
  ASTNode root{ASTNode::SCOPE};

  Token const &CurToken() const { return tokens.at(token_idx); }
  Token const &ConsumeToken() { return tokens.at(token_idx++); }
  Token const &ExpectToken(int token) {
    // TODO: allow explaining error
    if (CurToken() == token) {
      return ConsumeToken();
    }
    ErrorUnexpected(CurToken(), token);
  }

  ASTNode ParseScope() {
    ASTNode scope{ASTNode::SCOPE};
    table.PushScope();
    while (ConsumeToken() != Lexer::ID_SCOPE_END) {
      scope.AddChild(ParseStatement());
    }
    table.PopScope();
    return scope;
  }

  ASTNode ParseDecl() {
    ExpectToken(Lexer::ID_VAR);
    Token ident = ExpectToken(Lexer::ID_ID);
    table.AddVar(ident.lexeme, ident.line_id);
    if (CurToken() == Lexer::ID_ENDLINE) {
      return ASTNode{};
    }
    ExpectToken(Lexer::ID_ASSIGN);

    // TODO: idk if this is correct but class is in 6 minutes lol
    ASTNode node{ASTNode::ASSIGN};
    if (CurToken() == Lexer::ID_ID) {
      node.AddChild(ASTNode(ASTNode::IDENTIFIER, ident));
    } else if (CurToken() == Lexer::ID_NUMBER) {
      double val = std::stod(CurToken().lexeme);
      node.AddChild(ASTNode(ASTNode::NUMBER, val));
    } else {
      // TODO: error unexpected ID_ID or ID_NUMBER
    }
    return node;
  }

  ASTNode ParseStatement() {
    switch (CurToken()) {
    case Lexer::ID_SCOPE_Start:
      return ParseScope();
    case Lexer::ID_VAR:
      return ParseDecl();
    default:
      Error(CurToken(), "Unexpected token '", CurToken().lexeme, "'");
    }
  }

public:
  MacroCalc(std::ifstream &input) {
    tokens = lexer.Tokenize(input);
    Parse();
  };

  void Parse() {
    while (token_idx < tokens.size()) {
      root.AddChild(ParseStatement());
    }
  }

  void Execute() { root.Run(table); }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    ErrorNoLine("Format: ", argv[0], " [filename]");
  }

  std::string filename = argv[1];

  std::ifstream in_file(filename);
  if (in_file.fail()) {
    ErrorNoLine("Unable to open file '", filename, "'.");
  }

  MacroCalc calc{in_file};
  calc.Parse();
  calc.Execute();
}
