#ifndef MATRIX_CIRCUITS_HH
#define MATRIX_CIRCUITS_HH

#include <vector>

//#include "lin_alg/la.hh"

// TODO: найти где будут Edge

namespace CTS
{
struct Edge final // rtor, junc1, junc2, voltage
{
  size_t junc1, junc2;

  double rtor, eds, cur /** May be deleted in future */;

  Edge(size_t junc1_, size_t junc2_, double rtor_, double eds_) : junc1(junc1_),
                                                                  junc2(junc2_),
                                                                  rtor(rtor_),
                                                                  eds(eds_)
  {}
};

/**
 * @brief Circuit class. Solves a circut
 */
class Circuit final
{
private:
  std::vector<Edge> edges_;
 // MX::Matrix<int> incidence_;

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

  //MX::Matrix<double> make_eds_matr();
 // MX::Matrix<double> make_res_matr();

  //MX::Matrix<int> make_circ_matr();

  void dfs( size_t nstart, size_t nactual, std::vector<Edge> &cyc_rout, std::vector<Color> &colors );
  std::vector<Edge> dfs_start( size_t from );
};
} // namespace CTS

#endif // MATRIX_CIRCUITS_HH
