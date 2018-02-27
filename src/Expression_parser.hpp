#ifndef EXPRESSION_PARSER
#define EXPRESSION_PARSER

#include <stack>
#include <string>
#include "Expression.hpp"
#include "Operand.hpp"
#include "Operators.hpp"

template <typename T>
struct expression_parser {
  using value_type = T;
  Expression<T> parse(std::string str) {
    clear_stack();
    std::string::size_type pos_, f_;
    for (pos_ = 0, f_ = 0; pos_ < str.size(); ++pos_) {
      if (!is_operand_part(str[pos_])) {
        auto val = tanslator<T>::str_to_operand(str.substr(f_, pos_));
        Expression<T> expr_(val);
        val_stack.push(expr_);
        std::string::size_type t;
        for (t = 1; !is_operator(str.substr(pos_, t)); ++t)
          ;

        auto temp_ = str.substr(pos_, t);
        auto ot = *op_map_type.find(temp_);
        pos_ += t;
        Operators<T> op_(ot.second);
        if (!op_stack.empty()) {
          if (op_ < op_stack.top()) {
            eval();
          }
        }
        op_stack.push(op_);
        f_ = pos_;
      }
    }
    auto val = tanslator<T>::str_to_operand(str.substr(f_, pos_));
    Expression<T> expr_(val);
    val_stack.push(expr_);
    eval();
    return val_stack.top();
  }

 private:
  void eval() {
    while (!op_stack.empty()) {
      op_stack.top().use_operator(val_stack);
      op_stack.pop();
    }
  }
  void clear_stack() {
    while (!val_stack.empty()) {
      val_stack.pop();
    }
    while (!op_stack.empty()) {
      op_stack.pop();
    }
  }
  std::stack<Expression<T>> val_stack;
  std::stack<Operators<T>> op_stack;
};

#endif