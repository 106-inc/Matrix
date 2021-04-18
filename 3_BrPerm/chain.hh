#ifndef __CHAIN_H__
#define __CHAIN_H__

#include <set>
#include <stack>
#include <limits>
#include "matrix.hh"

namespace chain
{
  using ldbl = long double;

  class MatrixChain final
  {
  private:
    std::vector<MX::Matrix<ldbl>> chain_{};
    std::vector<size_t> order_{};

    std::vector<size_t> sizes_{};
  
  public:
    MatrixChain() = default;

    MatrixChain(const MatrixChain &) = delete;
    MatrixChain &operator =(const MatrixChain &) = delete;

    // add matrix to chain
    void push( const MX::Matrix<ldbl> &mat )
    {
      chain_.emplace_back(mat);
      order_.push_back(order_.size());
      if (sizes_.size() == 0)
        sizes_.push_back(mat.rows()), sizes_.push_back(mat.cols());
      else if (sizes_.back() != mat.rows())
        throw std::runtime_error{"Incompatible matrix size"};
      else
        sizes_.push_back(mat.cols());

      order_recalc();
    }

    ~MatrixChain() = default;

    MX::Matrix<ldbl> multiply ( );

  private:
    MX::Matrix<size_t> order_recalc();
  };


  MX::Matrix<size_t> MatrixChain::order_recalc()
  {
    size_t num_of_mat = chain_.size();

    MX::Matrix<size_t> calcs{num_of_mat, num_of_mat};
    MX::Matrix<size_t> braces{num_of_mat, num_of_mat};

    for (size_t l = 1; l < num_of_mat; ++l)
      for (size_t i = 0; i < num_of_mat - l; ++i)
      {
        size_t j = i + l;
        calcs.set(i, j, std::numeric_limits<size_t>::max());
        for (size_t k = i; k < j; ++k)
        {
          size_t to_set = calcs[i][k] + calcs[k + 1][j] + sizes_[i] * sizes_[k + 1] * sizes_[j + 1];
          size_t min_set = std::min(calcs[i][j], to_set); 
          if (min_set == to_set)
          {
            braces.set(i, j, k + 1);
            calcs.set(i, j, to_set);
          }
        }
      }
    std::cout << braces;
    std::cout << calcs[0][num_of_mat - 1] << std::endl;

    return braces;
  }

  MX::Matrix<ldbl> MatrixChain::multiply ( )
  {
    std::set<SubChain> mul_tree{};

    auto braces = order_recalc();

    size_t num_of_mat = chain_.size();
    size_t last_idx = num_of_mat - 1;

    std::stack<std::pair<size_t, size_t>> idx_st{};
    idx_st.emplace(0, last_idx);

    while (!idx_st.empty())
    {
      /* write main cycle */
    }

  }

  struct SubChain final
  {
  public:

    size_t from_, to_, cut_;

  public:

    SubChain ( size_t from, size_t to, size_t cut = 0 ) : from_(from), to_(to), cut_(cut)
    {};

    SubChain ( ) = default;

    /* TODO: redefine spaceship operator or all cmp operators */
  };
}

#endif // __CHAIN_H__