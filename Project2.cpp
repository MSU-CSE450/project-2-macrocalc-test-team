#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>

#include "ASTNode.hpp"
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
    std::cout << "Format: " << argv[0] << " [filename]" << std::endl;
    exit(1);
  }

  std::string filename = argv[1];

  std::ifstream in_file(filename); // Load the input file
  if (in_file.fail()) {
    std::cout << "ERROR: Unable to open file '" << filename << "'."
              << std::endl;
    exit(1);
  }

  MacroCalc calc = MacroCalc();
  calc.Parse(in_file);
  calc.Execute();
}
