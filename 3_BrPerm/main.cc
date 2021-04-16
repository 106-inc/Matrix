#include "chain.hh"

int main( void )
{
  chain::MatrixChain ch;
  size_t amount{};
  std::cin >> amount;

  for (size_t i = 0; i < amount; ++i)
  {
    size_t rw = 0, cl = 0;
    std::cin >> rw >> cl;

    ch.push(MX::Matrix<chain::ldbl>{rw, cl});
  }

  return 0;
}