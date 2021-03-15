#include "circuits.hh"
#include "driver/driver.hh"

extern std::vector<CTS::Edge> Edges_;

int main(int argc, char **argv)
{
  /*if (argc < 2)
  {
    std::cout << "USAGE: " << argv[0] << " FILENAME" << std::endl;
    return 0;
  }*/

  yy::Driver driver;//(/*argv[1]*/);

  if (!driver.parse())
    return 1;

  CTS::Circuit crc{Edges_, driver.get_juncs(), driver.get_loops()};
  // crc.dump("large_norm.png", "norm.dot");

  crc.curs_calc();
  crc.curs_out();

  return 0;
}