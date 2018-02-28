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
#include "parser_types.hpp"

template <typename T>
struct expression_parser {
  using value_type = T;
  Expression<T> parse(std::string str) {
    clear_stack();
    split(str);
    do_parse();
    return val_stack.top();
  }

 private:
  void do_parse() {
    std::pair<std::string, token_type> token, prev = {"", token_type::OPERATOR};
    while (!tokens.empty()) {
      token = tokens.front();
      tokens.pop_front();
      switch (token.second) {
        case token_type::VALUE: {
          auto v = translator<T>::str_to_operand(token.first);
          val_stack.push(v);  // push a plain Expression to the stack
          if (!op_stack.empty()) {
            auto level = Operator_querier::query_level(op_stack.top().tp);
            if (level == op_level::UNA) {
              op_stack.top().use_operator(val_stack);
              op_stack.pop();
            }
          }
          break;
        }
        case token_type::OPERATOR: {
          auto ot = Operator_querier::query_type(token.first, prev.second,
                                                 tokens.front().second);
          Operators<T> op(ot);
          if (!op_stack.empty() && op < op_stack.top() &&
              seq_stack.top() < op_stack.size()) {
            eval_all();
          }
          op_stack.push(op);
          break;
        }
        case token_type::SEQUENCE_BEG: {
          seq_stack.push(op_stack.size());  // push the position of the latest
                                            // parenthese on to a stack
          break;
        }
        case token_type::SEQUENCE_END: {  // use part of the operators
          eval_between();
          break;
        }
        default:
          break;
      }
      prev = token;
    }
    eval_all();
  }
  // split string to tokens
  void split(std::string str) {
    std::string::size_type pos_ = 0;
    pos_ += skip_space_from(str, pos_);  // skip white space
    while (pos_ < str.size()) {
      auto token = get_token(str.substr(pos_));  // get a token
      pos_ += token.first.size();                // move forward pos_
      tokens.push_back(token);
      pos_ += skip_space_from(str, pos_);  // skip white space
    }
  }
  void eval_all() {
    while (!op_stack.empty()) {
      op_stack.top().use_operator(val_stack);
      op_stack.pop();
    }
  }

  void eval_between() {
    // use all operators between parentheses
    while (op_stack.size() != seq_stack.top()) {
      op_stack.top().use_operator(val_stack);
      op_stack.pop();
    }
    seq_stack.pop();
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
  std::pair<std::string, token_type> get_token(std::string str) {
    char start = str[0];
    std::string res_;
    if (is_constant_begin(start)) {
      if (std::isdigit(start)) {
        res_ = read_a_num_token(str);
        return {res_, token_type::VALUE};
      }
      if (start == '[') {
        auto end = str.find(']');
        res_ = str.substr(0, end + 1);
        return {res_, token_type::VALUE};
      }
    } else if (is_name_begin(start)) {
      res_ = read_a_name_token(str);
      return {res_, token_type::NAME};
    } else if (is_op_begin(start)) {
      res_ = read_an_operator(str);
      return {res_, token_type::OPERATOR};
    } else if (is_seq_begin(start)) {
      res_ = read_a_sequence_point(str);
      return {res_, token_type::SEQUENCE_BEG};
    } else if (is_seq_end(start)) {
      res_ = read_a_sequence_point(str);
      return {res_, token_type::SEQUENCE_END};
    }
  }

  bool is_constant_begin(char ch_) { return std::isdigit(ch_) || ch_ == '['; }
  bool is_name_begin(char ch_) { return std::isalpha(ch_) || ch_ == '_'; }
  bool is_op_begin(char ch_) {
    return ch_ == '-' || ch_ == '+' || ch_ == '/' || ch_ == '*';
  }
  bool is_seq_begin(char ch_) { return ch_ == '('; }
  bool is_seq_end(char ch_) { return ch_ == ')'; }
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

  std::string read_a_sequence_point(std::string str) {
    return str.substr(0, 1);
  }
  std::stack<Expression<T>> val_stack;
  std::stack<Operators<T>> op_stack;
  std::stack<size_t> seq_stack;
  std::deque<std::pair<std::string, token_type>> tokens;
};

#endif