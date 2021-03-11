#ifndef MATRIX_LA_HH
#define MATRIX_LA_HH

#include "matrix.hh"
#include <vector>

namespace LA
{
using namespace MX;

class Sys : public Matrix<double>
{
  using Matrix<double>::rows_;

public:
  Sys(const Matrix &orig) : Matrix(orig)
  {
  }
  Sys(size_t rows, size_t cols) : Matrix(rows, cols)
  {
  }
  Sys(size_t rows, size_t cols, const initializer_list<double> &dat) : Matrix(rows, cols, dat)
  {
  }
  template <typename It> Sys(size_t rows, size_t cols, It beg, It end) : Matrix(rows, cols, beg, end)
  {
  }
  template <typename Func> Sys(size_t rows, size_t cols, Func action) : Matrix(rows, cols, action)
  {
  }

  std::vector<double> solve() const;
};

std::vector<double> Sys::solve() const
{
  Sys tmp{*this};

  tmp.GaussFWD();
  tmp.GaussBWD();

  std::vector<double> res;
  res.reserve(rows_);

  for (size_t i = 0, col_idx = cols_ - 1; i < rows_; ++i)
    res.push_back(tmp[i][col_idx] / tmp[i][i]);

  return res;
}
} // namespace LA

#endif // MATRIX_LA_HH
