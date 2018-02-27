#include <iostream>
#include "../Expression_parser.hpp"

int main() {
  expression_parser<int> parser;
  std::cout << parser.parse("5*3").eval();
}