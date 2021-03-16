#include "circuits.hh"
#include "driver/driver.hh"

extern std::vector<CTS::Edge> Edges_;

int main()
{
  yy::Driver driver;

  if (!driver.parse())
    return 1;

  CTS::Circuit crc{Edges_, driver.get_juncs(), driver.get_loops()};
  // crc.dump("large_norm.png", "norm.dot");

  try
  {
    crc.curs_calc();
    crc.curs_out();
  }
  catch (const MX::rank_lack &er)
  {
    std::cerr << "I CAN'T CALCULATE THIS CIRCUIT, SORRY :'(" << std::endl;
  }

  return 0;
}