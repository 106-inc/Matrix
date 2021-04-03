#include "chain.hh"

int main( void )
{
  chain::MatrixChain ch;

  for (int i = 0; i < 3; ++i)
  {
    size_t rw = 0, cl = 0;
    std::cin >> rw >> cl;

    ch.push(MX::Matrix<chain::ldbl>{rw, cl});
  }

  
  return 0;
}