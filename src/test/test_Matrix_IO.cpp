#include <fstream>
#include "../Matrix.hpp"

int main() {
  Matrix<int> mat;
  std::ifstream in("mat.mt");
  in >> mat;
  std::cout << mat;
}