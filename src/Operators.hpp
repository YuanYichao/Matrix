#ifndef OPERATORS
#define OPERATORS
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include "Expression.hpp"
#include "parser_types.hpp"

struct op_info {
  std::string sign;
  unsigned char lhs;
  unsigned char rhs;
};
// class op_info_base {
//   static const std::vector<std::pair<op_info, op_types>> base;
// };



struct myhash {
  using argument_type = op_info;
  using result_type = std::size_t;
  result_type operator()(argument_type const& arg) const noexcept {
    return (arg.sign[0] << 16) | (arg.lhs << 8) | arg.rhs |
           arg.sign.size() << 32;
  }
};
bool operator==(const op_info& lhs, const op_info& rhs) {
  return myhash()(lhs) == myhash()(rhs);
}

class Operator_querier {
 public:
  static op_types query_type(std::string op_sign, token_type lhs_type,
                             token_type rhs_type) {
    return op_map_type[op_info{op_sign, lhs_type, rhs_type}];
  };
  static op_level query_level(op_types tp) { return op_map_level[tp]; };

 private:
  static std::unordered_map<op_info, op_types, myhash> op_map_type;
  static std::map<op_types, op_level> op_map_level;
};

std::unordered_map<op_info, op_types, myhash> Operator_querier::op_map_type = {
    {{"*", token_type::VALUE, token_type::VALUE}, op_types::MULTI},
    {{"*", token_type::VALUE, token_type::OPERATOR}, op_types::MULTI},
    {{"*", token_type::VALUE, token_type::SEQUENCE_BEG}, op_types::MULTI},
    {{"*", token_type::VALUE, token_type::SEQUENCE_END}, op_types::MULTI},
    {{"/", token_type::VALUE, token_type::VALUE}, op_types::DIVIDE},
    {{"/", token_type::VALUE, token_type::OPERATOR}, op_types::DIVIDE},
    {{"/", token_type::VALUE, token_type::SEQUENCE_BEG}, op_types::DIVIDE},
    {{"/", token_type::VALUE, token_type::SEQUENCE_END}, op_types::DIVIDE},
    {{"+", token_type::VALUE, token_type::VALUE}, op_types::PLUS},
    {{"+", token_type::VALUE, token_type::OPERATOR}, op_types::PLUS},
    {{"+", token_type::VALUE, token_type::SEQUENCE_BEG}, op_types::PLUS},
    {{"+", token_type::VALUE, token_type::SEQUENCE_END}, op_types::PLUS},
    {{"-", token_type::VALUE, token_type::VALUE}, op_types::MINUS},
    {{"-", token_type::VALUE, token_type::OPERATOR}, op_types::MINUS},
    {{"-", token_type::VALUE, token_type::SEQUENCE_BEG}, op_types::MINUS},
    {{"-", token_type::VALUE, token_type::SEQUENCE_END}, op_types::MINUS},
    {{"-", token_type::OPERATOR, token_type::VALUE}, op_types::NEGATIVE},
    {{"-", token_type::SEQUENCE_END, token_type::VALUE}, op_types::NEGATIVE},
    {{"-", token_type::SEQUENCE_BEG, token_type::VALUE}, op_types::NEGATIVE},
};

std::map<op_types, op_level> Operator_querier::op_map_level{
    {op_types::MULTI, op_level::MUL},
    {op_types::DIVIDE, op_level::MUL},
    {op_types::PLUS, op_level::PLS},
    {op_types::MINUS, op_level::PLS},
    {op_types::NEGATIVE, op_level::UNA}};

bool is_operator(std::string str_) {
  for (const auto& t : operator_table) {
    if (t == str_) return true;
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
    auto rhs_ = val_stack.top();
    val_stack.pop();
    auto lhs_ = val_stack.top();
    val_stack.pop();
    auto res_ = Expression<T>(std::shared_ptr<Expression_base<T>>(
        new plus_Expression<T>(lhs_, rhs_)));
    val_stack.push(res_);
  }
};

template <typename T>
class Minus_operator : public Operators_base<T> {
  friend Operators<T>;
  virtual void use_operator(std::stack<Expression<T>>& val_stack) override {
    auto rhs_ = val_stack.top();
    val_stack.pop();
    auto lhs_ = val_stack.top();
    val_stack.pop();
    auto res_ = Expression<T>(std::shared_ptr<Expression_base<T>>(
        new minus_Expression<T>(lhs_, rhs_)));
    val_stack.push(res_);
  }
};

template <typename T>
class Multi_operator : public Operators_base<T> {
  friend Operators<T>;
  virtual void use_operator(std::stack<Expression<T>>& val_stack) override {
    auto rhs_ = val_stack.top();
    val_stack.pop();
    auto lhs_ = val_stack.top();
    val_stack.pop();
    auto res_ = Expression<T>(std::shared_ptr<Expression_base<T>>(
        new multi_Expression<T>(lhs_, rhs_)));
    val_stack.push(res_);
  }
};

template <typename T>
class Divide_operator : public Operators_base<T> {
  friend Operators<T>;
  virtual void use_operator(std::stack<Expression<T>>& val_stack) override {
    auto rhs_ = val_stack.top();
    val_stack.pop();
    auto lhs_ = val_stack.top();
    val_stack.pop();
    auto res_ = Expression<T>(std::shared_ptr<Expression_base<T>>(
        new divide_Expression<T>(lhs_, rhs_)));
    val_stack.push(res_);
  }
};

template <typename T>
class Negative_operator : public Operators_base<T> {
  friend Operators<T>;
  virtual void use_operator(std::stack<Expression<T>>& val_stack) override {
    auto obj_ = val_stack.top();
    val_stack.pop();
    auto res_ = Expression<T>(
        std::shared_ptr<Expression_base<T>>(new negative_Expression<T>(obj_)));
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
      case op_types::NEGATIVE: {
        op = std::shared_ptr<Operators_base<T>>(new Negative_operator<T>);
      }
      default:
        break;
    }
  }
};

template <typename T>
bool operator<(const Operators<T>& op1, const Operators<T>& op2) {
  return Operator_querier::query_level(op1.tp) <
         Operator_querier::query_level(op2.tp);
}

#endif