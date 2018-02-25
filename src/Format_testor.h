#ifndef FORMAT_TESTOR
#define FORMAT_TESTOR
#include <fstream>
#include <string>
#include <vector>


struct format_info {
  enum Type { Matrix, Sys };
  static format_info get_info_of(Type type_) {
    switch (type_) {
      case Matrix: {
        return format_info({'['}, {']'}, ';', {' ', '\t', '\n', ','});
      }
    }
  }
  const std::vector<char> begin_chs;
  const std::vector<char> end_chs;
  const char next_row_ch;
  const std::vector<char> trivial_chs;

 private:
  format_info(std::vector<char> bc, std::vector<char> ec, char nc,
              std::vector<char> tc)
      : begin_chs(bc), end_chs(ec), next_row_ch(nc), trivial_chs(tc) {}
};

class Format_testor {
 public:
  using Type = format_info::Type;
  static Format_testor &Init_testor(Type type_) {
    static Format_testor t(type_);
    return t;
  }
  bool is_begin(char ch_) const {
    for (auto t : info.begin_chs) {
      if (t == ch_) return true;
    }
    return false;
  }
  bool is_end(char ch_) const {
    for (auto t : info.end_chs) {
      if (t == ch_) return true;
    }
    return false;
  }
  bool is_next_row(char ch_) const { return ch_ == info.next_row_ch; }
  bool is_trivial(char ch_) const {
    for (auto t : info.trivial_chs) {
      if (t == ch_) return true;
    }
    return false;
  }
  ~Format_testor() = default;
  const format_info info;

 private:
  Format_testor(Type type_) : info(format_info::get_info_of(Type::Matrix)) {}

 public:
  Format_testor(const Format_testor &) = delete;
  Format_testor(Format_testor &&) = default;
  Format_testor &operator=(Format_testor &) = delete;
  Format_testor &operator=(Format_testor &&) = default;
};

#endif