#ifndef OPERAND
#define OPERAND
#include <string>

const std::string operand_chars = "1234567890";

template <typename T>
struct tanslator {
  static T str_to_operand(std::string str_);
};
template <>
struct tanslator<int> {
  static int str_to_operand(std::string str_) { return stoi(str_); }
};

bool is_operand_part(char ch) {
  return operand_chars.find(ch) != std::string::npos;
}

#endif