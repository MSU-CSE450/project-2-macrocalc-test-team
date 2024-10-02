#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct VariableInfo {
  std::string name;
  double value;
  size_t line_declared;
  bool initialized;
  VariableInfo() {}
  VariableInfo(std::string new_name, double new_value,
               size_t new_line_declared) {
    name = new_name;
    value = new_value;
    line_declared = new_line_declared;
    initialized = false;
  }
};

class SymbolTable {
private:
  typedef std::unordered_map<std::string, size_t> scope_t;
  std::vector<scope_t> scope_stack;
  std::vector<VariableInfo> all_variables;

public:
  SymbolTable() {
    scope_stack = std::vector<scope_t>();
    scope_t first_scope = scope_t();
    scope_stack.push_back(first_scope);
    all_variables = std::vector<VariableInfo>();
  }

  void PushScope() {
    scope_t new_scope = scope_t();
    this->scope_stack.push_back(new_scope);
  }

  void PopScope() {
    if (scope_stack.size() == 0) {
      std::cerr << "Error: tried to pop nonexistent scope" << std::endl;
      exit(1);
    } else if (scope_stack.size() == 1) {
      std::cerr << "Error: tried to pop outermost scope" << std::endl;
      exit(1);
    }
    scope_stack.pop_back();
  }

  bool HasVar(std::string name) const {
    for (auto curr_scope = scope_stack.rbegin();
         curr_scope != scope_stack.rend(); curr_scope++) {
      if (curr_scope->find(name) != curr_scope->end())
        return true;
    }
    return false;
  }

  size_t AddVar(std::string name, size_t line_num) {
    auto curr_scope = scope_stack.rbegin();
    if (curr_scope->find(name) != curr_scope->end()) {
      std::cerr << "Error: tried to declare variable already in scope"
                << std::endl;
      exit(1);
    }
    VariableInfo new_var_info = VariableInfo(name, 0.0, line_num);
    size_t new_index = this->all_variables.size();
    all_variables.push_back(new_var_info);
    curr_scope->insert({name, new_index});
    return new_index;
  }

  double GetValue(std::string name) const {
    assert(HasVar(name));
    for (auto curr_scope = scope_stack.rbegin();
         curr_scope != scope_stack.rend(); curr_scope++) {
      auto location_in_curr_scope = curr_scope->find(name);
      if (location_in_curr_scope != curr_scope->end()) {
        if (all_variables[location_in_curr_scope->second].initialized ==
            false) {
          std::cerr << "Error: tried to access variable which has been "
                       "declared but has no value"
                    << std::endl;
          exit(1);
        }
        return all_variables[curr_scope->at(name)].value;
      }
    }
  }

  void SetValue(std::string name, double new_value) {
    assert(HasVar(name));
    for (auto curr_scope = scope_stack.rbegin();
         curr_scope != scope_stack.rend(); curr_scope++) {
      auto location_in_curr_scope = curr_scope->find(name);
      if (location_in_curr_scope != curr_scope->end()) {
        all_variables[curr_scope->at(name)].value = new_value;
        all_variables[curr_scope->at(name)].initialized = true;
        break;
      }
    }
  }
};
