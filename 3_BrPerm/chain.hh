#ifndef __CHAIN_H__
#define __CHAIN_H__

#include "matrix.hh"
#include "timer.hh"
#include <limits>
#include <memory>
#include <set>
#include <stack>
#include <iterator>
#include <algorithm>


namespace chain
{
using ldbl = long double;

namespace detail
{
struct SubChain;
using pSChain = std::unique_ptr<SubChain>;

struct Interval final
{
  size_t from_{}, to_{}, cut_{};

  bool operator<(const Interval &rhs) const;
};


struct SubChain final
{
  Interval interv{};
  pSChain left{}, right{};

  SubChain(Interval inter, pSChain l = {}, pSChain r = {}) : interv(inter), left(std::move(l)), right(std::move(r)){};

  void insert(const Interval &to_insert);
  pSChain::pointer find(const Interval &to_find);

  bool is_leaf() const;

  SubChain() = default;

  SubChain(const SubChain&) = delete;
  SubChain& operator=(const SubChain&) = delete; 

  /* TODO: check is it okay */
};

} // namespace detail

class MatrixChain final
{
private:
  std::vector<MX::Matrix<ldbl>> chain_{};
  std::vector<size_t> order_{};

  std::vector<size_t> sizes_{};

  MX::Matrix<size_t> braces_mat_{};

  MX::Matrix<ldbl> result_{};

  ldbl elapsed_ms_{};

public:

  using back_it = std::back_insert_iterator<std::vector<MX::Matrix<ldbl>>>;

public:
  MatrixChain() = default;

  MatrixChain(const MatrixChain &) = delete;
  MatrixChain &operator=(const MatrixChain &) = delete;

  // add matrix to chain
  void emplace(const back_it& mat, size_t rows, size_t cols);

  void push(const MX::Matrix<ldbl> &mat);
  void push(const back_it& mat);

  // get multiplication order
  const std::vector<size_t> &get_order() const;

  // get current matrix chain
  const std::vector<MX::Matrix<ldbl>>& get_chain() const;

  const MX::Matrix<ldbl> &multiplied() const;

  ldbl get_mult_time() const
  {
    return elapsed_ms_;
  }

  back_it back_inserter()
  {
    return back_it(chain_);
  }

  ~MatrixChain() = default;

private:
  void set_braces();

  void fill_order();

  void order_n_mult();

  detail::pSChain fill_mul_tree();
};

} // namespace chain

#endif // __CHAIN_H__