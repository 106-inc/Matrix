#include "circuits.hh"

namespace CTS
{
    MX::Matrix<double> make_inc_matr (const std::vector<Edge> & edges, size_t j_num)
    {
        size_t e_num = edges.size();
        MX::Matrix<double> A {j_num, e_num};

        for (size_t i = 0; i < e_num; ++i)
        {
            A.set(edges[i].junc1, i, 1);
            A.set(edges[i].junc2, i, -1);
        }

        return A;
    }

    MX::Matrix<double> make_res_matr (const std::vector<Edge> & edges)
    {
        size_t e_num = edges.size();
        MX::Matrix<double> R {e_num, e_num};

        for (size_t i = 0; i < e_num; ++i)
            R.set(i, i, edges[i].rtor);

        return R;
    }

    MX::Matrix<double> make_eds_matr (const std::vector<Edge> & edges)
    {
        size_t e_num = edges.size();
        MX::Matrix<double> E {e_num, 1};

        for (size_t i = 0; i < e_num; ++i)
            R.set(i, 1, edges[i].voltage);

        return R;
    }
}