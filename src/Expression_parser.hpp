#ifndef EXPRESSION_PARSER
#define EXPRESSION_PARSER

#include <algorithm>
#include <cctype>
#include <deque>
#include <iostream>
#include <stack>
#include <string>
#include <utility>
#include "Expression.hpp"
#include "Operand.hpp"
#include "Operators.hpp"

template <typename T>
struct expression_parser {
  friend Operators<T>;
  using value_type = T;
  Expression<T> parse(std::string str) {
    clear_stack();
    split(str);
    do_parse();
    // std::string::size_type pos_, f_;
    // for (pos_ = 0, f_ = 0; pos_ < str.size(); ++pos_) {
    //   if (!is_operand_part(str[pos_])) {
    //     auto val = tanslator<T>::str_to_operand(str.substr(f_, pos_));
    //     Expression<T> expr_(val);
    //     val_stack.push(expr_);
    //     std::string::size_type t;
    //     for (t = 1; !is_operator(str.substr(pos_, t)); ++t)
    //       ;

    //     auto temp_ = str.substr(pos_, t);
    //     auto ot = *op_map_type.find(temp_);
    //     pos_ += t;
    //     Operators<T> op_(ot.second);
    //     if (!op_stack.empty()) {
    //       if (op_ < op_stack.top()) {
    //         eval();
    //       }
    //     }
    //     op_stack.push(op_);
    //     f_ = pos_;
    //   }
    // }
    // auto val = tanslator<T>::str_to_operand(str.substr(f_, pos_));
    // Expression<T> expr_(val);
    // val_stack.push(expr_);
    // eval();
    return val_stack.top();
  }

 private:
  enum type { CONSTANT, NAME, OPERATOR };
  void do_parse() {
    std::pair<std::string, type> token;
    while (!tokens.empty()) {
      token = tokens.front();
      tokens.pop_front();
      switch (token.second) {
        case CONSTANT: {
          auto v = tanslator<T>::str_to_operand(token.first);
          val_stack.push(v);
          break;
        }
        case OPERATOR: {
          if (token.first == ")") {
            eval_between();
            break;
          }
          auto ot = *op_map_type.find(token.first);
          Operators<T> op(ot.second);
          if (op.tp != PAR_LEFT && !op_stack.empty() && op < op_stack.top()) {
            eval_all();
          }
          op_stack.push(op);
          break;
        }
        default:
          break;
      }
    }
    eval_all();
  }
  void split(std::string str) {
    std::string::size_type pos_ = 0;
    pos_ += skip_space_from(str, pos_);
    while (pos_ < str.size()) {
      auto token = get_token(str.substr(pos_));
      pos_ += token.first.size();
      tokens.push_back(token);
      pos_ += skip_space_from(str, pos_);
    }
  }
  void eval_all() {
    while (!op_stack.empty()) {
      if (op_stack.top().tp != op_types::PAR_LEFT)
        op_stack.top().use_operator(val_stack);
      op_stack.pop();
    }
  }

  void eval_between() {
    while (op_stack.top().tp != op_types::PAR_LEFT) {
      op_stack.top().use_operator(val_stack);
      op_stack.pop();
    }
    op_stack.pop();
  }
  void clear_stack() {
    while (!val_stack.empty()) {
      val_stack.pop();
    }
    while (!op_stack.empty()) {
      op_stack.pop();
    }
  }
  std::string::size_type skip_space_from(std::string str,
                                         std::string::size_type start) {
    // // auto f = [](char ch_) -> bool { return std::isspace(ch_); };
    // // auto beg_ = std::remove_if(str.begin(), str.end(), f);
    // str.erase(beg_, str.end());
    std::string::size_type pos_;
    for (pos_ = start; pos_ < str.size(); ++pos_) {
      if (!std::isspace(str[pos_])) break;
    }
    return pos_ - start;
  }
  std::pair<std::string, type> get_token(std::string str) {
    char start = str[0];
    std::string res_;
    if (is_constant_begin(start)) {
      if (std::isdigit(start)) {
        res_ = read_a_num_token(str);
        return {res_, CONSTANT};
      }
      if (start == '[') {
        auto end = str.find(']');
        res_ = str.substr(0, end + 1);
        return {res_, CONSTANT};
      }
    } else if (is_name_begin(start)) {
      res_ = read_a_name_token(str);
      return {res_, NAME};
    } else if (is_op_begin(start)) {
      res_ = read_an_operator(str);
      return {res_, OPERATOR};
    }
  }

  bool is_constant_begin(char ch_) { return std::isdigit(ch_) || ch_ == '['; }
  bool is_name_begin(char ch_) { return std::isalpha(ch_) || ch_ == '_'; }
  bool is_op_begin(char ch_) {
    return ch_ == '-' || ch_ == '+' || ch_ == '/' || ch_ == '*' || ch_ == '(' ||
           ch_ == ')';
  }

  std::string read_a_num_token(std::string str) {
    std::string::size_type i;
    for (i = 0; i < str.size() && std::isdigit(str[i]); ++i) continue;
    if (i < str.size() && str[i] == '.') {
      for (++i; i < str.size() && std::isdigit(str[i]); ++i) continue;
    }
    return str.substr(0, i);
  }

  std::string read_a_name_token(std::string str) {
    auto end = str.find_first_not_of(
        "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_"
        "1234567890");
    if (end != std::string::npos) return str.substr(0, end);
    return str;
  }
  std::string read_an_operator(std::string str) {
    std::string::size_type len_;
    for (len_ = 1; len_ < str.size(); ++len_) {
      if (is_operator(str.substr(0, len_))) {
        return str.substr(0, len_);
      }
    }
    return str;
  }
  std::stack<Expression<T>> val_stack;
  std::stack<Operators<T>> op_stack;
  std::deque<std::pair<std::string, type>> tokens;
};

#endif