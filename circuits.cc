#include "circuits.hh"

namespace CTS
{

std::ostream &operator <<( std::ostream &ost, const Edge &edge )
{
  double real_cur = MX::is_zero(edge.cur) ? 0.0 : edge.cur;

  ost << edge.junc1 + 1 << " -- " << edge.junc2 + 1 << ": " << real_cur << " A" << std::endl;

  return ost;
}

Circuit::Circuit(const std::vector<Edge> &edges, size_t j_num) : edges_(edges), incidence_(j_num, edges_.size())
{
  size_t e_num = edges_.size();

  for (size_t i = 0; i < e_num; ++i)
  {
    incidence_.set(edges[i].junc1, i, 1);
    incidence_.set(edges[i].junc2, i, -1);
  }
}

MX::Matrix<double> Circuit::make_res_matr()
{
  size_t e_num = edges_.size();
  MX::Matrix<double> R{e_num, e_num};

  for (size_t i = 0; i < e_num; ++i)
    R.set(i, i, edges_[i].rtor);

  return R;
}

MX::Matrix<double> Circuit::make_eds_matr()
{
  size_t e_num = edges_.size();
  MX::Matrix<double> E{e_num, 1};

  for (size_t i = 0; i < e_num; ++i)
    E.set(i, 0, edges_[i].eds);

  return E;
}

bool Circuit::is_cyc_unique(const std::vector<int> &vec, const std::vector<std::vector<int>> &cyc_vec)
{
  for (auto &&cyc : cyc_vec)
  {
    size_t i = 0;
    for (size_t endi = cyc.size(); i < endi; ++i)
      if (std::abs(cyc[i]) != std::abs(vec[i]))
        break;
    if (i == cyc.size())
      return false;
  }

  return true;
}

MX::Matrix<double> Circuit::make_circ_matr()
{
  size_t inc_rows = incidence_.rows();
  size_t cycles_needed = incidence_.cols() - inc_rows + 1;

  std::vector<std::vector<int>> cycles;

  // go from all
  for (size_t i = 0; i < inc_rows && cycles.size() != cycles_needed; ++i)
  {
    auto tmp_vec = dfs_start(i);

    if (!is_cyc_unique(tmp_vec, cycles))
      continue;

    if (!tmp_vec.empty())
      cycles.push_back(tmp_vec);
  }

  MX::Matrix<double> circ{cycles.size(), edges_.size(),
                          [&cycles](int circ_num, int edge_num) { return cycles[circ_num][edge_num]; }};

  return circ;
}

std::vector<int> Circuit::dfs_start(size_t from)
{
  std::vector<int> tmp{};
  tmp.resize(edges_.size());
  std::vector<Color> cols(edges_.size(), Color::WHITE);

  dfs(from, from, from, tmp, cols);
  return tmp;
}

bool Circuit::dfs(size_t nstart, size_t nactual, size_t nprev, std::vector<int> &cyc_rout, std::vector<Color> &colors)
{
  /* Mark that now we are at this vert */
  colors[nactual] = Color::GREY;

  for (size_t i = 0, edg_size = edges_.size(); i < edg_size; ++i)
  {
    size_t dest_vert = edges_[i].junc1 == nactual ? edges_[i].junc2 : edges_[i].junc1;

    /* Check if we came from this vert */
    if (dest_vert == nprev)
      continue;

    /* Check if we have a route between verts */
    if (incidence_[nactual][i] == 0)
      continue;

    /* Check if we have already visited this vert */
    if (colors[dest_vert] == Color::GREY)
    {
      if (dest_vert == nstart)
      {
        cyc_rout[i] = incidence_[nactual][i];
        return true;
      }
      continue;
    }

    if (colors[dest_vert] == Color::WHITE)
    {
      std::vector<int> rout_cpy{cyc_rout};
      std::vector<Color> col_cpy{colors};

      rout_cpy[i] = incidence_[nactual][i];

      bool is_cycle = dfs(nstart, dest_vert, nactual, rout_cpy, col_cpy);

      if (is_cycle)
      {
        /* Means that we've found a cycle in previous dfs call */
        cyc_rout = rout_cpy;
        return true;
      }
    }
  }

  /* Mark that we go away from vert. */
  colors[nactual] = Color::BLACK;

  return false;
}

void Circuit::curs_calc()
{
  MX::Matrix<double> cut_inc{incidence_.rows() - 1, incidence_.cols(), 
  [this](int i, int j)
  {
    return this->incidence_[i][j];
  }
  };

  auto A_0 = MX::glue_side(cut_inc, MX::Matrix<double>{cut_inc.rows(), 1});
  auto B = make_circ_matr();

  auto BR = B * make_res_matr();
  auto BE = B * make_eds_matr(); 

  auto BR_BE = MX::glue_side(BR, BE);

  auto system = MX::glue_bott(A_0, BR_BE);

  // TODO: delete after debug
  std::cout << system;

  auto curs = MX::Matrix<double>::solve(system);

  for (size_t i = 0, endi = curs.size(); i < endi; ++i)
    edges_[i].cur = curs[i];
}
} // namespace CTS