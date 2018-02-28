#ifndef OPERATORS
#define OPERATORS
#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include "Expression.hpp"
enum op_types { MULTI, DIVIDE, PLUS, MINUS, NEGATIVE, PAR_LEFT, PAR_RIGTH };
enum op_level { PLS, MUL, PAR };
std::multimap<std::string, op_types> op_map_type = {
    {"*", op_types::MULTI},    {"/", op_types::DIVIDE},
    {"+", op_types::PLUS},     {"-", op_types::MINUS},
    {"-", op_types::NEGATIVE}, {"(", op_types::PAR_LEFT},
    {")", op_types::PAR_RIGTH}};

std::map<op_types, op_level> op_map_level = {{op_types::MULTI, op_level::MUL},
                                             {op_types::DIVIDE, op_level::MUL},
                                             {op_types::PLUS, op_level::PLS},
                                             {op_types::MINUS, op_level::PLS}};

bool is_operator(std::string str_) {
  for (const auto& t : op_map_type) {
    if (t.first == str_) return true;
  }
  if (str_ == "(" || str_ == ")") {
    return true;
  }
  return false;
}

template <typename T>
class Operators;

template <typename T>
class Operators_base {
  friend Operators<T>;
  virtual void use_operator(std::stack<Expression<T>>& val_stack) = 0;

 protected:
  virtual ~Operators_base() = default;
};

template <typename T>
class Plus_operator : public Operators_base<T> {
  friend Operators<T>;
  virtual void use_operator(std::stack<Expression<T>>& val_stack) override {
    auto expr1 = val_stack.top();
    val_stack.pop();
    auto expr2 = val_stack.top();
    val_stack.pop();
    auto res_ = make_plus_expr(expr2, expr1);
    val_stack.push(res_);
  }
};

template <typename T>
class Minus_operator : public Operators_base<T> {
  friend Operators<T>;
  virtual void use_operator(std::stack<Expression<T>>& val_stack) override {
    auto expr1 = val_stack.top();
    val_stack.pop();
    auto expr2 = val_stack.top();
    val_stack.pop();
    auto res_ = make_minus_expr(expr2, expr1);
    val_stack.push(res_);
  }
};

template <typename T>
class Multi_operator : public Operators_base<T> {
  friend Operators<T>;
  virtual void use_operator(std::stack<Expression<T>>& val_stack) override {
    auto expr1 = val_stack.top();
    val_stack.pop();
    auto expr2 = val_stack.top();
    val_stack.pop();
    auto res_ = make_multi_expr(expr2, expr1);
    val_stack.push(res_);
  }
};

template <typename T>
class Divide_operator : public Operators_base<T> {
  friend Operators<T>;
  virtual void use_operator(std::stack<Expression<T>>& val_stack) override {
    auto expr1 = val_stack.top();
    val_stack.pop();
    auto expr2 = val_stack.top();
    val_stack.pop();
    auto res_ = make_divide_expr(expr2, expr1);
    val_stack.push(res_);
  }
};
template <typename T>
bool operator<(const Operators<T>& op1, const Operators<T>& op2);

template <typename T>
class Operators {
  template <typename U>
  friend bool operator<(const Operators<U>& op1, const Operators<U>& op2);
  std::shared_ptr<Operators_base<T>> op;

 public:
  const op_types tp;
  void use_operator(std::stack<Expression<T>>& val_stack) {
    op->use_operator(val_stack);
  }
  Operators(op_types t) : op(nullptr), tp(t) {
    switch (t) {
      case op_types::MULTI: {
        op = std::shared_ptr<Operators_base<T>>(new Multi_operator<T>);
        break;
      }
      case op_types::DIVIDE: {
        op = std::shared_ptr<Operators_base<T>>(new Divide_operator<T>);
        break;
      }
      case op_types::PLUS: {
        op = std::shared_ptr<Operators_base<T>>(new Plus_operator<T>);
        break;
      }
      case op_types::MINUS: {
        op = std::shared_ptr<Operators_base<T>>(new Minus_operator<T>);
        break;
      }
      case op_types::PAR_LEFT:
      case op_types::PAR_RIGTH:
      default:
        break;
    }
  }
};

template <typename T>
bool operator<(const Operators<T>& op1, const Operators<T>& op2) {
  return op_map_level[op1.tp] < op_map_level[op2.tp];
}

#endif