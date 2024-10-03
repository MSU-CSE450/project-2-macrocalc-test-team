#include <cassert>
#include <fstream>
#include <string>

#include <vector>

#include "ASTNode.hpp"
#include "Error.hpp"
#include "SymbolTable.hpp"
#include "lexer.hpp"
#include "string_lexer.hpp"

using namespace emplex;

class MacroCalc {
private:
  std::vector<Token> tokens{};
  emplex::Lexer lexer{};
  SymbolTable table{};
  size_t token_idx{0};
  ASTNode root{ASTNode::SCOPE};

  emplex2::StringLexer string_lexer{};

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

  ASTNode ParseExpr(){
    return ASTNode{};
  }

  ASTNode ParsePrint(){
    ExpectToken(Lexer::ID_PRINT);
    ExpectToken(Lexer::ID_OPEN_PARENTHESIS);
    ASTNode node{ASTNode::PRINT};
    if (CurToken() == Lexer::ID_STRING){
      //strip quotes 
      std::string to_print = CurToken().lexeme.substr(1, CurToken().lexeme.length() - 2);
      std::vector<emplex2::Token> string_pieces = string_lexer.Tokenize(to_print);
      for (auto token : string_pieces){
        switch (token.id){
          case emplex2::StringLexer::ID_LITERAL:
            node.AddChild(ASTNode(ASTNode::STRING, token.lexeme));
            break;
          case emplex2::StringLexer::ID_ESCAPE_CHAR:
            node.AddChild(ASTNode(ASTNode::STRING, token.lexeme));
            break;
          case emplex2::StringLexer::ID_IDENTIFIER:
            node.AddChild(ASTNode(ASTNode::IDENTIFIER, token.lexeme.substr(1, token.lexeme.length() - 2)));
            break;
          default:
            ErrorUnexpected(CurToken(), Lexer::ID_STRING);
          //Since ID_LITERAL is a catchall for everything else, I don't think there should be any unexpected tokens in strings,
          //but I'll think about it some more and maybe  add some better error handling. 
        }
      }
      ConsumeToken();
      //Once we have ParseExpr working, we won't need this branch
    } else if (CurToken() == Lexer::ID_NUMBER){
      node.AddChild(ASTNode(ASTNode::NUMBER, std::stod(ConsumeToken().lexeme)));
    } else {
      node.AddChild(ParseExpr());
    }
    ExpectToken(Lexer::ID_CLOSE_PARENTHESIS);
    ExpectToken(Lexer::ID_ENDLINE);
    return node;
  }

  ASTNode ParseStatement() {
    switch (CurToken()) {
    case Lexer::ID_SCOPE_Start:
      return ParseScope();
    case Lexer::ID_VAR:
      return ParseDecl();
    case Lexer::ID_PRINT:
      return ParsePrint();
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
