#ifndef CIRC_LA_HH
#define CIRC_LA_HH

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
  Sys(size_t rows, size_t cols, const initializer_list<double> &dat) : Matrix(rows, cols, dat)
  {
  }
  template <typename It> Sys(size_t rows, size_t cols, It beg, It end) : Matrix(rows, cols, beg, end)
  {
  }
  template <typename Func> Sys(size_t rows, size_t cols, Func action) : Matrix(rows, cols, action)
  {
  }

  /**
   * @brief Solving system as augmented system's matrix. Solves only solvable systems
   * @return Vector of solutions
   */
  std::vector<double> solve() const;
};
} // namespace LA

#endif // CIRC_LA_HH
