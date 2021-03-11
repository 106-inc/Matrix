#ifndef MATRIX_LA_HH
#define MATRIX_LA_HH

#include "matrix.hh"
#include <vector>

/**
 * dfs(vert)
 * {
 *  color[vert] = grey; & put to matrix
 *
 *  for sbvert in vert.sub
 *  {
 *    if (color[sbvert] == white)
 *      dfs(sbvert)
 *    if (color[sbvert] == grey)
 *
 *  }
 * }
 *
 */
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
  Sys(size_t rows, size_t cols, func action) : Matrix(rows, cols, action)
  {
  }
  Sys(size_t rows, size_t cols, const initializer_list<double> &dat) : Matrix(rows, cols, dat)
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
