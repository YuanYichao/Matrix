#include <iostream>
#include <string>
#include <vector>
#include "../Matrix.hpp"

void test_block(std::string str_, void (*f)()) {
  std::cout << "Testing " << str_ << std::endl;
  f();
  std::cout << "Test Ended" << std::endl << std::endl;
}

void test_ctor_1() {
  Matrix<int> a(5);
  Matrix<int> b(2);
  std::cout << "a: [a(5)] \n" << a << std::endl;
  std::cout << "b: [b(2)] \n" << b << std::endl;
}

void test_ctor_2() {
  Matrix<int> c(3, 2, 2);
  Matrix<int> d(2, 2, 1);
  std::cout << "c: [a(3,2,2)] \n" << c << std::endl;
  std::cout << "d: [a(2,2,1)] \n" << d << std::endl;
}
void test_ctor_3() {
  int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::cout << "int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};" << std::endl;
  Matrix<int> e(arr1);
  std::cout << "e: e(arr1) \n" << e << std::endl;
}
void test_ctor_4() {
  int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::cout << "int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};" << std::endl;
  Matrix<int> e(3, 3, arr1);
  std::cout << "e: e(3, 3, arr1) \n" << e << std::endl;
}
void test_ctor_5() {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::cout << "std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};"
            << std::endl;
  Matrix<int> e(vec.cbegin(), vec.cend());
  std::cout << "e: e(vec.cbegin, vec.cend) \n" << e << std::endl;
}

void test_ctor_6() {
  Matrix<int> e = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::cout << "e: e = {1, 2, 3, 4, 5, 6, 7, 8, 9}; \n" << e << std::endl;
}
void test_ctor_7() {
  Matrix<int> a(5);
  Matrix<int> b = a;
  std::cout << "a: [a(5)] \n" << a << std::endl;
  std::cout << "b: b = a \n" << b << std::endl;
}
void test_ctor_8() {
  Matrix<int> a(5);
  Matrix<int> b = std::move(a);
  std::cout << "a: [a(5)] {after move: } \n" << a << std::endl;
  std::cout << "b: b = std::move(a) \n" << b << std::endl;
}

void test_op_assign() {
  Matrix<int> a(5);
  Matrix<int> b(2);
  std::cout << "a: [a(5)] \n" << a << std::endl;
  std::cout << "b: [b(2)] \n" << b << std::endl;
  std::cout << "assignment a=b" << std::endl << std::endl;
  a = b;
  std::cout << "a: [after] \n" << a << std::endl;
  std::cout << "b: [after] \n" << b << std::endl;
}

void test_mat_addition() {
  std::cout << "test without throw statement" << std::endl;
  Matrix<int> a(5, 6, 2);
  Matrix<int> b(5, 6, 3);
  std::cout << "a: \n" << a << std::endl;
  std::cout << "b: \n" << b << std::endl;
  std::cout << "c = a + b" << std::endl << std::endl;
  auto c = a + b;
  std::cout << "c: \n" << c << std::endl;

  std::cout << "test with throw statement" << std::endl;
  try {
    Matrix<int> a(5, 6, 2);
    Matrix<int> b(5, 5, 3);
    std::cout << "a: \n" << a << std::endl;
    std::cout << "b: \n" << b << std::endl;
    std::cout << "c = a + b" << std::endl << std::endl;
    auto c = a + b;
    std::cout << "c: \n" << c << std::endl;
  } catch (size_mismatch &e) {
    std::cout << "exception occured: \n" << e.what() << std::endl;
    std::cout << "type: size_mismatch" << std::endl;
  }
}

void test_sc_addition() {
  Matrix<int> a(5, 6, 2);
  std::cout << "a: \n" << a << std::endl;
  std::cout << "a = a + 1" << std::endl;

  std::cout << "a: [after]\n" << a + 1 << std::endl;
}

void test_mat_multiply() {
  std::cout << "test without throw statement" << std::endl;
  int arr1[12] = {3, 5, 2, 4, 6, 4, 8, 6, 22, 3, 1};
  int arr2[9] = {3, 6, 5, 7, 8, 1, 2, 9, 4};
  Matrix<int> a(4, 3, arr1);
  Matrix<int> b(3, 3, arr2);
  std::cout << "a: \n" << a << std::endl;
  std::cout << "b: \n" << b << std::endl;
  std::cout << "c = a * b" << std::endl << std::endl;
  auto c = a * b;
  std::cout << "c: \n" << c << std::endl;

  try {
    Matrix<int> a(5, 6, 2);
    Matrix<int> b(5, 5, 3);
    std::cout << "a: \n" << a << std::endl;
    std::cout << "b: \n" << b << std::endl;
    std::cout << "c = a * b" << std::endl << std::endl;
    auto c = a * b;
    std::cout << "c: \n" << c << std::endl;
  } catch (size_mismatch &e) {
    std::cout << "exception occured: \n" << e.what() << std::endl;
    std::cout << "type: size_mismatch" << std::endl;
  }
}

void test_sc_multiply() {
  Matrix<int> a(5, 6, 2);
  std::cout << "a: \n" << a << std::endl;
  std::cout << "a = a * 2" << std::endl;

  std::cout << "a: [after]\n" << a * 2 << std::endl;
}

void test_mat_subtraction() {
  std::cout << "test without throw statement" << std::endl;
  Matrix<int> a(5, 6, 2);
  Matrix<int> b(5, 6, 3);
  std::cout << "a: \n" << a << std::endl;
  std::cout << "b: \n" << b << std::endl;
  std::cout << "c = a - b" << std::endl << std::endl;
  auto c = a - b;
  std::cout << "c: \n" << c << std::endl;

  std::cout << "test with throw statement" << std::endl;
  try {
    Matrix<int> a(5, 6, 2);
    Matrix<int> b(5, 5, 3);
    std::cout << "a: \n" << a << std::endl;
    std::cout << "b: \n" << b << std::endl;
    std::cout << "c = a - b" << std::endl << std::endl;
    auto c = a - b;
    std::cout << "c: \n" << c << std::endl;
  } catch (size_mismatch &e) {
    std::cout << "exception occured: \n" << e.what() << std::endl;
    std::cout << "type: size_mismatch" << std::endl;
  }
}

void test_sc_subtraction() {
  Matrix<int> a(5, 6, 2);
  std::cout << "a: \n" << a << std::endl;
  std::cout << "a = a - 1" << std::endl;

  std::cout << "a: [after]\n" << a - 1 << std::endl;
}

void test_subscrption() {
  Matrix<int> a(5, 6, 2);
  const Matrix<int> b(5, 6, 3);
  std::cout << "a: \n" << a << std::endl;
  std::cout << "b: [const] \n" << b << std::endl;
  auto bound = a.size_pair();
  std::cout << "start accessing" << std::endl;
  std::cout << "for a:" << std::endl;
  for (unsigned int i = 0; i < bound.first; i++) {
    for (unsigned int j = 0; j < bound.second; j++) {
      std::cout << a[{i, j}] << ' ';
    }
    std::cout << std::endl;
  }
  std::cout << "for b:" << std::endl;
  for (unsigned int i = 0; i < bound.first; i++) {
    for (unsigned int j = 0; j < bound.second; j++) {
      std::cout << b[{i, j}] << ' ';
    }
    std::cout << std::endl;
  }
}

void test_trans() {
  Matrix<int> a(5, 6, 2);
  const Matrix<int> b(5, 6, 3);
  std::cout << "a: \n" << a << std::endl;
  std::cout << "b: [const] \n" << b << std::endl << std::endl;
  std::cout << "c = a.trans()" << std::endl;
  auto c = a.trans();
  std::cout << c << std::endl;
  std::cout << "d = b.trans()" << std::endl;
  auto d = b.trans();
  std::cout << d << std::endl;
}

void test_row() {
  Matrix<int> a(5, 6, 2);
  std::cout << "a: \n" << a << std::endl;
  a.add_row_back();
  std::cout << "a.add_row_back(): \n" << a << std::endl;
  a.add_row_front();
  std::cout << "a.add_row_front(): \n" << a << std::endl;
  a.add_row_after(2);
  std::cout << "a.add_row_after(2): \n" << a << std::endl;
  a.del_row_back();
  std::cout << "a.del_row_back(): \n" << a << std::endl;
  a.del_row_front();
  std::cout << "a.del_row_front(): \n" << a << std::endl;
  a.del_row_after(2);
  std::cout << "a.del_row_after(2): \n" << a << std::endl;
}

void test_col() {
  Matrix<int> a(5, 6, 2);
  std::cout << "a: \n" << a << std::endl;
  a.add_col_back();
  std::cout << "a.add_col_back(): \n" << a << std::endl;
  a.add_col_front();
  std::cout << "a.add_col_front(): \n" << a << std::endl;
  a.add_col_after(2);
  std::cout << "a.add_col_after(2): \n" << a << std::endl;
  a.del_col_back();
  std::cout << "a.del_col_back(): \n" << a << std::endl;
  a.del_col_front();
  std::cout << "a.del_col_front(): \n" << a << std::endl;
  a.del_col_after(2);
  std::cout << "a.del_col_after(2): \n" << a << std::endl;
}

void test_resize() {
  Matrix<int> a(5, 6, 2);
  std::cout << "a: \n" << a << std::endl;
  std::cout << "c = a.resize(6,6)" << std::endl;
  auto c = a.resize(6, 6);
  std::cout << c << std::endl;
  std::cout << "d = a.resize(5,5)" << std::endl;
  auto d = a.resize(5, 5);
  std::cout << d << std::endl;
  std::cout << "e = a.resize(4,5)" << std::endl;
  auto e = a.resize(4, 5);
  std::cout << e << std::endl;
  std::cout << "f = a.resize(6,5)" << std::endl;
  auto f = a.resize(6, 5);
  std::cout << f << std::endl;
  std::cout << "g = a.resize(12,5)" << std::endl;
  auto g = a.resize(12, 5);
  std::cout << g << std::endl;
}

int main() {
  std::cout << "Start Testing constructors" << std::endl << std::endl;
  test_block("Matrix(sz_)", test_ctor_1);
  test_block("Matrix(i,j,val)", test_ctor_2);
  test_block("Matrix(const T (&arr_)[C])", test_ctor_3);
  test_block(
      "Matrix(const size_type i_, const size_type j_, const T (&arr_)[C]) ",
      test_ctor_4);
  test_block("Matrix(const IT beg_, const IT end_)", test_ctor_5);
  test_block("Matrix(const std::initializer_list<T> in_)", test_ctor_6);
  test_block("Matrix(const Matrix &met_)", test_ctor_7);
  test_block("Matrix(Matrix &&met_)", test_ctor_8);
  std::cout << "Start Tests of constructors Ended" << std::endl << std::endl;

  std::cout << "Start Testing overloaded operators" << std::endl << std::endl;
  test_block("assignment operator", test_op_assign);

  test_block("mat operator+", test_mat_addition);
  test_block("scalar operator+", test_sc_addition);

  test_block("mat operator*", test_mat_multiply);
  test_block("scalar operator*", test_sc_multiply);

  test_block("mat operator-", test_mat_subtraction);
  test_block("scalar operator-", test_sc_subtraction);

  test_block("operator[]", test_subscrption);
  std::cout << "Tests of overloaded operators Ended" << std::endl << std::endl;

  std::cout << "Start Testing other operations" << std::endl << std::endl;
  test_block("trans()", test_trans);

  test_block("add/del row", test_row);

  test_block("add/del col", test_col);

  test_block("resize()", test_resize);

  std::cout << "Tests of other operations Ended" << std::endl << std::endl;
}