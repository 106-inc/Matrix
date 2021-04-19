#include "chain.hh"

void naive_mul(size_t amount, std::vector<MX::Matrix<chain::ldbl>> &mtr_vec)
{
  MX::Matrix<chain::ldbl> res = mtr_vec[0];

  for (size_t i = 1; i < amount; ++i)
    res *= mtr_vec[i];

  std::cout << "Naive mul:" << std::endl;
  std::cout << res << std::endl;
}

void matr_init(size_t amount, std::vector<MX::Matrix<chain::ldbl>> &mtr_vec)
{
  int tmp_elem = 0;
  std::vector<double> matr_buf{};

  for (size_t i = 0; i < amount; ++i)
  {
    size_t rw = 0, cl = 0;
    std::cin >> rw >> cl;

    for (size_t i = 0; i < rw * cl; ++i)
    {
      std::cin >> tmp_elem;
      matr_buf.push_back(tmp_elem);
    }

    mtr_vec.push_back(MX::Matrix<chain::ldbl>{rw, cl, matr_buf.begin(), matr_buf.end()});
    matr_buf.clear();
  }
}

int main()
{

  chain::MatrixChain ch;
  size_t amount{};
  std::cin >> amount;
  std::vector<MX::Matrix<chain::ldbl>> mtr_vec;

  matr_init(amount, mtr_vec);

  naive_mul(amount, mtr_vec);

  for (size_t i = 0; i < amount; ++i)
    ch.push(mtr_vec[i]);

  auto res = ch.multiply();

  std::cout << "Optimal mul:" << std::endl;
  std::cout << res << std::endl;

  return 0;
}