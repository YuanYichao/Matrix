#ifndef INTERFACE
#define INTERFACE
#include <iostream>
#include <string>

class Interface {
 public:
  Interface() {}
  std::string get_command() {
    print_prompt();
    std::string res_;
    std::getline(in, res_);
    return res_;
  }
  template <typename T>
  void print(T res) {
    print_prompt();
    out << res << std::endl;
  }

 private:
  void print_prompt() { out << ">>>"; }
  std::istream& in = std::cin;
  std::ostream& out = std::cout;
};

#endif