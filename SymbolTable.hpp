#pragma once

#include <cassert>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Error.hpp"

struct VariableInfo {
  std::string name{};
  double value{};
  size_t line_declared{};
  bool initialized = false;
};

class SymbolTable {
private:
  typedef std::unordered_map<std::string, size_t> scope_t;
  std::vector<scope_t> scope_stack{1};
  std::vector<VariableInfo> all_variables{};

public:
  void PushScope() { this->scope_stack.emplace_back(); }

  void PopScope() {
    if (scope_stack.size() == 0) {
      throw std::runtime_error("tried to pop nonexistent scope");
    } else if (scope_stack.size() == 1) {
      throw std::runtime_error("tried to pop outermost scope");
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
      Error(line_num, "Redeclaration of variable ", name);
    }
    VariableInfo new_var_info = VariableInfo{name, 0.0, line_num};
    size_t new_index = this->all_variables.size();
    all_variables.push_back(new_var_info);
    curr_scope->insert({name, new_index});
    return new_index;
  }

  double GetValue(std::string name, size_t line_num) const {
    assert(HasVar(name));
    for (auto curr_scope = scope_stack.rbegin();
         curr_scope != scope_stack.rend(); curr_scope++) {
      auto location_in_curr_scope = curr_scope->find(name);
      if (location_in_curr_scope != curr_scope->end()) {
        if (all_variables[location_in_curr_scope->second].initialized ==
            false) {
          Error(line_num, "attempt to access uninitialized variable ", name);
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
