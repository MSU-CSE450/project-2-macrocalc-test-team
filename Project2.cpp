#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Below are some suggestions on how you might want to divide up your project.
// You may delete this and divide it up however you like.
#include "ASTNode.hpp"
#include "lexer.hpp"
#include "SymbolTable.hpp"

class MacroCalc {
private:
  std::vector<emplex::Token> tokens{};
  emplex::Lexer lexer{};
  SymbolTable table{};
  size_t token_id{0};
  ASTNode root{ASTNode::SCOPE};



public:
  MacroCalc() {};
  void Parse(std::ifstream & input){
    tokens = lexer.Tokenize(input);
    //parse everything into an AST
  }
  void Execute(){
    root.Run(table);
  }
};

int main(int argc, char * argv[])
{
  if (argc != 2) {
    std::cout << "Format: " << argv[0] << " [filename]" << std::endl;
    exit(1);
  }

  std::string filename = argv[1];
  
  std::ifstream in_file(filename);              // Load the input file
  if (in_file.fail()) {
    std::cout << "ERROR: Unable to open file '" << filename << "'." << std::endl;
    exit(1);
  }

  // TO DO:  
  // PARSE input file to create Abstract Syntax Tree (AST).
  // EXECUTE the AST to run your program.
  MacroCalc calc = MacroCalc();
  calc.Parse(in_file);
  calc.Execute();
}
