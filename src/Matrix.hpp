#ifndef MATRIX
#define MATRIX
#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <utility>
#include <vector>
#include "reader.hpp"

///---own generic algorithm------

/**
 * naive version
 */
template <typename It1, typename It2, typename It3>
inline void add_sequence_to(It1 beg1_, It2 end1_, It3 des_) {
  while (beg1_ != end1_) {
    *des_++ += *beg1_++;
  }
}

template <typename It1, typename It2, typename It3>
inline void minus_sequence_from(It1 beg1_, It2 end1_, It3 des_) {
  while (beg1_ != end1_) {
    *des_++ -= *beg1_++;
  }
}

template <typename It1, typename It2, typename It3>
inline void multiply_sequence_to(It1 beg1_, It2 end1_, It3 des_) {
  while (beg1_ != end1_) {
    *des_++ *= *beg1_++;
  }
}

template <typename It1, typename It2, typename It3, typename It4>
inline void multiply_two_sequences_to(It1 beg1_, It2 end1_, It3 beg2_,
                                      It4 des_) {
  while (beg1_ != end1_) {
    *des_++ = *beg1_++ * *beg2_++;
  }
}

///-------------------------------

struct size_mismatch : public std::logic_error {
  using logic_error::logic_error;
  using logic_error::what;
};

template <typename T>
class Matrix;

template <typename T>
class col_iterator;

template <typename T>
struct std::iterator_traits<col_iterator<T>> {
  using difference_type = ptrdiff_t;
  using pointer = T *;
  using value_type = T;
  using reference = T &;
  using iterator_category = std::random_access_iterator_tag;
};

template <typename T>
class Matrix_reader;

/**
 * post-condition: print the met into the file associated with out
 */
template <typename T>
std::ostream &operator<<(std::ostream &out, const Matrix<T> &met_);

/**
 * low efficiency
 */
template <typename T>
std::istream &operator>>(std::istream &in, Matrix<T> &met_);

template <typename T>
void swap(Matrix<T> &met1, Matrix<T> &met2) {
  met1.swap(met2);
}

template <typename T>
bool is_identical_size(const Matrix<T> &met1, const Matrix<T> &met2) {
  return met1.col_length == met2.col_length &&
         met1.row_length == met2.row_length;
}

template <typename T>
bool is_mult_size(const Matrix<T> &met1, const Matrix<T> &met2) {
  return met1.col_length == met2.row_length;
}

template <typename T>
class Matrix {
 public:
  ///-------------------attributes----------
  friend col_iterator<T>;
  friend std::ostream &operator<<<>(std::ostream &out, const Matrix<T> &met_);
  friend Matrix_reader<T>;
  friend bool is_identical_size<>(const Matrix<T> &met1, const Matrix<T> &met2);
  friend bool is_mult_size<>(const Matrix<T> &met1, const Matrix<T> &met2);
  using size_type = size_t;
  using value_type = T;
  using iterator = T *;
  using reference = T &;
  using const_reference = const reference;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<T *>;
  using const_reverse_iterator = const reverse_iterator;
  using column_iterator = col_iterator<T>;
  using const_column_iterator = const column_iterator;
  ///--------------End attributes------------
  ///--------------methods----------------------
  ///--------------------------------------------
  /// copy control, ctor, dtor
  ///--------------------------------------------
  /**
   * default constructor, create an unit Matrix
   */
  Matrix() : row_length(0), col_length(0), beg(nullptr), ed(beg), m_size(0) {}
  /**
   * constructor which creates an unit Matrix
   */
  Matrix(size_type sz_)
      : row_length(sz_),
        col_length(sz_),
        beg(ator.allocate(proper_size(sz_, sz_))),
        ed(beg),
        m_size(proper_size(sz_, sz_)) {
    for (size_type i = 0; i < size(); i++) {
      if (i % sz_ == i / sz_) {
        ator.construct(ed++, 1);
      } else {
        ator.construct(ed++, 0);
      }
    }
  }
  /**
   * create a Matrix with uniform value
   */
  Matrix(const size_type i_, const size_type j_, const T &val_ = 0)
      : row_length(i_),
        col_length(j_),
        beg(ator.allocate(proper_size(i_, j_))),
        ed(beg),
        m_size(proper_size(i_, j_)) {
    for (size_type i = 0; i < size(); i++) {
      ator.construct(ed++, val_);
    }
  }
  /**
   * convert an array to Matrix
   */
  template <size_t C>
  Matrix(const T (&arr_)[C])
      : row_length(1),
        col_length(C),
        beg(ator.allocate(proper_size(C, 1))),
        ed(beg),
        m_size(proper_size(C, 1)) {
    for (size_type i = 0; i < C; i++) ator.construct(ed++, 0);
    std::copy(std::begin(arr_), std::end(arr_), beg);
  }
  /**
   * convert an array to Matrix with given size
   */
  template <size_t C>
  Matrix(const size_type i_, const size_type j_, const T (&arr_)[C])
      : row_length(i_),
        col_length(j_),
        beg(ator.allocate(proper_size(i_, j_))),
        ed(beg),
        m_size(proper_size(i_, j_)) {
    for (size_type i = 0; i < size(); i++) ator.construct(ed++, 0);
    if (C <= size()) {
      std::copy(std::begin(arr_), std::end(arr_), beg);
    } else {
      std::copy_n(std::begin(arr_), C, beg);
    }
  }
  /**
   * constructor to support it-initialization
   */
  template <typename IT>
  Matrix(const IT beg_, const IT end_)
      : row_length(1),
        col_length(std::distance(beg_, end_)),
        beg(ator.allocate(proper_size(row_length, col_length))),
        ed(beg),
        m_size(proper_size(row_length, col_length)) {
    for (size_type i = 0; i < size(); i++) ator.construct(ed++, 0);
    std::copy(beg_, end_, beg);
  }
  /**
   * constructor to support list initialization
   */
  Matrix(const std::initializer_list<T> in_) : Matrix(in_.begin(), in_.end()) {}
  /**
   * copy constructor
   */
  Matrix(const Matrix &met_)
      : row_length(met_.row_length),
        col_length(met_.col_length),
        beg(ator.allocate(met_.max_size())),
        ed(beg),
        m_size(met_.max_size()) {
    for (size_type i = 0; i < size(); i++) ator.construct(ed++, 0);
    std::copy(met_.begin(), met_.end(), beg);
  }
  /**
   * move constructor
   * use copy and swap
   */
  Matrix(Matrix &&met_) : Matrix() { swap(met_); }
  /**
   * swap
   */
  void swap(Matrix &met_) {
    using std::swap;

    swap(row_length, met_.row_length);
    swap(col_length, met_.col_length);
    swap(beg, met_.beg);
    swap(m_size, met_.m_size);
    swap(ed, met_.ed);
  }
  /**
   * move && copy assignment operator
   */
  Matrix &operator=(Matrix met_) {
    swap(met_);
    return *this;
  }
  /**
   * destructor
   */
  ~Matrix() { free_resource(); }
  ///---------------------------------------
  /// operator overloading
  ///---------------------------------------
  /**
   * Matrix addition
   * notice: not symmetric
   * @exception
   */
  Matrix operator+(const Matrix &met_) const {
    if (!is_identical_size(*this, met_))
      throw size_mismatch("during Martix addition, size mismatch");
    else {
      auto res_ = *this;
      add_sequence_to(met_.beg, met_.ed, res_.beg);
      return res_;
    }
  }
  /**
   * scalar addition
   * notice: not symmetric
   */
  Matrix operator+(const T scl_) const noexcept {
    auto res_ = *this;
    auto f = [&scl_](reference t) { t += scl_; };
    std::for_each(res_.begin(), res_.end(), f);
    return res_;
  }
  /**
   * Matrix multiply
   * @exception
   */
  Matrix operator*(const Matrix &met_) const {
    if (!is_mult_size(*this, met_))
      throw size_mismatch("during Matrix multiplication, size mismatches");
    else {
      Matrix res_(row_length, met_.col_length, 0);
      std::vector<T> p_(res_.col_length, 0);
      for (size_type i = 0; i < res_.row_length; ++i) {
        for (size_type j = 0; j < res_.col_length; ++j) {
          auto r_beg = (*this).cbegin(i, nullptr);
          auto r_end = (*this).cend(i, nullptr);
          auto c_beg = met_.cbegin(nullptr, j);
          multiply_two_sequences_to(r_beg, r_end, c_beg, p_.begin());
          // why?
          // std::accumulate(p_.cbegin(), p_.cend(), res_[{i, j}]);
          for (auto &pp : p_) {
            res_[{i, j}] += pp;
          }
        }
      }
      return res_;
    }
  }
  /**
   * scalar multuply
   */
  Matrix operator*(const T scl_) const noexcept {
    auto res_ = *this;
    auto f = [&scl_](reference t) { t *= scl_; };
    std::for_each(res_.begin(), res_.end(), f);
    return res_;
  }
  /**
   * Matrix substraction
   * notice: not symmetric
   * @exception
   */
  Matrix operator-(const Matrix &met_) const {
    if (!is_identical_size(*this, met_))
      throw size_mismatch("during Martix addition, size mismatch");
    else {
      auto res_ = *this;
      minus_sequence_from(met_.beg, met_.ed, res_.beg);
      return res_;
    }
  }
  /**
   * scalar substraction
   * notice: not symmetric
   */
  Matrix operator-(const T scl_) const noexcept {
    auto res_ = *this;
    auto f = [&scl_](reference t) { t -= scl_; };
    std::for_each(res_.begin(), res_.end(), f);
    return res_;
  }
  /**
   * overloaded subscrption operator
   * @param std::pair<T,T>
   */
  reference operator[](const std::pair<size_type, size_type> &pos_) noexcept {
    return access(pos_.first, pos_.second);
  }
  /**
   * overloaded subscrption operator for const
   * @param std::pair<T,T>
   */
  const_reference operator[](const std::pair<size_type, size_type> &pos_) const
      noexcept {
    return access(pos_.first, pos_.second);
  }

  ///------------------------------------
  /// other operations
  ///------------------------------------
  /**
   * transpose
   * @exception
   */
  Matrix trans() { return do_trans(); }
  /**
   * transpose for const
   * @exception
   */
  const Matrix trans() const { return do_trans(); }
  /**
   * @exception std::out_of_range
   */
  reference at(const size_type i_, const size_type j_) {
    if (i_ > row_length || j_ > col_length)
      throw std::out_of_range(
          "during access to element in Matrix, indice is out of range");
    return access(i_, j_);
  }
  /**
   * @exception std::out_of_range
   */
  const_reference at(const size_type i_, const size_type j_) const {
    if (i_ > row_length || j_ > col_length)
      throw std::out_of_range(
          "during access to element in Matrix, indice is out of range");
    return access(i_, j_);
  }
  /**
   * @return const size_type
   */
  const std::pair<size_type, size_type> size_pair() const noexcept {
    return {row_length, col_length};
  }

  /**
   * @return std::pair<size_type, size_type>
   */
  const size_type size() const noexcept { return row_length * col_length; }
  /**
   * @return std::pair<size_type, size_type>
   */
  const size_type max_size() const noexcept { return m_size; }
  /**
   * resize the Matrix
   * @exception std::bad_alloc
   */
  Matrix &resize(const size_type i_, const size_type j_) {
    return do_resize(i_, j_);
  }
  /**
   * resize the Matrix
   * @exception std::bad_alloc
   */
  Matrix &resize(const std::pair<size_type, size_type> sz_) {
    return do_resize(sz_.first, sz_.second);
  }

  /**
   * add rows to currenct metrix, resize if room is not enough
   * @return additional_info for the users to assign value
   */
  std::pair<iterator, iterator> add_row_back() {
    secure_space(row_length + 1, col_length);
    for (size_type i = 0; i < col_length; ++i) {
      ator.construct(ed++, 0);
    }
    ++row_length;
    return std::make_pair(begin(0, nullptr), end(0, nullptr));
  }
  std::pair<iterator, iterator> add_row_front() {
    secure_space(row_length + 1, col_length);
    auto t = ed;
    for (size_type i = 0; i < col_length; ++i) {
      ator.construct(ed++);
    }
    ++row_length;
    std::copy_backward(beg, t, ed);
    for (size_type i = 0; i < col_length; ++i) {
      at(0, i) = 0;
    }
    return std::make_pair(begin(row_length - 1, nullptr),
                          end(row_length - 1, nullptr));
  }

  std::pair<iterator, iterator> add_row_after(const size_type row_num_) {
    secure_space(row_length + 1, col_length);
    auto which = row_num_ + 2;
    auto diff = which * col_length;
    auto t = ed;
    for (size_type i = 0; i < col_length; ++i) {
      ator.construct(ed++);
    }
    ++row_length;
    std::copy_backward(beg + diff, t, ed);
    for (size_type i = 0; i < col_length; ++i) {
      at(which, i) = 0;
    }
    return std::make_pair(begin(which, nullptr), end(which, nullptr));
  }
  /**
   * add a col to current metrix, resize
   * @return additional_infoMatrix<T>:: for the users to assign
   * Matrix<T>::Matrix<T>::value
   */
  std::pair<column_iterator, column_iterator> add_col_back() {
    secure_space(row_length, col_length + 1);
    for (size_type i = 0; i < row_length; ++i) {
      ator.construct(ed++);
    }
    for (size_type i = row_length; i > 0; --i) {
      auto which_row_ = i - 1;
      std::copy_backward(begin(which_row_, nullptr), end(which_row_, nullptr),
                         end(which_row_, nullptr) + which_row_);
    }
    ++col_length;
    for (auto b_ = begin(nullptr, col_length - 1),
              e_ = end(nullptr, col_length - 1);
         b_ != e_; ++b_) {
      *b_ = 0;
    }
    return std::make_pair(begin(nullptr, 0), end(nullptr, 0));
  }

  std::pair<column_iterator, column_iterator> add_col_front() {
    secure_space(row_length, col_length + 1);
    for (size_type i = 0; i < row_length; ++i) {
      ator.construct(ed++);
    }
    for (size_type i = row_length; i > 0; --i) {
      auto which_row_ = i - 1;
      std::copy_backward(begin(which_row_, nullptr), end(which_row_, nullptr),
                         end(which_row_, nullptr) + i);
    }
    ++col_length;
    for (auto b_ = begin(nullptr, 0), e_ = end(nullptr, 0); b_ != e_; ++b_) {
      *b_ = 0;
    }
    return std::make_pair(begin(nullptr, col_length - 1),
                          end(nullptr, col_length - 1));
  }

  std::pair<column_iterator, column_iterator> add_col_after(
      const size_type col_num_) {
    secure_space(row_length, col_length + 1);
    add_col_back();
    auto which = col_num_ + 2;
    for (size_type i = 0; i < row_length; ++i) {
      std::copy_backward(begin(i, nullptr) + which, end(i, nullptr) - 1,
                         end(i, nullptr));
    }
    for (auto b_ = begin(nullptr, which), e_ = end(nullptr, which); b_ != e_;
         ++b_) {
      *b_ = 0;
    }
    return std::make_pair(begin(nullptr, which), end(nullptr, which));
  }

  void del_row_back() {
    for (size_type i = 0; i < col_length; ++i) {
      ator.destroy(--ed);
    }
    --row_length;
  }

  void del_row_front() {
    std::copy(begin() + col_length, end(), begin());
    for (size_type i = 0; i < col_length; ++i) {
      ator.destroy(--ed);
    }
    --row_length;
  }

  void del_row_after(const size_type row_num_) {
    auto which = row_num_ + 2;
    std::copy(begin() + col_length * which, end(),
              begin() + col_length * (which - 1));
    for (size_type i = 0; i < col_length; ++i) {
      ator.destroy(--ed);
    }
    --row_length;
  }

  void del_col_back() {
    for (size_type i = 1; i < row_length; ++i) {
      std::copy(begin(i, nullptr), end(i, nullptr) - 1, begin(i, nullptr) - i);
    }
    for (size_type i = 0; i < row_length; ++i) {
      ator.destroy(ed--);
    }
    --col_length;
  }

  void del_col_front() {
    for (size_type i = 0; i < row_length; ++i) {
      std::copy(begin(i, nullptr) + 1, end(i, nullptr), begin(i, nullptr) - i);
    }
    for (size_type i = 0; i < row_length; ++i) {
      ator.destroy(ed--);
    }
    --col_length;
  }
  // may be faster
  void del_col_after(const size_type col_num_) {
    auto which = col_num_ + 2;
    for (size_type i = 0; i < row_length; ++i) {
      std::copy(begin(i, nullptr) + which, end(i, nullptr),
                begin(i, nullptr) + which - 1);
    }
    del_col_back();
  }

  ///------------------------------------
  /// operations related to iterator
  ///------------------------------------
  /// iterator based on row
  /**
   * end() and ++
   * @return an iterator go through the whole Matrix
   */
  iterator begin() noexcept { return beg; }
  const_iterator begin() const noexcept { return beg; }
  const_iterator cbegin() const noexcept { return beg; }
  reverse_iterator rbegin() noexcept { return std::reverse_iterator<T>(beg); }
  const_reverse_iterator rbegin() const noexcept {
    return std::reverse_iterator<T>(beg);
  }
  const_reverse_iterator crbegin() const noexcept {
    return std::reverse_iterator<T>(beg);
  }
  /**
   * @see begin()
   */
  iterator end() noexcept { return ed; }
  const_iterator end() const noexcept { return ed; }
  const_iterator cend() const noexcept { return ed; }
  reverse_iterator rend() noexcept { return std::reverse_iterator<T>(ed); }
  const_reverse_iterator rend() const noexcept {
    return std::reverse_iterator<T>(ed);
  }
  const_reverse_iterator crend() noexcept {
    return std::reverse_iterator<T>(ed);
  }
  /**
   * @param nu would no be used, it is a way to indicate that the begginning is
   * i_ row. into an overloaded set
   */
  iterator begin(size_type row_, void *nu) { return beg + col_length * row_; }
  const_iterator begin(size_type row_, void *nu) const {
    return beg + col_length * row_;
  }
  const_iterator cbegin(size_type row_, void *nu) const {
    return beg + col_length * row_;
  }
  /**
   * @param nu would no be used, it is a way to put different kinds of iterators
   * into an overloaded set into an overloaded set
   */
  iterator end(size_type row_, void *nu) {
    return beg + col_length * (row_ + 1);
  }
  const_iterator end(size_type row_, void *nu) const {
    return beg + col_length * (row_ + 1);
  }
  const_iterator cend(size_type row_, void *nu) const {
    return beg + col_length * (row_ + 1);
  }
  /// iterator based on col
  /**
   * @param nu would no be used
   * @see begin(void *nu, size_type row_);
   */
  column_iterator begin(void *nu, size_type col_) {
    return col_iterator<T>(size_pair(), beg + col_);
  }
  const_column_iterator begin(void *nu, size_type col_) const {
    return col_iterator<T>(size_pair(), beg + col_);
  }
  const_column_iterator cbegin(void *nu, size_type col_) const {
    return col_iterator<T>(size_pair(), beg + col_);
  }
  /**
   * @see end(void *nu, size_type row_)
   */
  column_iterator end(void *nu, size_type col_) {
    return col_iterator<T>(size_pair(), beg + col_ + row_length * col_length);
  }
  const_column_iterator end(void *nu, size_type col_) const {
    return col_iterator<T>(size_pair(), beg + col_ + row_length * col_length);
  }
  const_column_iterator cend(void *nu, size_type col_) const {
    return col_iterator<T>(size_pair(), beg + col_ + row_length * col_length);
  }

  /**
   * iterator based on column
   */
  column_iterator begin_on_col() noexcept { return begin(nullptr, 0); }
  const_column_iterator begin_on_col() const noexcept {
    return begin(nullptr, 0);
  }
  const_column_iterator cbegin_on_col() const noexcept {
    return begin(nullptr, 0);
  }
  column_iterator end_on_col() noexcept { return end(nullptr, col_length - 1); }
  const_column_iterator end_on_col() const noexcept {
    return end(nullptr, col_length - 1);
  }
  const_column_iterator cend_on_col() const noexcept {
    return end(nullptr, col_length - 1);
  }

 private:
  /**
   * utility function for trans.
   * @see trans()
   * @exception
   */
  Matrix do_trans() const {
    Matrix res_((*this).col_length, (*this).row_length, 0);
    for (size_type i = 0; i < (*this).row_length; ++i) {
      for (size_type j = 0; j < (*this).col_length; ++j) {
        res_[{j, i}] = (*this)[{i, j}];
      }
    }
    return res_;
  }
  /**
   * utility function for access operation
   */
  reference access(size_type i_, size_type j_) noexcept {
    return beg[index_cal(i_, j_)];
  }
  /**
   * utility function for access operation with const
   */
  const_reference access(size_type i_, size_type j_) const noexcept {
    return beg[index_cal(i_, j_)];
  }
  /**
   * utility function for resize()
   */
  Matrix &do_resize(size_type i_, size_type j_) {
    if (i_ * j_ == size()) {
      row_length = i_;
      col_length = j_;
      return *this;
    }
    if (i_ < row_length) {
      while (i_ != row_length) {
        del_row_back();
      }
    }
    if (j_ < col_length) {
      while (j_ != col_length) {
        del_col_back();
      }
    }
    if (j_ > col_length) {
      while (j_ != col_length) {
        add_col_back();
      }
    }
    if (i_ > row_length) {
      while (i_ != row_length) {
        add_row_back();
      }
    }
    return *this;
  }

  const size_type proper_size(const size_type i_, const size_type j_) {
    return i_ * j_ * 2;
  }
  void secure_space(const size_type i_, const size_type j_) {
    if (i_ * j_ > size()) {
      auto t_beg = ator.allocate(proper_size(i_, j_));
      auto t_ed = t_beg + size();
      std::uninitialized_copy(beg, ed, t_beg);
      free_resource();
      beg = t_beg;
      ed = t_ed;
      m_size = proper_size(i_, j_);
    }
  }
  void free_resource() noexcept {
    while (beg != ed) {
      ator.destroy(--ed);
    }
    ator.deallocate(beg, m_size);
    m_size = 0;
  }

  /**
   * utility function for access(size_type, size_type) get proper indice
   */
  const size_type index_cal(size_type i_, size_type j_) const {
    return i_ * this->col_length + j_;
  }
  ///-------------END method------------------
  ///-------------attributes-----------------
  size_type row_length;
  size_type col_length;
  static std::allocator<T> ator;
  T *beg;
  T *ed;
  size_type m_size;
  ///-------------End attributes--------------
};

template <typename T>
std::allocator<T> Matrix<T>::ator = std::allocator<T>();

/**
 * post-condition: print the met into the file associated with out
 */
template <typename T>
std::ostream &operator<<(std::ostream &out, const Matrix<T> &met_) {
  unsigned int elem_num = met_.size();
  for (unsigned int i = 0; i < elem_num; i++) {
    if (!(i % met_.col_length) && i) out << std::endl;
    out << met_.beg[i];
    if (i != elem_num - 1) out << ' ';
  }
  return out;
}

/**
 * low efficiency
 */
template <typename T>
std::istream &operator>>(std::ifstream &in, Matrix<T> &met_) {
  static Matrix_reader<Matrix<T>> reader;
  met_ = reader.read(in);
  return in;
}

///-----------companion iterators-------

template <typename T>
void swap(col_iterator<T> &it1_, col_iterator<T> &it2_) noexcept {
  it1_.swap(it2_);
}

template <typename T>
col_iterator<T> operator+(const int dis_, const col_iterator<T> &it_) noexcept {
  return it_ + dis_;
}

template <typename T>
class col_iterator {
 public:
  friend Matrix<T>;
  template <typename U>
  friend void swp(col_iterator<U> &it1_, col_iterator<U> &it2_) noexcept;
  using size_type = typename Matrix<T>::size_type;
  using difference_type =
      typename std::iterator_traits<col_iterator<T>>::difference_type;
  using pointer = typename std::iterator_traits<col_iterator<T>>::pointer;
  using const_pointer = const pointer;
  using value_type = typename std::iterator_traits<col_iterator<T>>::value_type;
  using const_value_type = const value_type;
  using reference = typename std::iterator_traits<col_iterator<T>>::reference;
  using const_reference = const reference;

  col_iterator(const std::pair<size_type, size_type> size_info_,
               T *ptr_) noexcept
      : ptr(ptr_), row_size(size_info_.first), col_size(size_info_.second) {}

  col_iterator(const col_iterator &) noexcept = default;
  col_iterator(col_iterator &&) noexcept = default;
  virtual ~col_iterator() = default;

  void swap(col_iterator &it_) {
    using std::swap;

    swap(ptr, it_.ptr);
    swap(row_size, it_.row_size);
    swap(col_size, it_.col_size);
  }

  col_iterator &operator=(col_iterator it_) noexcept { swap(it_); }
  /// input && output iterator part

  col_iterator &operator++() noexcept {
    this->ptr += this->col_size;
    return (*this);
  }
  col_iterator operator++(int)noexcept {
    auto temp = *this;
    ++*this;
    return temp;
  }
  reference operator*() noexcept { return *ptr; }
  const_reference operator*() const noexcept { return *ptr; }
  pointer operator->() noexcept { return ptr; }
  const_pointer operator->() const noexcept { return ptr; }

  /// forward iterator part
  col_iterator() noexcept : ptr(nullptr), row_size(0), col_size(0) {}
  bool operator==(const col_iterator &it_) const noexcept {
    return ptr == it_.ptr;
  }
  bool operator!=(const col_iterator &it_) const noexcept {
    return !(it_ == *this);
  }
  /// bidirectional Iterator

  col_iterator &operator--() noexcept {
    this->ptr -= this->col_size;
    return (*this);
  }
  col_iterator operator--(int)noexcept {
    auto temp = *this;
    --*this;
    return temp;
  }

  /// random access iterator
  bool operator<(const col_iterator &it_) const noexcept {
    return ptr - it_.ptr < 0;
  }
  bool operator<=(const col_iterator &it_) const noexcept {
    return *this < it_ || *this == it_;
  }
  bool operator>(const col_iterator &it_) const noexcept {
    return !(*this <= it_);
  }
  bool operator>=(const col_iterator &it_) const noexcept {
    return !(*this < it_);
  }

  col_iterator operator+(const int dis_) const noexcept {
    auto temp = *this;
    temp += dis_;
    return temp;
  }
  col_iterator &operator+=(const int dis_) noexcept {
    this->ptr += this->col_size * dis_;
    return (*this);
  }
  col_iterator operator-(const int dis_) const noexcept {
    auto temp = *this;
    temp -= dis_;
    return temp;
  }

  col_iterator &operator-=(const int dis_) noexcept {
    this->ptr -= this->col_size * dis_;
    return (*this);
  }

  difference_type operator-(const col_iterator &it_) const noexcept {
    return ptr - it_.ptr;
  }

  value_type operator[](const int pos_) noexcept { return *(ptr + pos_); }
  const_value_type operator[](const int pos_) const noexcept {
    return *(ptr + pos_);
  }

  /// others
  operator bool() const noexcept { return ptr; }

 private:
  ///-----------attributes-------------
  T *ptr;
  size_type row_size;
  size_type col_size;
  ///----------End attributes----------
};

#endif