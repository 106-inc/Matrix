#include "circuits.hh"
#include "driver.hh"

extern std::vector<CTS::Edge> Edges_;

int main()
{
  yy::Driver driver;

  if (!driver.parse())
    return 1;

  driver.solve();

  return 0;
}