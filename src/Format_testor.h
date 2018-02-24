#ifndef FORMAT_TESTOR
#define FORMAT_TESTOR
#include <fstream>
#include <string>
#include <vector>

struct format_info {
  enum Type { Matrix };
  static format_info get_info_of(Type type_) {
    switch (type_) {
      case Matrix: {
        return format_info('[', ']', ';', {' ', ',', '\n'}, {' ', '\n', '\t'});
      }
    }
  }
  const char begin_ch;
  const char end_ch;
  const char next_row_ch;
  const std::vector<char> trivial_chs;
  const std::vector<char> sys_trivial_chs;

 private:
  format_info(char bc, char ec, char nc, std::vector<char> tc,
              std::vector<char> sc)
      : begin_ch(bc),
        end_ch(ec),
        next_row_ch(nc),
        trivial_chs(tc),
        sys_trivial_chs(sc) {}
};

class Format_testor {
 public:
  using Type = format_info::Type;
  static Format_testor &Init_testor(Type type_) {
    static Format_testor t(type_);
    return t;
  }
  bool is_sys_trivial(char ch_) const {
    for (auto t : info.sys_trivial_chs) {
      if (t == ch_) return true;
    }
    return false;
  }
  bool is_begin(char ch_) const { return ch_ == info.begin_ch; }
  bool is_end(char ch_) const { return ch_ == info.end_ch; }
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