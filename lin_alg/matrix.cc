#include "matrix.hh"

namespace MX
{
template <> std::ostream &operator<<(std::ostream &ost, const Matrix<double> &matr)
{
  ost << "   | ";
  for (size_t i = 0, cols = matr.cols(); i < cols; ++i)
    ost << std::setw(4) << i;

  ost << std::endl;

  ost << "   +-";
  for (size_t i = 0, cols = matr.cols(); i < cols; ++i)
    ost << "----";

  ost << std::endl;

  for (size_t i = 0, cols = matr.cols(), rows = matr.rows(); i < rows; ++i)
  {
    ost << std::setw(3) << i << "| ";

    for (size_t j = 0; j < cols; ++j)
    {
      auto tmp = matr[i][j];

      if (is_zero(tmp))
        ost << std::setw(3) << 0 << ";";
      else
        ost << std::setw(3) << tmp << ";";
    }

    ost << "|" << std::endl;
  }

  std::cerr << "[";
  for (size_t i = 0; i < matr.rows(); ++i)
  {
    std::cerr << "[";
    for (size_t j = 0; j < matr.cols(); ++j)
      std::cerr << matr[i][j] << ((j == matr.cols() - 1) ? "" : ",");

    std::cerr << "]" << ((i == matr.rows() - 1) ? "" : ",") << std::endl;
  }


  std::cerr << "]";

  return ost;
}

bool is_zero(long double num)
{
  return std::abs(num) < EPS;
}
} // namespace MX