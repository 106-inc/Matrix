#ifndef CIRC_CIRCUITS_HH
#define CIRC_CIRCUITS_HH

#include "../lin_alg/matrix.hh"
#include <fstream>
#include <cstdlib>
#include <unordered_set>

// TODO: найти где будут Edge

namespace CTS
{
struct Edge final // rtor, junc1, junc2, voltage
{
  size_t junc1, junc2;

  double rtor, eds, cur;

  Edge(size_t j1, size_t j2, double rtr, double ed) : junc1(j1), junc2(j2), rtor(rtr), eds(ed)
  {
  }

  double get_cur() const
  {
    return MX::is_zero(cur) ? 0.0 : cur;
  }
};

std::ostream &operator<<(std::ostream &ost, const Edge &edge);

/**
 * @brief Circuit class. Solves a circut
 */
class Circuit final
{
private:
  std::vector<Edge> edges_;
  MX::Matrix<int> incidence_;
  MX::Matrix<double> circs_;
  MX::Matrix<double> inc_cut_;

public:
  Circuit(const MX::Matrix<int> &inc, const MX::Matrix<double> &res, const MX::Matrix<double> &eds);

  /*
   * What here should locate:
   * 1) Calculating matrix B
   * 2) calculating currents
   */

  MX::Matrix<double> curs_calc();
  void curs_out() const
  {
    for (auto &&edge : edges_)
      std::cout << edge;
  }

  void dump(const std::string &png_file, const std::string &dot_file = "dump.dot") const;

private:
  std::unordered_set<size_t> edges_visited;
  size_t cycles_amount{};
  enum class Color
  {
    WHITE = 0,
    GREY,
    BLACK
  };

  MX::Matrix<double> make_eds_matr() const;
  MX::Matrix<double> make_res_matr() const;

  void fill_inc_cut(const std::unordered_set<size_t> &j_loops);

  void fill_circ_matr();

  bool dfs(size_t nstart, size_t nactual, size_t ecurr, std::vector<int> &cyc_rout, std::vector<Color> &colors);
  void dfs_start(size_t from);

  void insert_cycle(const std::vector<int> &cyc);

  bool check_n_insert(const std::vector<int> &vec);
  bool is_cyc_unique(const std::vector<int> &vec) const;
};
} // namespace CTS

#endif // CIRC_CIRCUITS_HH
