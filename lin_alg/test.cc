#include "gtest/gtest.h"
#include <iostream>

#include "matrix.hh"

using std::cin;
using std::cout;
using std::endl;
using std::string;

using namespace MX;

TEST(la, det)
{
  Matrix<double> m1{2, 2, {1, 1, 0, 3}};

  EXPECT_DOUBLE_EQ(m1.det(), 3);
}

TEST(matr, cout)
{
  Matrix<double> m1{3, 4, {1.5, 2, 3.2, 6.7,
                           0, 6.3, 3.5, 0,
                           1, 5.6, 7.1, 7}};
  std::cout << m1 << std::endl;
}

TEST(Gauss, FWD)
{
    Matrix<double> m1{3, 4, {1.5, 2, 3.2, 6.7,
                             0, 6.3, 3.5, 0,
                             1, 5.6, 7.1, 7}};

    std::cout << m1.GaussFWD() << std::endl;
}

TEST(Gauss, BWD)
{
    Matrix<double> m1{2, 2, {1.5, 2,
                                    0, 6.3,}};

    std::cout << m1.GaussBWD() << std::endl;
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
