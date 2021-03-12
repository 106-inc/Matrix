#include "driver/driver.hh"
#include "circuits.hh"

extern std::vector<CTS::Edge> Edges_;

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "USAGE: " << argv[0] << " FILENAME" << std::endl;
    return 0;
  }

  yy::Driver driver(argv[1]);

  if (!driver.parse())
    return 1;

  try 
  {
    CTS::Circuit crc{Edges_, driver.get_juncs()};
  }
  catch (const std::runtime_error &err)
  {
    std::cerr << err.what() << std::endl;
  }
  catch (const std::out_of_range &err)
  {
    std::cerr << err.what() << std::endl;
  }


  driver.dump();

  return 0;
}