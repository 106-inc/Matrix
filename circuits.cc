#include "circuits.hh"

namespace CTS
{

std::ostream &operator <<( std::ostream &ost, const Edge &edge )
{
  double real_cur = MX::is_zero(edge.cur) ? 0.0 : edge.cur;

  ost << edge.junc1.real << " -- " << edge.junc2.real << ": " << real_cur << " A" << std::endl;

  return ost;
}

Circuit::Circuit(const std::vector<Edge> &edges, size_t j_num) : edges_(edges), 
                                                                 incidence_(j_num, edges_.size()),
                                                                 circs_(edges_.size() - j_num + 1, edges_.size())

{
  size_t e_num = edges_.size();

  for (size_t i = 0; i < e_num; ++i)
  {
    incidence_.set(edges[i].junc1.norm, i, 1);
    incidence_.set(edges[i].junc2.norm, i, -1);
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

void Circuit::insert_cycle( size_t num, const std::vector<int> &cyc )
{
  for (size_t i = 0, endi = circs_.cols(); i < endi; ++i)
    circs_.set(num, i, cyc[i]);
}

bool Circuit::is_cyc_unique(const std::vector<int> &vec) const
{
  for (size_t i = 0, endi = circs_.rows(); i < endi; ++i)
  {
    size_t j = 0;
    for (size_t endj = circs_.cols(); j < endj; ++j)
      if (std::abs(circs_[i][j]) != std::abs(vec[j]))
        break;
    if (j == circs_.cols())
      return false;
  }

  return true;
}

void Circuit::fill_circ_matr()
{
  size_t inc_rows = incidence_.rows();
  size_t cycles_found = 0;


  // go from all
  for (size_t i = 0; i < inc_rows && circs_.rows() != cycles_found; ++i)
  {
    auto tmp_vec = dfs_start(i);

    if (!is_cyc_unique(tmp_vec))
      continue;

    if (!tmp_vec.empty())
      insert_cycle(cycles_found++, tmp_vec);
  }
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
    size_t dest_vert = edges_[i].junc1.norm == nactual ? edges_[i].junc2.norm : edges_[i].junc1.norm;

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
  fill_circ_matr();
  
  std::cout << "A:\n" << incidence_ << "\nB:\n" << circs_ << std::endl;

  auto BR = circs_ * make_res_matr();
  auto BE = circs_ * make_eds_matr(); 

  auto BR_BE = MX::glue_side(BR, BE);

  auto system = MX::glue_bott(A_0, BR_BE);

  std::cerr << system << std::endl;
  std::cerr << "cols: " << system.cols() << ", rows: " << system.rows() << std::endl;

  dump("picture.png");

  auto curs = MX::Matrix<double>::solve(system);

  for (size_t i = 0, endi = curs.size(); i < endi; ++i)
    edges_[i].cur = curs[i];
}

void Circuit::dump( const std::string &png_file, const std::string& dot_file) const
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

  fout << "digraph D {\n" << "rankdir=\"LR\";\n";

  for (auto&& e: edges_)
  {
      fout << name_of_edge << " [label=\n\" "
                                       "Edge # " << num_of_edge << "\n"
                                       "I = "         << e.get_cur() << " A\n "
                                       "R = "        << e.rtor      << " Om\n "
                                       "E = "         << e.eds       << " V\n\", "

                                       "shape = box, color = black]" << std::endl;

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