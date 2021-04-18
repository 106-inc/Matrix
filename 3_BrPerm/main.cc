#include "chain.hh"

int main( void )
{
#if 0
  chain::MatrixChain ch;
  size_t amount{};
  std::cin >> amount;

  for (size_t i = 0; i < amount; ++i)
  {
    size_t rw = 0, cl = 0;
    std::cin >> rw >> cl;

    ch.push(MX::Matrix<chain::ldbl>{rw, cl});
  }
#endif

  chain::MatrixChain ch;

  MX::Matrix<chain::ldbl> m1{2, 2, [](size_t i, size_t j){return i == j;}};
  MX::Matrix<chain::ldbl> m2{2, 3, {1, 2, 3, 4, 5, 6}};
  MX::Matrix<chain::ldbl> m3{3, 3, [](size_t i, size_t j){return i == j;}};

  ch.push(m1);
  ch.push(m2);
  ch.push(m3);

  auto res = ch.multiply();

  std::cout << res << std::endl;

  return 0;
}