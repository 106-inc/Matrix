#include "circuits.hh"

namespace CTS
{

std::ostream &operator<<(std::ostream &ost, const Edge &edge)
{
  double real_cur = MX::is_zero(edge.cur) ? 0.0 : edge.cur;

  ost << edge.junc1 << " -- " << edge.junc2 << ": " << real_cur << " A" << std::endl;

  return ost;
}

Circuit::Circuit(const MX::Matrix<int> &inc, const MX::Matrix<double> &res, const MX::Matrix<double> &eds)
    : incidence_(inc), resistance_(res), eds_(eds), circs_(incidence_.cols(), incidence_.cols())
{
  // fill edges vector

  size_t e_num = incidence_.cols();

  edges_.reserve(e_num);

  // for more locality
  auto inc_t = MX::transpose(inc);
  std::unordered_set<size_t> j_loops;

  for (size_t i = 0; i < e_num; ++i)
  {
    Edge new_edge = {incidence_.rows(), incidence_.rows(), res[i][i], eds[i][0]};
    size_t v_cnt = 0;

    for (size_t j = 0; j < incidence_.rows(); ++j)
    {
      auto elem = inc_t[i][j];
      if (elem != 0)
      {
        if (elem > 0)
          new_edge.junc2 = j;
        else
          new_edge.junc1 = j;
        ++v_cnt;
      }
    }

    if (v_cnt == 1)
    {
      if (new_edge.junc1 != incidence_.rows())
        new_edge.junc2 = new_edge.junc1;
      else
        new_edge.junc1 = new_edge.junc2;

      j_loops.insert(new_edge.junc1);
    }

    edges_.push_back(new_edge);
  }

  fill_inc_cut(j_loops);
}

void Circuit::fill_inc_cut(const std::unordered_set<size_t> &j_loops)
{
  inc_cut_ = MX::Matrix<double>{incidence_.rows() - j_loops.size(), incidence_.cols()};

  size_t cut_cnt = 0;
  for (size_t i = 0, endi = incidence_.rows(); i < endi; ++i)
  {
    if (j_loops.contains(i))
      continue;
    for (size_t j = 0, endj = incidence_.cols(); j < endj; ++j)
      inc_cut_.set(cut_cnt, j, incidence_[i][j]);
    ++cut_cnt;
  }
}

void Circuit::insert_cycle(const std::vector<int> &cyc)
{
  for (size_t i = 0, endi = circs_.cols(); i < endi; ++i)
  {
    circs_.set(cycles_amount, i, cyc[i]);
    if (cyc[i])
      edges_visited.insert(i);
  }
  ++cycles_amount;
}

bool Circuit::is_cyc_unique(const std::vector<int> &vec) const
{
  for (size_t i = 0; i < vec.size(); ++i)
    if (vec[i] && !edges_visited.contains(i))
      return true;

  return false;
}
bool Circuit::check_n_insert(const std::vector<int> &vec)
{
  if (is_cyc_unique(vec))
    insert_cycle(vec);

  if (edges_visited.size() == edges_.size())
    return false;

  return true;
}

void Circuit::fill_circ_matr()
{
  // go from all
  for (size_t i = 0; i < incidence_.rows() && cycles_amount < circs_.cols(); ++i)
  {
    dfs_start(i);
    if (edges_visited.size() == edges_.size())
      break;
  }

  // std::cout << circs_ << std::endl;
}

void Circuit::dfs_start(size_t from)
{
  std::vector<int> rout{};
  rout.resize(edges_.size());
  std::vector<bool> visited(edges_.size(), false);

  struct stk_frame final
  {
    size_t nactual, ecurr;
    std::vector<int> cyc_rout;
    std::vector<bool> visited;
  };
  std::stack<stk_frame> stack;

  stack.push({from, edges_.size(), rout, visited});

  while (!stack.empty())
  {
    size_t nactual = stack.top().nactual, ecurr = stack.top().ecurr;
    auto cyc_rout = stack.top().cyc_rout;
    auto vted = stack.top().visited;

    stack.pop();
    /* Mark that now we are at this vert */
    vted[nactual] = true;

    for (size_t i = 0, edg_size = edges_.size(); i < edg_size; ++i)
    {
      /* Check if we came from this vert */
      if (i == ecurr)
        continue;

      auto &cur_edge = edges_[i];
      /* Check if we have a route between verts */
      if (nactual != cur_edge.junc1 && nactual != cur_edge.junc2)
        continue;

      size_t dest_vert = cur_edge.junc1 == nactual ? cur_edge.junc2 : cur_edge.junc1;

      int to_cyc_rout = nactual == cur_edge.junc1 ? 1 : -1;
      bool cur_state = vted[dest_vert];
      /* Check if we have already visited this vert */
      if (cur_state)
      {
        if (dest_vert == from)
        {
          cyc_rout[i] = to_cyc_rout;
          if (!check_n_insert(cyc_rout))
            return;
          cyc_rout[i] = 0;
        }
        continue;
      }

      std::vector<int> rout_cpy{cyc_rout};
      std::vector<bool> vted_cpy{vted};

      rout_cpy[i] = to_cyc_rout;

      stk_frame frame = {dest_vert, i, rout_cpy, vted_cpy};
      stack.push(frame);
    }
  }
}

MX::Matrix<double> Circuit::curs_calc()
{
  auto A_0 = MX::glue_side(inc_cut_, MX::Matrix<double>{inc_cut_.rows(), 1});

  fill_circ_matr();

  auto BR = circs_ * resistance_;
  auto BE = circs_ * eds_;

  auto BR_BE = MX::glue_side(BR, BE);

  auto system = MX::glue_bott(A_0, BR_BE);

  // dump("hello.png");
  auto curs = MX::Matrix<double>::solve(system);

  MX::Matrix<double> sol = {curs.size(), 1, curs.begin(), curs.end()};

  for (size_t i = 0, endi = curs.size(); i < endi; ++i)
    edges_[i].cur = curs[i];

  return sol;
}

void Circuit::dump(const std::string &png_file, const std::string &dot_file) const
{
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
    std::string name_of_edge = "E" + std::to_string(num_of_edge);

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
    fout << e.junc1 << " -> " << name_of_edge << " -> " << e.junc2 << std::endl;

    fout << std::endl;

    ++num_of_edge;
  }

  fout << "}\n";

  fout.close();

  std::string prompt = "dot " + dot_file + " -Tpng >" + png_file;

  if (system(prompt.c_str()) == -1)
    std::cerr << "An error occurred in system command" << std::endl;
}

} // namespace CTS
