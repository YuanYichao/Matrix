#ifndef OPERAND
#define OPERAND
#include <string>

const std::string operand_chars = "1234567890";

template <typename T>
struct translator {
  static T str_to_operand(std::string str_);
};
template <>
struct translator<int> {
  static int str_to_operand(std::string str_) { return stoi(str_); }
};
template<>
struct translator<double>{
  static double str_to_operand(std::string str_) { return stod(str_); }
};

bool is_operand_part(char ch) {
  return operand_chars.find(ch) != std::string::npos;
}

#endif