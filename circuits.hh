#ifndef MATRIX_CIRCUITS_HH
#define MATRIX_CIRCUITS_HH

#include <vector>

#include "lin_alg/la.hh"
#include "lin_alg/matrix.hh"

// TODO: найти где будут Edge

namespace CTS
{
    struct Edge; // rtor, junc1, junc2, voltage

    MX::Matrix<double> make_inc_matr (const std::vector<Edge> & edges, size_t jnum);
    MX::Matrix<double> make_res_matr (const std::vector<Edge> & edges);
    MX::Matrix<double> make_eds_matr (const std::vector<Edge> & edges);
}

#endif //MATRIX_CIRCUITS_HH
