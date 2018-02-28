#include <iostream>
#include "../Expression_parser.hpp"

int main() {
  expression_parser<double> parser;
  int t = parser.parse("(1+5) *(2+1) /3").eval();
  std::cout << t;
}