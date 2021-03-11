#include "matrix.hh"

namespace LA
{
using namespace MX;

class MatrixLA : public Matrix<long double>
{
  MatrixLA &GaussFWD();
};

MatrixLA &MatrixLA::GaussFWD()
{
  long double sign = 1;

  Matrix<long double> tmp_dbl{rows_, cols_};
  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j)
      tmp_dbl.set(i, j, static_cast<long double>(arr_[i][j]));

  for (size_t i = 0; i < cols_; ++i)
  {
    bool zero_col = true;

    if (tmp_dbl[i][i] != 0) // TODO: EPSILON
      zero_col = false;
    else
      for (size_t j = i + 1; j < rows_; ++j)
        if (tmp_dbl[j][i] != 0)
        {
          tmp_dbl.swap_lines(j, i);
          zero_col = false;
          sign = -sign;
          break;
        }

    if (zero_col)
      return 0;

    for (size_t k = i + 1; k < rows_; ++k)
    {
      if (tmp_dbl[k][i] == 0) // TODO: EPSILON
        continue;

      long double mul = tmp_dbl[k][i] / tmp_dbl[i][i];
      tmp_dbl.add_line(k, i, -mul);
    }
  }

  long double res = sign;
  for (size_t i = 0; i < cols_; ++i)
    res *= tmp_dbl[i][i];

  return res;
}
} // namespace LA