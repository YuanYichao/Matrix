#ifndef READER
#define READER
#include <cctype>
#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include "Format_testor.h"
struct format_error : std::exception {
  using size_type = std::string::size_type;
  format_error() {}
  format_error(std::string fault, std::string str, size_type pos_)
      : words(fault + "\n" + str + "\n") {
    auto temp = std::string(fault.size(), ' ');
    temp[pos_] = '^';
    words += temp;
    words += "\n";
    auto sz_ = temp.size();
    while (pos_ <= sz_) {
      temp[pos_++] = '~';
    }
    words += temp;
    words += "\n";
  }
  virtual const char *what() const noexcept override { return words.c_str(); }

 private:
  std::string words;
};

template <typename T>
class Reader_base {
 public:
  using Type = Format_testor::Type;
  virtual T read(std::ifstream &in_) = 0;
  virtual T read(std::string str_) = 0;
  // virtual T read(istream &in_) = 0;
  Reader_base(Type type_) : testor(Format_testor::Init_testor(type_)) {}

 protected:
  const Format_testor &testor;
  std::string snap(std::ifstream &in_) {
    std::string temp;
    char t;
    while (!testor.is_end(t = in_.get())) {
      temp.push_back(t);
    }
    return temp;
  }
};

template <typename T>
struct reader_type_traits {
  using size_type = typename T::size_type;
  using value_type = typename T::value_type;
};

template <typename T>
class Matrix_reader : public Reader_base<T> {
 public:
  using size_type = typename reader_type_traits<T>::size_type;
  using value_type = typename reader_type_traits<T>::value_type;
  using Type = typename Reader_base<T>::Type;
  Matrix_reader() : Reader_base<T>(Type::Matrix) {}

  virtual T read(std::ifstream &in_) override {
    auto str_ = this->snap(in_);
    return do_read(str_);
  }
  virtual T read(std::string str_) override { return do_read(str_); }

 private:
  T do_read(std::string str_) {
    str_ = str_.substr(1);
    size_type w_ = 0, h_ = 0;
    std::vector<value_type> temp;
    value_type t;
    typename std::string::size_type pos_ = 0;
    while ((pos_ = str_.find(this->testor.info.next_row_ch)) !=
           std::string::npos) {
      std::stringstream stm_(str_.substr(0, pos_++));
      get_val(stm_, temp);
      if (!w_) w_ = temp.size();
      str_ = str_.substr(pos_);
    }

    std::stringstream stm_(str_);
    get_val(stm_, temp);

    if (!w_) {
      return T();
    } else {
      h_ = temp.size() / w_;
      T res_(temp.begin(), temp.end());
      res_.resize(h_, w_);
      return res_;
    }
  }
  bool get_val(std::stringstream &in_, std::vector<value_type> &temp_) {
    if (std::is_integral<value_type>::value)
      return get_val_integer(in_, temp_);
    else
      return get_val_float(in_, temp_);
  }

  bool get_val_integer(std::stringstream &in_, std::vector<value_type> &temp_) {
    char ch_;
    value_type t;
    while (in_.rdbuf()->in_avail()) {  // check whether the buffer is empty
      while (this->testor.is_trivial(ch_ = in_.get()))
        ;
      if (std::isdigit(ch_)) {
        in_.unget();
      } else {
        throw format_error();
      }
      in_ >> t;
      temp_.push_back(t);
    }
  }

  bool get_val_float(std::stringstream &in_, std::vector<value_type> &temp_) {
    char ch_;
    value_type t;
    while (in_.rdbuf()->in_avail()) {
      while (this->testor.is_trivial(ch_ = in_.get()))
        ;
      if (std::isdigit(ch_) || ch_ == '.') {
        in_.unget();
      } else {
        throw format_error();
      }
      in_ >> t;
      temp_.push_back(t);
    }
  }
};

#endif