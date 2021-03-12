#include "circuits.hh"

namespace CTS
{

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
    E.set(i, 1, edges_[i].eds);

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
  tmp.reserve(edges_.size());
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
        return true;
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
  std::cout << "CIRC_MATR: \n" << make_circ_matr();
  std::cout << "EDS_MATR: \n" << make_eds_matr();
  std::cout << "RES_MATR: \n" << make_res_matr();
}
} // namespace CTS