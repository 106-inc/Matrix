#include "la.hh"
#include "matrix.hh"
#include <cstdint>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

using std::vector;
using namespace MX;

int main()
{
  uint cols = 0, rows = 0;
  cin >> rows >> cols;

  uint el_num = cols * rows;

  vector<double> raw{};
  raw.reserve(el_num);

  for (int i = 0, end = el_num; i < end; ++i)
  {
    double tmp;
    cin >> tmp;
    raw.push_back(tmp);
  }

  Matrix<double> matr{rows, cols, raw.begin(), raw.end()};

  cout << matr << endl;

  cout << matr.GaussFWD() << endl;

  //cout << matr.GaussBWD() << endl << endl;

  LA::Sys system{matr};

  //auto solution = system.solve();

  //for (auto &&el : solution)
  //  cout << el << " ";

  cout << endl;

  return 0;
}