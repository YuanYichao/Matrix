#ifndef EXPRESSION
#define EXPRESSION
#include <iostream>
#include <memory>
//#define DBG

template <typename T>
class Operators;

template <typename T>
class Plus_operator;

template <typename T>
class Minus_operator;

template <typename T>
class Multi_operator;

template <typename T>
class Divide_operator;

template <typename T>
class Negative_operator;

template <typename T>
class Expression_base;

template <typename T>
class Expression;

template <typename T>
class Expression_base {
 public:
  friend Expression<T>;
  using value_type = T;

 protected:
  virtual ~Expression_base() = default;

 private:
  virtual value_type eval() const = 0;
};

template <typename T>
class plain_Expression : public Expression_base<T> {
  using value_type = typename Expression_base<T>::value_type;
  friend Expression<T>;
  plain_Expression(const T& val_) : val(new T(val_)) {}
  std::shared_ptr<T> val;
  T eval() const {
#ifdef DBG
    std::cout << "plain eval. value: " << *val << std::endl;
#endif

    return *val;
  }
};

template <typename T>
class Expression {
  friend Operators<T>;
  friend Plus_operator<T>;
  friend Minus_operator<T>;
  friend Multi_operator<T>;
  friend Divide_operator<T>;
  friend Negative_operator<T>;

 public:
  Expression();
  Expression(const T& val_)
      : expr(std::shared_ptr<Expression_base<T>>(
            new plain_Expression<T>(val_))) {}
  T eval() const { return expr->eval(); }

 private:
  Expression(std::shared_ptr<Expression_base<T>> expr_) : expr(expr_) {}
  std::shared_ptr<Expression_base<T>> expr;
};

template <typename T>
class binary_Expression : public Expression_base<T> {
 protected:
  using value_type = typename Expression_base<T>::value_type;
  binary_Expression(const Expression<T> lhs_, const Expression<T> rhs_)
      : lhs(lhs_), rhs(rhs_) {}
  Expression<T> lhs, rhs;

 private:
  virtual value_type eval() const = 0;
};

template <typename T>
class plus_Expression : public binary_Expression<T> {
  friend Plus_operator<T>;
  using value_type = typename binary_Expression<T>::value_type;
  using binary_Expression<T>::binary_Expression;

 private:
  virtual value_type eval() const override {
#ifdef DBG
    std::cout << "plus eval. " << std::endl;
#endif
    return this->lhs.eval() + this->rhs.eval();
  }
};

template <typename T>
class minus_Expression : public binary_Expression<T> {
  using value_type = typename binary_Expression<T>::value_type;
  using binary_Expression<T>::binary_Expression;
  friend Minus_operator<T>;

 private:
  virtual value_type eval() const override {
#ifdef DBG
    std::cout << "minus eval. " << std::endl;
#endif
    return this->lhs.eval() - this->rhs.eval();
  }
};

template <typename T>
class multi_Expression : public binary_Expression<T> {
  using value_type = typename binary_Expression<T>::value_type;
  using binary_Expression<T>::binary_Expression;
  friend Multi_operator<T>;

 private:
  virtual value_type eval() const override {
#ifdef DBG
    std::cout << "multi eval. " << std::endl;
#endif
    return this->lhs.eval() * this->rhs.eval();
  }
};

template <typename T>
class divide_Expression : public binary_Expression<T> {
  using value_type = typename binary_Expression<T>::value_type;
  using binary_Expression<T>::binary_Expression;
  friend Divide_operator<T>;

 private:
  virtual value_type eval() const override {
#ifdef DBG
    std::cout << "divide eval. " << std::endl;
#endif
    return this->lhs.eval() / this->rhs.eval();
  }
};

template <typename T>
class unary_Expression : public Expression_base<T> {
 protected:
  using value_type = typename Expression_base<T>::value_type;
  unary_Expression(const Expression<T> expr_) : expr(expr_) {}
  Expression<T> expr;

 private:
  virtual value_type eval() const = 0;
};

template <typename T>
class negative_Expression : public unary_Expression<T> {
  using value_type = typename unary_Expression<T>::value_type;
  using unary_Expression<T>::unary_Expression;
  friend Negative_operator<T>;
  virtual value_type eval() const override {
#ifdef DBG
    std::cout << "negative eval. " << std::endl;
#endif
    return -this->expr.eval();
  }
};

#endif