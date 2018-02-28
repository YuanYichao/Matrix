#include <iostream>
#include "../Expression_parser.hpp"
#include "../interface.h"

int main() {
  expression_parser<double> parser;
  Interface interface;
  auto str = interface.get_command();
  while (str != "`") {
    if (str.size() == 0) {
      str = interface.get_command();
      continue;
    }
    auto res_ = parser.parse(str).eval();
    interface.print(res_);
    str = interface.get_command();
  }
}