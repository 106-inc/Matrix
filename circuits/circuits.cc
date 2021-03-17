#include "circuits.hh"

namespace CTS
{

std::ostream &operator<<(std::ostream &ost, const Edge &edge)
{
  double real_cur = MX::is_zero(edge.cur) ? 0.0 : edge.cur;

  ost << edge.junc1.real << " -- " << edge.junc2.real << ": " << real_cur << " A" << std::endl;

  return ost;
}

Circuit::Circuit(const std::vector<Edge> &edges, size_t j_num, const std::unordered_set<size_t> &j_loops)
    : edges_(edges), incidence_(j_num, edges_.size()),
      circs_(edges_.size(), edges_.size()), inc_cut_{j_num - j_loops.size(), edges_.size()}
{
  size_t e_num = edges_.size();

  for (size_t i = 0; i < e_num; ++i)
  {
    size_t norm1 = edges[i].junc1.norm, norm2 = edges[i].junc2.norm;

    incidence_.set(norm1, i, 1);
    incidence_.set(norm2, i, -1);
  }

  size_t cut_cnt = 0;
  for (size_t i = 0; i < incidence_.rows(); ++i)
  {
    if (j_loops.contains(i))
      continue;
    for (size_t j = 0; j < incidence_.cols(); ++j)
      inc_cut_.set(cut_cnt, j, incidence_[i][j]);
    ++cut_cnt;
  }
}

MX::Matrix<double> Circuit::make_res_matr() const
{
  size_t e_num = edges_.size();
  MX::Matrix<double> R{e_num, e_num};

  for (size_t i = 0; i < e_num; ++i)
    R.set(i, i, edges_[i].rtor);

  return R;
}

MX::Matrix<double> Circuit::make_eds_matr() const
{
  size_t e_num = edges_.size();
  MX::Matrix<double> E{e_num, 1};

  for (size_t i = 0; i < e_num; ++i)
    E.set(i, 0, edges_[i].eds);

  return E;
}

void Circuit::insert_cycle(size_t num, const std::vector<int> &cyc)
{
  for (size_t i = 0, endi = circs_.cols(); i < endi; ++i)
  {
    circs_.set(num, i, cyc[i]);
    if (cyc[i])
      edges_visited.insert(i);
  }
}

bool Circuit::is_cyc_unique(const std::vector<int> &vec) const
{
  for (size_t i = 0; i < vec.size(); ++i)
    if (vec[i] && !edges_visited.contains(i))
      return true;

  return false;
}

void Circuit::fill_circ_matr()
{
  size_t inc_rows = incidence_.rows();
  size_t cycles_found = 0;

  // go from all
  for (size_t i = 0; i < inc_rows && cycles_found < circs_.cols(); ++i)
  {
    std::vector<int> tmp_vec = dfs_start(i);

    while (!tmp_vec.empty() && cycles_found < circs_.cols())
    {
      insert_cycle(cycles_found++, tmp_vec);
      tmp_vec = dfs_start(i);
    }
  }

  std::cout << circs_ << std::endl;
}

std::vector<int> Circuit::dfs_start(size_t from) const
{
  std::vector<int> tmp{};
  tmp.resize(edges_.size());
  std::vector<Color> cols(edges_.size(), Color::WHITE);

  if (!dfs(from, from, edges_.size(), tmp, cols))
    tmp.clear();

  return tmp;
}

bool Circuit::dfs(size_t nstart, size_t nactual, size_t ecurr, std::vector<int> &cyc_rout,
                  std::vector<Color> &colors) const
{
  /* Mark that now we are at this vert */
  colors[nactual] = Color::GREY;

  for (size_t i = 0, edg_size = edges_.size(); i < edg_size; ++i)
  {
    /* Check if we came from this vert */
    if (i == ecurr)
      continue;

    auto &cur_edge = edges_[i];
    /* Check if we have a route between verts */
    if (nactual != cur_edge.junc1.norm && nactual != cur_edge.junc2.norm)
      continue;

    size_t dest_vert = cur_edge.junc1.norm == nactual ? cur_edge.junc2.norm : cur_edge.junc1.norm;

    int to_cyc_rout = nactual == cur_edge.junc1.norm ? 1 : -1;
    /* Check if we have already visited this vert */
    if (colors[dest_vert] == Color::GREY)
    {
      if (dest_vert == nstart)
      {
        cyc_rout[i] = to_cyc_rout;
        if (is_cyc_unique(cyc_rout))
          return true;
        cyc_rout[i] = 0;
      }
      continue;
    }

    if (colors[dest_vert] == Color::WHITE)
    {
      std::vector<int> rout_cpy{cyc_rout};
      std::vector<Color> col_cpy{colors};

      rout_cpy[i] = to_cyc_rout;

      bool is_cycle = dfs(nstart, dest_vert, i, rout_cpy, col_cpy);

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
  auto A_0 = MX::glue_side(inc_cut_, MX::Matrix<double>{inc_cut_.rows(), 1});
  fill_circ_matr();

  auto BR = circs_ * make_res_matr();
  auto BE = circs_ * make_eds_matr();

  auto BR_BE = MX::glue_side(BR, BE);

  auto system = MX::glue_bott(A_0, BR_BE);

  auto curs = MX::Matrix<double>::solve(system);

  for (size_t i = 0, endi = curs.size(); i < endi; ++i)
    edges_[i].cur = curs[i];
}

void Circuit::dump(const std::string &png_file, const std::string &dot_file) const
{
  char name_of_edge = 'A';
  size_t num_of_edge = 0;
  std::ofstream fout;

  fout.open(dot_file, std::ios::out);

  if (!fout.is_open())
  {
    std::cerr << "Can't open dump file" << dot_file << "\n";
    return;
  }

  fout << "digraph D {\n"
       << "rankdir=\"LR\";\n";

  for (auto &&e : edges_)
  {
    fout << name_of_edge
         << " [label=\n\" "
            "Edge # "
         << num_of_edge
         << "\n"
            "I = "
         << e.get_cur()
         << " A\n "
            "R = "
         << e.rtor
         << " Om\n "
            "E = "
         << e.eds
         << " V\n\", "

            "shape = box, color = black]"
         << std::endl;

    fout << e.junc1.norm << " -> " << name_of_edge << " -> " << e.junc2.norm << std::endl;

    fout << std::endl;

    ++name_of_edge;
    ++num_of_edge;
  }

  fout << "}\n";

  fout.close();

  std::string prompt = "dot " + dot_file + " -Tpng >" + png_file;

  if (system(prompt.c_str()) == -1)
    std::cerr << "An error occurred in system command" << std::endl;
}

} // namespace CTS