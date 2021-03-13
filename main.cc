#include "driver/driver.hh"
#include "circuits.hh"

extern std::vector<CTS::Edge> Edges_;

int main(int argc, char **argv)
{
  if (0 && argc < 2)
  {
    std::cout << "USAGE: " << argv[0] << " FILENAME" << std::endl;
    return 0;
  }

  yy::Driver driver("../tst/in/vl_test.dat");

  if (!driver.parse())
    return 1;

  CTS::Circuit crc{Edges_, driver.get_juncs()};
  crc.curs_calc();
  crc.curs_out();

  return 0;
}