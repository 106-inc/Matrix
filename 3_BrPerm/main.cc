#include "chain.hh"
#include <fstream>



#if (TIME == 1)
static MX::Matrix<chain::ldbl> naive_mul(chain::MatrixChain& chain)
{
  std::vector<MX::Matrix<chain::ldbl>> cur_chain = chain.get_chain();
  size_t ch_size = cur_chain.size();

  MX::Matrix<chain::ldbl> res = cur_chain[0];
 
  for (size_t i = 1; i < ch_size; ++i)
    res *= cur_chain[i];
  
  return res;
}
#endif


void ch_init(chain::MatrixChain& chain)
{
  int matr_lim = 2;
  size_t rw = 0, cl = 0;
  auto& in = std::cin;

  chain::MatrixChain::back_it ch_back = chain.back_inserter();

  while(in)
  {
    in >> rw >> cl;

    if (!in)
      break;
    
    if (!in.good())
      throw std::runtime_error{"Invalid character at stdin"};

    
    ch_back = MX::Matrix<chain::ldbl>::Random(rw, cl, matr_lim);

    chain.emplace(ch_back, rw, cl);

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


  /* TODO: think about output iterator like std::back_inserter for removing mtr_vec */
  
  try
  {
    //matr_init(mtr_vec);
    ch_init(ch);
  }

  catch (std::exception& err)
  {
    std::cerr << "Error occured in " << err.what() << std::endl;
    return -1;
  }

#if (TIME == 1)
  Time::Timer naive_time;

  MX::Matrix<chain::ldbl> naive_res = naive_mul(ch);

  auto measured_naive = naive_time.elapsed();
  std::cout << "naive multiplication time: " << measured_naive << " microsecs\n";
#endif



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
