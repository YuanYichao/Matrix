#ifndef EXPRESSION
#define EXPRESSION
#include <memory>

template <typename T>
class Expression_base;

template <typename T>
class Expression;

template <typename T>
Expression<T> make_plus_expr(Expression<T>, Expression<T>);
template <typename T>
Expression<T> make_minus_expr(Expression<T>, Expression<T>);
template <typename T>
Expression<T> make_multi_expr(Expression<T>, Expression<T>);
template <typename T>
Expression<T> make_divide_expr(Expression<T>, Expression<T>);
template <typename T>
Expression<T> make_negative_expr(Expression<T>);
template <typename T>
Expression<T> make_positive_expr(Expression<T>);

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
  T eval() const { return *val; }
};

template <typename T>
class Expression {
  template <typename U>
  friend Expression<U> make_plus_expr(Expression<U>, Expression<U>);
  template <typename U>
  friend Expression<U> make_minus_expr(Expression<U>, Expression<U>);
  template <typename U>
  friend Expression<U> make_multi_expr(Expression<U>, Expression<U>);
  template <typename U>
  friend Expression<U> make_divide_expr(Expression<U>, Expression<U>);
  template <typename U>
  friend Expression<U> make_negative_expr(Expression<U>);
  template <typename U>
  friend Expression<U> make_positive_expr(Expression<U>);

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
  template <typename U>
  friend Expression<U> make_plus_expr(Expression<U>, Expression<U>);

  using value_type = typename binary_Expression<T>::value_type;
  using binary_Expression<T>::binary_Expression;

 private:
  virtual value_type eval() const override {
    return this->lhs.eval() + this->rhs.eval();
  }
};

template <typename T>
class minus_Expression : public binary_Expression<T> {
  using value_type = typename binary_Expression<T>::value_type;
  using binary_Expression<T>::binary_Expression;
  template <typename U>
  friend Expression<U> make_minus_expr(Expression<U>, Expression<U>);

 private:
  virtual value_type eval() const override {
    return this->lhs.eval() - this->rhs.eval();
  }
};

template <typename T>
class multi_Expression : public binary_Expression<T> {
  using value_type = typename binary_Expression<T>::value_type;
  using binary_Expression<T>::binary_Expression;
  template <typename U>
  friend Expression<U> make_multi_expr(Expression<U>, Expression<U>);

 private:
  virtual value_type eval() const override {
    return this->lhs.eval() * this->rhs.eval();
  }
};

template <typename T>
class divide_Expression : public binary_Expression<T> {
  using value_type = typename binary_Expression<T>::value_type;
  using binary_Expression<T>::binary_Expression;
  template <typename U>
  friend Expression<U> make_divide_expr(Expression<U>, Expression<U>);

 private:
  virtual value_type eval() const override {
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
  template <typename U>
  friend Expression<U> make_negative_expr(Expression<U>);

  virtual value_type eval() const override { return -this->expr.eval(); }
};

template <typename T>
Expression<T> make_plus_expr(Expression<T> lhs, Expression<T> rhs) {
  return std::shared_ptr<Expression_base<T>>(new plus_Expression<T>(lhs, rhs));
}
template <typename T>
Expression<T> make_minus_expr(Expression<T> lhs, Expression<T> rhs) {
  return std::shared_ptr<Expression_base<T>>(new minus_Expression<T>(lhs, rhs));
}
template <typename T>
Expression<T> make_multi_expr(Expression<T> lhs, Expression<T> rhs) {
  return std::shared_ptr<Expression_base<T>>(new multi_Expression<T>(lhs, rhs));
}
template <typename T>
Expression<T> make_divide_expr(Expression<T> lhs, Expression<T> rhs) {
  return std::shared_ptr<Expression_base<T>>(
      new divide_Expression<T>(lhs, rhs));
}
template <typename T>
Expression<T> make_negative_expr(Expression<T> expr) {
  return std::shared_ptr<Expression_base<T>>(new negative_Expression<T>(expr));
}
#endif