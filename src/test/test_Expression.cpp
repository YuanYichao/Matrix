#include <iostream>
#include <string>
#include "../Expression.hpp"
#include "../Expression_parser.h"
int main() {
  Expression<int> expr1(1);
  Expression<int> expr2(2);
  auto expr3 = make_plus_expr(expr1, expr2);
  std::cout << expr3.eval() << std::endl;
  auto expr4 = make_minus_expr(expr1, expr2);
  std::cout << expr4.eval() << std::endl;
  auto expr5 = make_multi_expr(expr1, expr2);
  std::cout << expr5.eval() << std::endl;
  auto expr6 = make_divide_expr(expr1, expr2);
  std::cout << expr6.eval() << std::endl;
  auto expr7 = make_negative_expr(expr1);
  std::cout << expr7.eval() << std::endl;
  std::cout << expression_parser<int>::parse("122 + 120 / 5").eval()
            << std::endl;
}