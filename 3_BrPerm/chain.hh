#ifndef __CHAIN_H__
#define __CHAIN_H__

#include <limits>
#include <set>
#include <stack>
#include "matrix.hh"

namespace chain
{
using ldbl = long double;

namespace detail
{
struct SubChain final
{
  size_t from_, to_, cut_;

  SubChain(size_t from, size_t to, size_t cut = 0) : from_(from), to_(to), cut_(cut){};

  SubChain() = default;

  bool operator<(const SubChain &rhs) const
  {
    if (cut_ == rhs.cut_)
      return to_ < rhs.to_;
    return cut_ < rhs.cut_;
  }
  /* TODO: check is it okay */
};
}

class MatrixChain final
{
private:
  std::vector<MX::Matrix<ldbl>> chain_{};
  std::vector<size_t> order_{};

  std::vector<size_t> sizes_{};

  MX::Matrix<size_t> braces_mat_{};

public:
  MatrixChain() = default;

  MatrixChain(const MatrixChain &) = delete;
  MatrixChain &operator=(const MatrixChain &) = delete;

  // add matrix to chain
  void push(const MX::Matrix<ldbl> &mat)
  {
    chain_.emplace_back(mat);
    if (sizes_.size() == 0)
      sizes_.push_back(mat.rows()), sizes_.push_back(mat.cols());
    else if (sizes_.back() != mat.rows())
      throw std::runtime_error{"Incompatible matrix size"};
    else
      sizes_.push_back(mat.cols());

    order_recalc();
  }

  const std::vector<size_t> &get_order() const
  {
    return order_;
  }

  ~MatrixChain() = default;

  MX::Matrix<ldbl> multiply();

private:
  void order_recalc();

  std::set<detail::SubChain> fill_mul_tree();
};

void MatrixChain::order_recalc()
{
  size_t num_of_mat = chain_.size();

  MX::Matrix<size_t> calcs{num_of_mat, num_of_mat};
  braces_mat_ = MX::Matrix<size_t>{num_of_mat, num_of_mat};

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
          braces_mat_.set(i, j, k + 1);
          calcs.set(i, j, to_set);
        }
      }
    }
  /*std::cout << braces_mat_;
  std::cout << calcs[0][num_of_mat - 1] << std::endl;*/
}

MX::Matrix<ldbl> MatrixChain::multiply()
{
  auto mul_tree = fill_mul_tree();
  order_.clear();
  order_.reserve(braces_mat_.cols() - 1);
  // here we perform multiplication
  auto cur_matr_it = mul_tree.begin(), matr_end_it = mul_tree.end();

  auto res_matr = chain_[cur_matr_it->from_];
  order_.emplace_back(cur_matr_it->from_);
  ++cur_matr_it;

  while (cur_matr_it != matr_end_it)
  {
    if (cur_matr_it->from_ == cur_matr_it->to_)
    {
      res_matr *= chain_[cur_matr_it->from_];
      order_.emplace_back(cur_matr_it->from_);
    }

    ++cur_matr_it;
  }

  for (auto elem : order_)
    std::cout << elem << " ";
  std::cout << std::endl;
  return res_matr;
}

std::set<detail::SubChain> MatrixChain::fill_mul_tree()
{
  std::set<detail::SubChain> mul_tree{};

  if (braces_mat_.cols() != sizes_.size() + 1)
  {
    order_recalc();
    order_.clear();
  }

  size_t num_of_mat = chain_.size();

  std::stack<std::pair<size_t, size_t>> idx_stack{};
  idx_stack.emplace(0, num_of_mat - 1);

  size_t start = 0, end = 0;

  while (!idx_stack.empty())
  {
    auto cur_p = idx_stack.top();
    idx_stack.pop();

    start = cur_p.first;
    end = cur_p.second;

    if (start == end)
    {
      mul_tree.emplace(start, end, end);
      continue;
    }

    size_t cut = braces_mat_[start][end];

    mul_tree.emplace(start, end, cut - 1);

    idx_stack.emplace(start, cut - 1);
    idx_stack.emplace(cut, end);
  }

  return mul_tree;
}

} // namespace chain

#endif // __CHAIN_H__