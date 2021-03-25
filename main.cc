#include "circuits/circuits.hh"
#include "grammar/driver/driver.hh"

extern std::vector<CTS::Edge> Edges_;

int main()
{
  yy::Driver driver;

  if (!driver.parse())
    return 1;

  return 0;
}