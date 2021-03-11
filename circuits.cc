#include "circuits.hh"

namespace CTS
{
    
Circuit::Circuit(const std::vector<Edge> &edges, size_t j_num) : edges_(edges),
                                                                 incidence_(j_num, edges_.size())
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


MX::Matrix<int> Circuit::make_circ_matr()
{
  size_t inc_rows = incidence_.rows();

  std::vector<std::vector<Edge>> cycles;

  // go from all 
  for (size_t i = 0; i < inc_rows; ++i)
    cycles.push_back(dfs_start(i));


  
}

std::vector<CTS::Edge> Circuit::dfs_start( size_t from )
{
  std::vector<Edge> tmp{};
  std::vector<Color> cols(edges_.size(), Color::WHITE);

  dfs(from, from, tmp, cols);
  return tmp;
}

void Circuit::dfs( size_t nstart, size_t nactual, std::vector<Edge> &cyc_rout, std::vector<Color> &colors )
{
  /* Mark that now we are at this vert */
  colors[nactual] = Color::GREY;

  Edge eprev = cyc_rout.back();
  size_t nprev = eprev.junc1 == nactual ? eprev.junc2 : eprev.junc1;


  for (size_t i = 0, edg_size = edges_.size(); i < edg_size; ++i)
  {
    /* Check if we came from this vert */
    if (i == nprev)
      continue;
    
    /* Check if we have already visited this vert */
    if (colors[i] == Color::GREY)
    {
      if (i == nstart)
        return;
      continue;
    }

    if (colors[i] == Color::WHITE)
    {
      std::vector<Edge> rout_cpy{cyc_rout};
      std::vector<Color> col_cpy{colors};
      dfs(nstart, i, rout_cpy, col_cpy);

      if (!rout_cpy.empty())
      {
        /* Means that we've found a cycle in previous dfs call */
        cyc_rout = rout_cpy;
        return;
      }
    }
  }
  
  cyc_rout.clear();
  /* Mark that we go away from vert. */
  colors[nactual] = Color::BLACK;
}


void Circuit::curs_calc()
{
  
}
} // namespace CTS