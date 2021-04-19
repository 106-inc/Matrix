#include "chain.hh"
#include "timer.hh"
#include <fstream>

#if (TIME == 1)
static MX::Matrix<chain::ldbl> naive_mul(size_t amount, std::vector<MX::Matrix<chain::ldbl>> &mtr_vec)
{
  MX::Matrix<chain::ldbl> res = mtr_vec[0];

  for (size_t i = 1; i < amount; ++i)
    res *= mtr_vec[i];

  return res;
}
#endif

void matr_init(size_t amount, std::vector<MX::Matrix<chain::ldbl>> &mtr_vec)
{
  int matr_lim = 5;
  std::vector<double> matr_buf{};

  for (size_t i = 0; i < amount; ++i)
  {
    size_t rw = 0, cl = 0;
    std::cin >> rw >> cl;

    mtr_vec.push_back(MX::Matrix<chain::ldbl>::Random(rw, cl, matr_lim));
    matr_buf.clear();
  }
}

int main()
{
#if (REDIRECT == 1)
  std::ifstream ifs("../test/5.dat");
  if (!ifs)
    return -1;

  std::cin.rdbuf(ifs.rdbuf());

#endif // REDIRECT

  chain::MatrixChain ch;
  size_t amount{};
  std::cin >> amount;
  std::vector<MX::Matrix<chain::ldbl>> mtr_vec;

  matr_init(amount, mtr_vec);

#if (TIME == 1)
  Time::Timer naive_time;

  MX::Matrix<chain::ldbl> naive_res = naive_mul(amount, mtr_vec);

  auto measured_naive = naive_time.elapsed();
  std::cout << "naive multiplication time: " << measured_naive << " microsecs\n";
#endif

  for (size_t i = 0; i < amount; ++i)
    ch.push(mtr_vec[i]);

#if (TIME == 1)
  Time::Timer optim_time;

  auto res = ch.multiplied();

  auto measured_opt = optim_time.elapsed();
  std::cout << "optimal multiplication time: " << measured_opt << " microsecs\n";

  auto gain = static_cast<long double>(measured_naive) / measured_opt;
  std::cout << "Gain in " << gain << " times" << std::endl;
#endif

#if (TIME == 1)
  if (res == naive_res)
    std::cout << "TEST PASSED" << std::endl;
  else
    std::cout << "TEST FAILED" << std::endl;
#endif

  std::vector<size_t> optimal_order = ch.get_order();

#if (ORDER == 1)
  for (auto matr : optimal_order)
    std::cout << matr << " ";

  std::cout << std::endl;

#endif

#if (REDIRECT == 1)
  ifs.close();
#endif
  return 0;
}