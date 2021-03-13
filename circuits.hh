#ifndef CIRC_CIRCUITS_HH
#define CIRC_CIRCUITS_HH

#include "lin_alg/matrix.hh"

// TODO: найти где будут Edge

namespace CTS
{
struct Junc final
{
  size_t real, norm;

  Junc(size_t r, size_t n = 0) : real(r), norm(n)
  {}
};

struct Edge final // rtor, junc1, junc2, voltage
{
  Junc junc1, junc2;

  double rtor, eds, cur;

  Edge(size_t j1, size_t j2, double rtr, double ed) : junc1(j1), junc2(j2), rtor(rtr), eds(ed)
  {
  }
};

std::ostream &operator <<( std::ostream &ost, const Edge &edge );

/**
 * @brief Circuit class. Solves a circut
 */
class Circuit final
{
private:
  std::vector<Edge> edges_;
  MX::Matrix<double> incidence_;
  MX::Matrix<double> circs_;

public:
  Circuit(const std::vector<Edge> &edges, size_t j_num);

  /*
   * What here should locate:
   * 1) Calculating matrix B
   * 2) calculating currents
   */


  void curs_calc();
  void curs_out()
  {
    for (auto &&edge : edges_)
      std::cout << edge;
  }

private:
  enum class Color
  {
    WHITE = 0,
    GREY,
    BLACK
  };

  MX::Matrix<double> make_eds_matr() const;
  MX::Matrix<double> make_res_matr() const;

  void fill_circ_matr();

  bool dfs(size_t nstart, size_t nactual, size_t nprev, std::vector<int> &cyc_rout, std::vector<Color> &colors);
  std::vector<int> dfs_start(size_t from);

  void insert_cycle( size_t num, const std::vector<int> &cyc );

  bool is_cyc_unique(const std::vector<int> &vec) const;

  void dump();
};
} // namespace CTS

#endif // CIRC_CIRCUITS_HH
