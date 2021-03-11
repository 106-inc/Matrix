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


MX::Matrix<int> Circuit::make_circ_matr()
{
  size_t inc_rows = incidence_.rows();

  std::vector<std::vector<int>> cycles;

  // go from all 
  for (size_t i = 0; i < inc_rows; ++i)
  {
    auto tmp_vec = dfs_start(i);
    if (!tmp_vec.empty())
      cycles.push_back(tmp_vec);
  }

  MX::Matrix<double> circ{edges_.size(), cycles.size(), [&cycles](int i, int j)
                                                      {
                                                        int res{};
                                                        for (auto & edge : cycles[i])
                                                        {
                                                          if (edge == j)
                                                            return 1.0;
                                                          if (edge == -j)
                                                            return -1.0;
                                                          return 0.0;
                                                        }
                                                      }
  };
  
  // TODO: weak place здесь явно не все -- разобраться!!

  MX::Matrix<double> diag = circ.diag();

  size_t i = 0;
  for (size_t size = diag.rows(); i < size && !MX::is_zero(diag[i][i]); ++i);

  if (i < diag.rows())
  {
    MX::Matrix<double> res = {i, circ.cols(), [&circ](int i, int j)
    {

    }};
    return res;
  }

  return circ;
}

std::vector<int> Circuit::dfs_start( size_t from )
{
  std::vector<int> tmp{};
  std::vector<Color> cols(edges_.size(), Color::WHITE);

  dfs(from, from, tmp, cols);
  return tmp;
}

void Circuit::dfs( size_t nstart, size_t nactual, std::vector<int> &cyc_rout, std::vector<Color> &colors )
{
  /* Mark that now we are at this vert */
  colors[nactual] = Color::GREY;

  size_t nprev = cyc_rout.back();


  for (size_t i = 0, edg_size = edges_.size(); i < edg_size; ++i)
  {
    /* Check if we came from this vert */
    if (i == nprev)
      continue;

    /* Check if we have a route between verts */
    if (incidence_[nactual][i] == 0)
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
      std::vector<int> rout_cpy{cyc_rout};
      std::vector<Color> col_cpy{colors};

      rout_cpy.push_back(-i * incidence_[nactual][i]);
      
      size_t nnext = edges_[i].junc1 == nactual ? edges_[i].junc2 : edges_[i].junc1;

      dfs(nstart, nnext, rout_cpy, col_cpy);

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