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
    E.set(i, 1, edges_[i].voltage);

  return E;
}
} // namespace CTS