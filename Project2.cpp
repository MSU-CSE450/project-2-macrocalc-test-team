#include <cassert>
#include <fstream>
#include <string>

#include <vector>

#include "ASTNode.hpp"
#include "Error.hpp"
#include "SymbolTable.hpp"
#include "lexer.hpp"

class MacroCalc {
private:
  std::vector<emplex::Token> tokens{};
  emplex::Lexer lexer{};
  SymbolTable table{};
  size_t token_id{0};
  ASTNode root{ASTNode::SCOPE};

public:
  MacroCalc() {};
  void Parse(std::ifstream &input) {
    tokens = lexer.Tokenize(input);
    // parse everything into an AST
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

  MacroCalc calc = MacroCalc();
  calc.Parse(in_file);
  calc.Execute();
}
