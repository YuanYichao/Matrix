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
class cr_iterator_base;

template <typename T>
struct std::iterator_traits<cr_iterator_base<T>> {
  using difference_type = ptrdiff_t;
  using pointer = T *;
  using value_type = T;
  using reference = T &;
  using iterator_category = std::random_access_iterator_tag;
};

template <typename T>
class row_iterator;

template <typename T>
struct std::iterator_traits<row_iterator<T>> {
  using difference_type = ptrdiff_t;
  using pointer = T *;
  using value_type = T;
  using reference = T &;
  using iterator_category = std::random_access_iterator_tag;
};

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

class Format_testor {
 public:
  static Format_testor Init_testor() { return Format_testor(); }
  bool is_end(char ch_) { return ch_ == ';'; }
  bool is_next_row(char ch_) { return ch_ == '\n'; }

 private:
  Format_testor() = default;
};

/**
 * post-condition: print the met into the file associated with out
 */
template <typename T>
std::ostream &operator<<(std::ostream &out, const Matrix<T> &met_) {
  unsigned int elem_num = met_.size();
  for (int i = 0; i < elem_num; i++) {
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
std::istream &operator>>(std::istream &in, Matrix<T> &met_) {
  T val_;
  std::vector<T> temp;
  typename Matrix<T>::size_type w_ = 0, cnt_ = 0, h_ = 0;
  auto tor = Format_testor::Init_testor();

  while (!tor.is_end(in.get())) {
    in.unget();
    in >> val_;
    temp.push_back(val_);
    ++cnt_;
    if (!w_ && tor.is_next_row(in.get())) {
      w_ = cnt_;
      in.unget();
    } else {
      in.unget();
    }
  }

  h_ = cnt_ / w_;
  met_.col_length = w_;
  met_.row_length = h_;

  met_ = Matrix<T>(temp.begin(), temp.end());
}

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
  friend row_iterator<T>;
  friend col_iterator<T>;
  friend std::ostream &operator<<<>(std::ostream &out, const Matrix<T> &met_);
  friend std::istream &operator>><>(std::istream &in, Matrix<T> &met_);
  friend bool is_identical_size<>(const Matrix<T> &met1, const Matrix<T> &met2);
  friend bool is_mult_size<>(const Matrix<T> &met1, const Matrix<T> &met2);
  using size_type = size_t;
  using iterator = T *;
  using reference = T &;
  using const_reference = const reference;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<T *>;
  using const_reverse_iterator = const reverse_iterator;
  using row_iterator = row_iterator<T>;
  using const_row_iterator = const row_iterator;
  using col_iterator = col_iterator<T>;
  using const_col_iterator = const col_iterator;
  ///--------------End attributes------------
  ///--------------methods----------------------
  ///--------------------------------------------
  /// copy control, ctor, dtor
  ///--------------------------------------------
  /**
   * default constructor, create an unit Matrix
   */
  Matrix() : col_length(0), row_length(0), beg(nullptr), m_size(0), ed(beg) {}
  /**
   * constructor which creates an unit Matrix
   */
  Matrix(size_type sz_)
      : row_length(sz_),
        col_length(sz_),
        beg(ator.allocate(size() * 2)),
        m_size(size() * 2),
        ed(beg) {
    size_type pos_ = 0;
    for (int i = 0; i < size(); i++) {
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
        beg(ator.allocate(size() * 2)),
        m_size(size() * 2),
        ed(beg) {
    for (int i = 0; i < size(); i++) {
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
        beg(ator.allocate(C * 2)),
        m_size(C * 2),
        ed(beg) {
    for (int i = 0; i < C; i++) ator.construct(ed++, 0);
    std::copy(std::begin(arr_), std::end(arr_), beg);
  }
  /**
   * convert an array to Matrix with given size
   */
  template <size_t C>
  Matrix(const size_type i_, const size_type j_, const T (&arr_)[C])
      : row_length(i_),
        col_length(j_),
        beg(ator.allocate(size() * 2)),
        m_size(size() * 2),
        ed(beg) {
    for (int i = 0; i < size(); i++) ator.construct(ed++, 0);
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
        beg(ator.allocate(size() * 2)),
        m_size(size() * 2),
        ed(beg) {
    for (int i = 0; i < size(); i++) ator.construct(ed++, 0);
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
        m_size(met_.max_size()),
        ed(beg) {
    for (int i = 0; i < size(); i++) ator.construct(ed++, 0);
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
  Matrix &operator=(Matrix met_) { swap(met_); }
  /**
   * destructor
   */
  ~Matrix() {
    while (beg != ed) {
      ator.destroy(--ed);
    }
    ator.deallocate(beg, m_size);
  }
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
      for (int i = 0; i < res_.row_length; ++i) {
        for (int j = 0; j < res_.col_length; ++j) {
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

  ///------------------------------------
  /// operations related to iterator
  ///------------------------------------
  /// normal iterator
  /**
   * the sequence through which the iteration goes on is undefined
   * only guarantee that all elements in the met can be access through
 begin(),
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
  /// iterator based on row
  /**
   * @param nu would no be used, it is a way to put different kinds of iterators
   * into an overloaded set
   */
  row_iterator begin(size_type row_, void *nu) {
    return ::row_iterator<T>(size_pair(), beg + col_length * row_);
  }
  const_row_iterator begin(size_type row_, void *nu) const {
    return ::row_iterator<T>(size_pair(), beg + col_length * row_);
  }
  const_row_iterator cbegin(size_type row_, void *nu) const {
    return ::row_iterator<T>(size_pair(), beg + col_length * row_);
  }
  /**
   * @param nu would no be used, it is a way to put different kinds of iterators
   * into an overloaded set into an overloaded set
   */
  row_iterator end(size_type row_, void *nu) {
    return ::row_iterator<T>(size_pair(), beg + col_length * (row_ + 1));
  }
  const_row_iterator end(size_type row_, void *nu) const {
    return ::row_iterator<T>(size_pair(), beg + col_length * (row_ + 1));
  }
  const_row_iterator cend(size_type row_, void *nu) const {
    return ::row_iterator<T>(size_pair(), beg + col_length * (row_ + 1));
  }
  /// iterator based on col
  /**
   * @param nu would no be used
   * @see begin(void *nu, size_type row_);
   */
  col_iterator begin(void *nu, size_type col_) {
    return ::col_iterator<T>(size_pair(), beg + col_);
  }
  const_col_iterator begin(void *nu, size_type col_) const {
    return ::col_iterator<T>(size_pair(), beg + col_);
  }
  const_col_iterator cbegin(void *nu, size_type col_) const {
    return ::col_iterator<T>(size_pair(), beg + col_);
  }
  /**
   * @see end(void *nu, size_type row_)
   */
  col_iterator end(void *nu, size_type col_) {
    return ::col_iterator<T>(size_pair(), beg + col_ + row_length * col_length);
  }
  const_col_iterator end(void *nu, size_type col_) const {
    return ::col_iterator<T>(size_pair(), beg + col_ + row_length * col_length);
  }
  const_col_iterator cend(void *nu, size_type col_) const {
    return ::col_iterator<T>(size_pair(), beg + col_ + row_length * col_length);
  }

 private:
  /**
   * utility function for trans.
   * low efficiency
   * @see trans()
   * @exception
   */
  Matrix do_trans() const {
    Matrix res_((*this).col_length, (*this).row_length, 0);
    for (int i = 0; i < (*this).row_length; ++i) {
      for (int j = 0; j < (*this).col_length; ++j) {
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
   * low-efficiency
   */
  Matrix &do_resize(size_type i_, size_type j_) {
    Matrix res_(i_, j_, 0);
    auto row_ = i_ < row_length ? i_ : row_length;
    auto col_ = j_ < col_length ? j_ : col_length;
    for (int i = 0; i < row_; i++) {
      for (int j = 0; j < col_; j++) {
        res_[{i, j}] = (*this)[{i, j}];
      }
    }
    *this = std::move(res_);
    return *this;
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

///-----------companion iterators-------
template <typename T>
class cr_iterator_base;

template <typename T>
void swap(cr_iterator_base<T> &it1_, cr_iterator_base<T> &it2_) noexcept {
  it1_.swap(it2_);
}

template <typename T>
cr_iterator_base<T> operator+(const int dis_,
                              const cr_iterator_base<T> &it_) noexcept {
  return it_ + dis_;
}

template <typename T>
class cr_iterator_base {
 protected:
  friend Matrix<T>;
  friend void swap<>(cr_iterator_base<T> &it1_,
                     cr_iterator_base<T> &it2_) noexcept;
  using size_type = typename Matrix<T>::size_type;
  using difference_type =
      typename std::iterator_traits<cr_iterator_base<T>>::difference_type;
  using pointer = typename std::iterator_traits<cr_iterator_base<T>>::pointer;
  using const_pointer = const pointer;
  using value_type =
      typename std::iterator_traits<cr_iterator_base<T>>::value_type;
  using const_value_type = const value_type;
  using reference =
      typename std::iterator_traits<cr_iterator_base<T>>::reference;
  using const_reference = const reference;

  cr_iterator_base(const std::pair<size_type, size_type> size_info_,
                   T *ptr_) noexcept
      : ptr(ptr_), row_size(size_info_.first), col_size(size_info_.second) {}

  ///-----------attributes-------------
  T *ptr;
  size_type row_size;
  size_type col_size;
  ///----------End attributes----------

 public:
  cr_iterator_base(const cr_iterator_base &) noexcept = default;
  cr_iterator_base(cr_iterator_base &&) noexcept = default;
  virtual ~cr_iterator_base() = default;

  void swap(cr_iterator_base &it_) {
    using std::swap;

    swap(ptr, it_.ptr);
    swap(row_size, it_.row_size);
    swap(col_size, it_.col_size);
  }

  cr_iterator_base &operator=(cr_iterator_base it_) noexcept { swap(it_); }
  /// input && output iterator part

  /// operator++() noexcept
  /// operator++(int na) noexcept
  reference operator*() noexcept { return *ptr; }
  const_reference operator*() const noexcept { return *ptr; }
  pointer operator->() noexcept { return ptr; }
  const_pointer operator->() const noexcept { return ptr; }
  /// forward iterator part
  cr_iterator_base() noexcept : ptr(nullptr), row_size(0), col_size(0) {}
  bool operator==(const cr_iterator_base &it_) const noexcept {
    return ptr == it_.ptr;
  }
  bool operator!=(const cr_iterator_base &it_) const noexcept {
    return !(it_ == *this);
  }
  /// bidirectional Iterator

  /// &operator--() noexcept
  /// operator--() noexcept

  /// random access iterator
  bool operator<(const cr_iterator_base &it_) const noexcept {
    return ptr - it_.ptr < 0;
  }
  bool operator<=(const cr_iterator_base &it_) const noexcept {
    return *this < it_ || *this == it_;
  }
  bool operator>(const cr_iterator_base &it_) const noexcept {
    return !(*this <= it_);
  }
  bool operator>=(const cr_iterator_base &it_) const noexcept {
    return !(*this < it_);
  }

  // virtual operator+(const int dis_) const noexcept
  // virtual &operator+=(const int dis_) noexcept
  // virtual operator-(const int dis_) const noexcept
  // virtual &operator-=(const int dis_) noexcept

  difference_type operator-(const cr_iterator_base &it_) const noexcept {
    return ptr - it_.ptr;
  }

  value_type operator[](const int pos_) noexcept { return *(ptr + pos_); }
  const_value_type operator[](const int pos_) const noexcept {
    return *(ptr + pos_);
  }
  /// others
  operator bool() const noexcept { return ptr; }
};
/**
 * create an row_iterator through col_iterator
 */
template <typename T>
row_iterator<T> col_to_row(const col_iterator<T> &it_) noexcept {
  return row_iterator<T>({it_.row_size, it_.col_size}, it_.ptr);
};

template <typename T>
class col_iterator : public cr_iterator_base<T> {
 public:
  ///--------------attributes--------------------------
  friend Matrix<T>;
  friend row_iterator<T> col_to_row<>(const col_iterator<T> &it_) noexcept;
  ///--------------End attribues------------------------
  ///-------------methods-----------------------------
  using cr_iterator_base<T>::cr_iterator_base;
  col_iterator &operator++() noexcept {
    this->ptr += this->col_size;
    return (*this);
  }
  col_iterator operator++(int)noexcept {
    auto temp = *this;
    ++*this;
    return temp;
  }

  col_iterator &operator--() noexcept {
    this->ptr -= this->col_size;
    return (*this);
  }
  col_iterator operator--(int)noexcept {
    auto temp = *this;
    --*this;
    return temp;
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
  ///--------------End methods-------------------------
};

template <typename T>
col_iterator<T> row_to_col(const row_iterator<T> &it_) noexcept {
  return col_iterator<T>({it_.row_size, it_.col_size}, it_.ptr);
}

template <typename T>
class row_iterator : public cr_iterator_base<T> {
 public:
  ///--------------attributes--------------------------
  friend Matrix<T>;
  friend col_iterator<T> row_to_col<>(const row_iterator<T> &it_) noexcept;
  template <typename U>
  friend typename row_iterator<U>::difference_type operator-(
      const row_iterator<U> &it1_, const row_iterator<U> &it2_);
  ///--------------End attribues------------------------
  ///-------------methods-----------------------------
  using cr_iterator_base<T>::cr_iterator_base;
  row_iterator &operator++() noexcept {
    ++this->ptr;
    return *this;
  }
  row_iterator operator++(int)noexcept {
    auto t = *this;
    ++*this;
    return t;
  }

  row_iterator &operator--() noexcept {
    --this->ptr;
    return *this;
  }

  row_iterator operator--(int)noexcept {
    auto t = *this;
    --*this;
    return t;
  }

  row_iterator operator+(const int dis_) const noexcept {
    auto t = *this;
    t += dis_;
    return t;
  }
  row_iterator &operator+=(const int dis_) noexcept {
    this->ptr += dis_;
    return *this;
  }
  row_iterator operator-(const int dis_) const noexcept {
    auto t = *this;
    t -= dis_;
    return t;
  }
  row_iterator &operator-=(const int dis_) noexcept {
    this->ptr -= dis_;
    return *this;
  }

  ///--------------End methods-------------------------
};

// fix me
template <typename T>
typename row_iterator<T>::difference_type operator-(
    const row_iterator<T> &it1_, const row_iterator<T> &it2_) {
  return it1_.ptr - it2_.ptr;
}

#endif