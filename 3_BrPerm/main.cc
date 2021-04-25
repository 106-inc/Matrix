#include "chain.hh"
#include <fstream>

#if (TIME == 1)
static MX::Matrix<chain::ldbl> naive_mul(std::vector<MX::Matrix<chain::ldbl>> &mtr_vec)
{
  MX::Matrix<chain::ldbl> res = mtr_vec[0];
  size_t amount = mtr_vec.size();

  for (size_t i = 1; i < amount; ++i)
    res *= mtr_vec[i];

  return res;
}
#endif

void matr_init(std::vector<MX::Matrix<chain::ldbl>> &mtr_vec)
{
  int matr_lim = 2;
  size_t rw = 0, cl = 0;
  auto& in = std::cin;

  while(in)
  {
    in >> rw >> cl;

    if (!in)
      break;
    
    if (!in.good())
      throw std::runtime_error{"Invalid character at stdin"};

    mtr_vec.push_back(MX::Matrix<chain::ldbl>::Random(rw, cl, matr_lim));
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
  std::vector<MX::Matrix<chain::ldbl>> mtr_vec;

  /* TODO: think about output iterator like std::back_inserter for removing mtr_vec */
  //auto ch_it = std::back_insert_iterator{ch};
  
  try
  {
    matr_init(mtr_vec);
  }

  catch (std::exception& err)
  {
    std::cerr << "Error occured in " << err.what() << std::endl;
    return -1;
  }

#if (TIME == 1)
  Time::Timer naive_time;

  MX::Matrix<chain::ldbl> naive_res = naive_mul(mtr_vec);

  auto measured_naive = naive_time.elapsed();
  std::cout << "naive multiplication time: " << measured_naive << " microsecs\n";
#endif

  for (auto&& mtr_vec_elm : mtr_vec)
    ch.push(mtr_vec_elm);

#if (TIME == 1)
  Time::Timer optim_time;

  auto res = ch.multiplied();

  auto measured_opt = ch.get_mult_time();
  std::cout << "optimal multiplication time: " << measured_opt << " microsecs\n";

  auto gain = static_cast<long double>(measured_naive) / measured_opt;
  std::cout << "Gain in " << gain << " times" << std::endl;

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
