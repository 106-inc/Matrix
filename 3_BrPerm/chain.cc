#include "chain.hh"

namespace chain
{

namespace detail
{
bool Interval::operator<(const Interval &rhs) const
{
  if (cut_ == rhs.cut_)
    return to_ < rhs.to_;
  return cut_ < rhs.cut_;
}

void SubChain::insert(const Interval &to_insert)
{
  auto cur_node = this;

  while (true)
  {
    if (to_insert < cur_node->interv)
    {
      if (!cur_node->left)
      {
        cur_node->left = std::make_unique<SubChain>(to_insert);
        return;
      }
      cur_node = cur_node->left.get();
    }
    else
    {
      if (!cur_node->right)
      {
        cur_node->right = std::make_unique<SubChain>(to_insert);
        return;
      }
      cur_node = cur_node->right.get();
    }
  }
}

bool SubChain::is_leaf() const
{
  return !left && !right;
}
} // namespace detail
void MatrixChain::push(const MX::Matrix<ldbl> &mat)
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

const std::vector<size_t> &MatrixChain::get_order() const
{
  return order_;
}

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
}

MX::Matrix<ldbl> MatrixChain::multiply()
{
  auto root = fill_mul_tree();
  order_.clear();
  order_.reserve(braces_mat_.cols() - 1);

  // here we walk through the tree and perform multiplication & fill order vec

  auto cur_node = root.get();

  MX::Matrix<ldbl> res{};

  using vis_el = std::pair<detail::SubChain *, bool>;
  using mat_el = std::pair<MX::Matrix<ldbl>, int>;

  std::stack<vis_el> to_visit;
  std::stack<mat_el> mx_stack{};

  to_visit.emplace(root.get(), false);

  constexpr int MULTED = -1;

  while (!to_visit.empty())
  {
    auto &cur_top = to_visit.top();
    cur_node = cur_top.first;

    if (cur_node->is_leaf())
    {
      auto idx = cur_node->interv.from_;
      mx_stack.emplace(chain_[idx], idx);
      to_visit.pop();
    }
    else if (cur_top.second)
    {
      auto ms_el1 = mx_stack.top();
      mx_stack.pop();
      auto ms_el2 = mx_stack.top();
      mx_stack.pop();

      auto &m1 = ms_el1.first, &m2 = ms_el2.first;

      if (ms_el1.second != MULTED)
        order_.push_back(ms_el1.second);
      if (ms_el2.second != MULTED)
        order_.push_back(ms_el2.second);

      mx_stack.emplace(m1 * m2, MULTED);
      to_visit.pop();
    }
    else
    {
      cur_top.second = true;
      to_visit.emplace(cur_node->left.get(), false);
      to_visit.emplace(cur_node->right.get(), false);
    }
  }

  return mx_stack.top().first;
}

detail::pSChain MatrixChain::fill_mul_tree()
{
  size_t num_of_mat = chain_.size();
  detail::Interval init_int = {0, num_of_mat - 1, braces_mat_[0][num_of_mat - 1] - 1};
  detail::pSChain root = std::make_unique<detail::SubChain>(init_int);

  if (braces_mat_.cols() != sizes_.size() + 1)
  {
    order_recalc();
    order_.clear();
  }

  std::stack<std::pair<size_t, size_t>> idx_stack{};
  idx_stack.emplace(0, root->interv.cut_);
  idx_stack.emplace(root->interv.cut_ + 1, num_of_mat - 1);

  size_t start = 0, end = 0;

  while (!idx_stack.empty())
  {
    auto cur_p = idx_stack.top();
    idx_stack.pop();

    start = cur_p.first;
    end = cur_p.second;

    if (start == end)
    {
      root->insert({start, end, end});
      continue;
    }

    size_t cut = braces_mat_[start][end];

    root->insert({start, end, cut - 1});

    idx_stack.emplace(start, cut - 1);
    idx_stack.emplace(cut, end);
  }

  return root;
}

} // namespace chain
