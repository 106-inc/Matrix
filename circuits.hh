#ifndef MATRIX_CIRCUITS_HH
#define MATRIX_CIRCUITS_HH

#include <vector>

#include "lin_alg/la.hh"

// TODO: найти где будут Edge

namespace CTS
{
struct Edge final // rtor, junc1, junc2, voltage
{
  size_t junc1, junc2;

  double rtor, eds, cur /** May be deleted in future */;
};

/**
 * @brief Circuit class. Solves a circut
 */
class Circuit final
{
private:
  std::vector<Edge> edges_;
  MX::Matrix<int> incidence_;

public:
  Circuit( const std::vector<Edge> &edges, size_t j_num );

  /*
   * What here should locate:
   * 1) Calculating matrix B
   * 2) calculating currents
   */


  void curs_calc();

private:

  enum class Color
  {
    WHITE = 0, GREY, BLACK
  };

  MX::Matrix<double> make_eds_matr();
  MX::Matrix<double> make_res_matr();

  MX::Matrix<double> make_circ_matr();

  bool dfs( size_t nstart, size_t nactual, size_t nprev, std::vector<int> &cyc_rout, std::vector<Color> &colors );
  std::vector<int> dfs_start( size_t from );

  static bool is_cyc_unique( const std::vector<int> &vec, const std::vector<std::vector<int>> &cyc_vec );
};
} // namespace CTS

#endif // MATRIX_CIRCUITS_HH
