#include "chain.hh"
#include "timer.hh"

static MX::Matrix<chain::ldbl> naive_mul(size_t amount, std::vector<MX::Matrix<chain::ldbl>> &mtr_vec)
{
  MX::Matrix<chain::ldbl> res = mtr_vec[0];

  for (size_t i = 1; i < amount; ++i)
    res *= mtr_vec[i];

  std::cout << "Naive mul:" << std::endl;
  std::cout << res << std::endl;

  return res; 
}

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

  chain::MatrixChain ch;
  size_t amount{};
  std::cin >> amount;
  std::vector<MX::Matrix<chain::ldbl>> mtr_vec;

  matr_init(amount, mtr_vec);

  MX::Matrix<chain::ldbl> naive_res = naive_mul(amount, mtr_vec);

  for (size_t i = 0; i < amount; ++i)
    ch.push(mtr_vec[i]);

  auto res = ch.multiply();



  if (res == naive_res)
    std::cout << "Test passed" << std::endl;
  else 
    std::cout << "Test didn't pass" << std::endl;



  std::cout << "Optimal mul:" << std::endl;
  std::cout << res << std::endl;


  std::vector<size_t> optimal_order = ch.get_order();

#if (ORDER == 1)
  for (auto matr: optimal_order)
    std::cout << matr << " ";

  std::cout << std::endl;
  
#endif

  return 0;
}