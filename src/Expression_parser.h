#ifndef EXPRESSION_PARSER
#define EXPRESSION_PARSER

#include <string>
#include "Expression.hpp"

template <typename T>
struct common_reader {
  static T read(std::string str);
};
template <>
struct common_reader<int> {
  static int read(std::string str) { return stoi(str); }
};

template <typename T>
struct expression_parser {
  static Expression<T> parse(std::string str) {
    std::string::size_type pos_;
    if ((pos_ = str.find_first_of("+-")) != std::string::npos) {
      if (str[pos_] == '+')
        return make_plus_expr(parse(str.substr(0, pos_)),
                              parse(str.substr(pos_ + 1)));
      else
        return make_minus_expr(parse(str.substr(0, pos_)),
                               parse(str.substr(pos_ + 1)));
    }
    if ((pos_ = str.find_first_of("*/")) != std::string::npos) {
      if (str[pos_] == '*')
        return make_multi_expr(parse(str.substr(0, pos_)),
                               parse(str.substr(pos_ + 1)));
      else
        return make_divide_expr(parse(str.substr(0, pos_)),
                                parse(str.substr(pos_ + 1)));
    }
    return Expression<int>(common_reader<int>::read(str));
  }
};

#endif