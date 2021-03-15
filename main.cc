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

  crc.curs_calc();
  crc.curs_out();

  return 0;
}