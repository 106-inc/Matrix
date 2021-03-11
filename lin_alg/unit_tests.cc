#include "gtest/gtest.h"
#include <iostream>

#include "matrix.hh"
#include "la.hh"

using std::cin;
using std::cout;
using std::endl;
using std::string;

using namespace MX;
using namespace LA;

TEST(la, det)
{
  Matrix<double> m1{2, 2, {1, 1, 0, 3}};

  EXPECT_DOUBLE_EQ(m1.det(), 3);
}

TEST(matr, cout)
{
  Matrix<double> m1{3, 4, {1.5, 2, 3.2, 6.7, 0, 6.3, 3.5, 0, 1, 5.6, 7.1, 7}};
  std::cout << m1 << std::endl;
}

TEST(la, solve)
{
    Sys s{2, 3, {0, 1, 7,
                 1, 0, 4}};

    auto res = s.solve();

    EXPECT_DOUBLE_EQ(res[0], 4);
    EXPECT_DOUBLE_EQ(res[1], 7);
}

TEST(lamda, eye)
{
  auto func = [](int i, int j)
          { return (i == j); };

  Matrix<double> m1{4, 4, func};

  Matrix<double> m2{4, 4, {1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, 1, 0,
                           0, 0, 0, 1}};

  EXPECT_EQ(m1, m2);
}

TEST(glue, down)
{
    Matrix<double> m1{1, 2, {1, 1}};
    Matrix<double> m2{2, 2, {2, 2,
                             3, 3}};

    std::cout << m1.glue_dn(m2) << std::endl;
}

TEST(glue, right)
{
    Matrix<double> m1{2, 1, {1, 1}};
    Matrix<double> m2{2, 2, {2, 3,
                             2, 3}};

    std::cout << m1.glue_rt(m2) << std::endl;
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
