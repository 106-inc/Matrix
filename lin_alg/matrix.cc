#include "matrix.hh"

namespace MX
{
template <> std::ostream &operator<<(std::ostream &ost, const Matrix<double> &matr)
{
  for (size_t i = 0, cols = matr.cols(), rows = matr.rows(); i < rows; ++i)
  {
    ost << "| ";

    for (size_t j = 0; j < cols; ++j)
    {
      auto tmp = matr[i][j];

      if (is_zero(tmp))
        ost << "0 ";
      else
        ost << tmp << " ";
    }

    ost << "|" << std::endl;
  }

  return ost;
}

bool is_zero(long double num)
{
  return std::abs(num) < EPS;
}
} // namespace MX