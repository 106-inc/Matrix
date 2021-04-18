#ifndef MATRIX_HH
#define MATRIX_HH

#include "mem.hh"

#include <exception>
#include <iomanip>
#include <vector>

namespace MX
{

/*
 *
 * Matrix class
 *
 */

using std::initializer_list;

const long double EPS = 1e-12;

struct rank_lack : public std::runtime_error
{
  rank_lack(const char *msg) : std::runtime_error(msg)
  {
  }
};

/**
 * @brief Compares floating point numbers with zero
 * @param num Number you need to compare
 */
bool is_zero(long double num);

/**
 * @class Template matrix
 */
template <typename DataT> class Matrix : public VBuf<Row<DataT>>
{

private:
  using VBuf<Row<DataT>>::arr_;
  using VBuf<Row<DataT>>::size_;
  using VBuf<Row<DataT>>::used_;

  size_t &rows_ = size_;
  size_t cols_{};

public:
  /**
   * @brief Empty matrix ctor
   */
  explicit Matrix(size_t rows = 0, size_t cols = 0);

  /**
   * @brief Matrix ctor with data
   * @param dat List of starting data
   */
  Matrix(size_t rows, size_t cols, const initializer_list<DataT> &dat);

  /**
   * @brief Copy ctor
   */
  Matrix(const Matrix &orig);

  /**
   * @brief Iterator filling ctor
   * @param beg Start iterator
   * @param end Start iterator
   */
  template <typename It> Matrix(size_t rows, size_t cols, It beg, It end);

  /**
   * @brief Function filling ctor
   * @param action Filling function/functor/lamda
   */
  template <typename Func> Matrix(size_t rows, size_t cols, Func action);
  
  static Matrix Identity(size_t rows, size_t cols)
  {
    return {rows, cols, [](size_t i, size_t j){ return static_cast<DataT>(i == j); }};
  }

  /**
   * @brief Swap this matrix with rhs
   * @param rhs Other matrix
   */
  void swap(Matrix &rhs) noexcept;

  size_t cols() const noexcept;
  size_t rows() const noexcept;

  /**
   * @brief Get elem by coordinates
   * @throw std::out_of_range if 'row' or 'col' bigger than matrix size
   * @return Matrix elem copy
   */
  const DataT &get(size_t row, size_t col) const;

  /**
   * @brief Set elem by coordinates
   * @throw std::out_of_range if 'row' or 'col' bigger than matrix size
   * @param val New elem value
   */
  void set(size_t row, size_t col, DataT val);

  /**
   * @brief Get row by number
   * @throw std::out_of_range if 'row' or 'col' bigger than matrix size
   * @return Matrix elem copy
   */
  const Row<DataT> &operator[](size_t row) const;

  /**
   * @brief Calculate and return determinant
   * @throw std::bad_typeid if DataT is not floating point type,
   *        std::range_error if matrix is not square
   * @return Determinant
   */
  DataT det() const;

  /**
   * @brief Self transpose method
   * @return Self reference
   */
  Matrix &transpose() &;

  Matrix operator-() const;

  Matrix &operator=(const Matrix &orig);

  /**
   * @throw std::range_error if 'this' and 'matr' is not add suitable
   * @return Self reference
   */
  Matrix &operator+=(const Matrix &matr);

  /**
   * @throw std::range_error if 'this' and 'matr' is not sub suitable
   * @return Self reference
   */
  Matrix &operator-=(const Matrix &matr);

  /**
   * @throw std::range_error if 'this' and 'matr' is not mul suitable
   * @return Self reference
   */
  Matrix &operator*=(const Matrix &matr);

  Matrix &operator*=(DataT mul);

  bool operator==(const Matrix &matr) const;
  bool operator!=(const Matrix &matr) const;

  /**
   * @brief Concatenate this matrix with matr by down side
   * @param matr Matrix to concatenate with
   * @return Self reference
   */
  Matrix &glue_dn(const Matrix &matr);

  /**
   * @brief Concatenate this matrix with matr by right side
   * @param matr Matrix to concatenate with
   * @return Self reference
   */
  Matrix &glue_rt(const Matrix &matr);

  /**
   * @brief Swap line indexed as 'l1' and line indexed as 'l2'
   * @param l1 First row
   * @param l2 Second row
   * @throw std::runtime_error if l1 or l2 is greater (or equal) then rows_
   * @return Self reference
   */
  Matrix &swap_lines(size_t l1, size_t l2);

  /**
   * @brief Swap line indexed as 'l1' and line indexed as 'l2'
   * @param l1 First row
   * @param l2 Second row
   * @throw std::runtime_error if l1 or l2 is greater (or equal) then rows_
   * @return Self reference
   */
  Matrix &swap_columns(size_t l1, size_t l2);

  /**
   * @brief Add line indexed as 'from' multiplied by 'mul' to line indexed as 'to'
   * @param to Line which will be changed
   * @param from Line which will not be changed
   * @throw std::runtime_error if 'to' or 'from' is greater (or equal) then 'rows_'
   * @return Self reference
   */
  Matrix &add_line(size_t to, size_t from, DataT mul);

  /**
   * @brief Add column indexed as 'from' multiplied by 'mul' to column indexed as 'to'
   * @param to column which will be changed
   * @param from column which will not be changed
   * @throw std::runtime_error if 'to' or 'from' is greater (or equal) then 'rows_'
   * @return Self reference
   */
  Matrix &add_column(size_t to, size_t from, DataT mul);

  /**
   * @brief Multiply line indexed as 'l' by mul
   * @param l Line index
   * @throw std::runtime_error if 'l' is greater (or equal) then 'rows_'
   * @return Self reference
   */
  Matrix &mul_line(size_t l, DataT mul);

  Matrix &del_line(size_t line_num);

  /**
   * @brief Check if this and matr is suitable for addition
   * @return True if suitable, False otherwise
   */
  bool sum_suitable(const Matrix<DataT> &matr) const;

  /**
   * @brief Forward part of Gauss method
   * @return Self reference
   */
  Matrix GaussFWD() const;

  /**
   * @brief Diagonalise matrix function
   * @param mat reference to matrix
   * @return Created matrix.
   */
  Matrix diag() const;

  static std::vector<double> solve(const Matrix &mat)
  {
    Matrix tmp = mat.diag();

    std::vector<double> res;
    res.reserve(tmp.rows_);

    for (size_t i = 0, col_idx = tmp.cols_ - 1; i < col_idx; ++i)
      res.push_back(tmp[i][col_idx] / tmp[i][i]);

    return res;
  }
};

template <typename DataT> Matrix<DataT> operator+(const Matrix<DataT> &lhs, const Matrix<DataT> &rhs);
template <typename DataT> Matrix<DataT> operator-(const Matrix<DataT> &lhs, const Matrix<DataT> &rhs);
template <typename DataT> Matrix<DataT> operator*(const Matrix<DataT> &lhs, const Matrix<DataT> &rhs);
template <typename DataT> Matrix<DataT> operator*(const Matrix<DataT> &matr, DataT mul);
template <typename DataT> Matrix<DataT> operator*(DataT mul, const Matrix<DataT> &matr);
template <typename DataT> Matrix<DataT> transpose(const Matrix<DataT> &matr);
template <typename DataT> Matrix<DataT> glue_side(const Matrix<DataT> &matr1, const Matrix<DataT> &matr2);
template <typename DataT> Matrix<DataT> glue_bott(const Matrix<DataT> &matr1, const Matrix<DataT> &matr2);
template <typename DataT> std::ostream &operator<<(std::ostream &ost, const Matrix<DataT> &matr);

/*
 *
 * Matrix class method realisations
 *
 */

template <typename DataT>
Matrix<DataT>::Matrix(size_t rows /* = 0 */, size_t cols /* = 0 */) : VBuf<Row<DataT>>(rows), cols_(cols)
{
  Row<DataT> tmp{cols_};
  for (; used_ < size_; ++used_)
    copy_construct(arr_ + used_, tmp);
}

template <typename DataT>
template <typename Func>
Matrix<DataT>::Matrix(size_t rows, size_t cols, Func action) : Matrix(rows, cols)
{
  // Matrix<DataT> tmp{rows_, cols_};

  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j)
      /*tmp.*/ set(i, j, action(i, j));

  /*swap(tmp);*/
}

template <typename DataT>
Matrix<DataT>::Matrix(size_t rows, size_t cols, const initializer_list<DataT> &data) : Matrix(rows, cols)
{
  size_t elem_num = rows_ * cols_;

  auto cur = data.begin(), end = data.end();

  for (size_t i = 0; i < elem_num && cur != end; ++i, ++cur)
    arr_[i / cols_].set(i % cols_, *cur);
}

template <typename DataT> Matrix<DataT>::Matrix(const Matrix &orig) : VBuf<Row<DataT>>(orig.rows_), cols_(orig.cols_)
{
  Row<DataT> tmp{cols_};
  for (; used_ < size_; ++used_)
    copy_construct(arr_ + used_, orig[used_]);
}

template <typename DataT>
template <typename It>
Matrix<DataT>::Matrix(size_t rows, size_t cols, It beg, It end) : Matrix(rows, cols)
{
  size_t i = 0;
  size_t elem_num = rows_ * cols_;

  for (It cur = beg; cur != end && i < elem_num; ++i, ++cur)
    arr_[i / cols_].set(i % cols_, *cur);
}

template <typename DataT> void Matrix<DataT>::swap(Matrix<DataT> &rhs) noexcept
{
  VBuf<Row<DataT>>::swap(rhs);
  std::swap(cols_, rhs.cols_);
}

template <typename DataT> const DataT &Matrix<DataT>::get(size_t row, size_t col) const
{
  if (row >= rows_)
    throw std::out_of_range("Getter's row index too big.");

  return arr_[row].get(col);
}

template <typename DataT> void Matrix<DataT>::set(size_t row, size_t col, DataT val)
{
  if (row >= rows_)
    throw std::out_of_range("Setter's row index too big.");

  arr_[row].set(col, val);
}

template <typename DataT> const Row<DataT> &Matrix<DataT>::operator[](size_t row) const
{
  if (row >= rows_)
    throw std::out_of_range("Row index too big.");

  return arr_[row];
}

template <typename DataT> size_t Matrix<DataT>::cols() const noexcept
{
  return cols_;
}

template <typename DataT> size_t Matrix<DataT>::rows() const noexcept
{
  return rows_;
}

template <typename DataT> DataT Matrix<DataT>::det() const
{
  if (!std::is_floating_point<DataT>::value)
    throw std::runtime_error("det() method only for floating point types");

  if ((cols_ != rows_))
    throw std::range_error("Matrix should be square.");

  long double sign = 1;

  Matrix<DataT> tmp{*this};

  for (size_t i = 0; i < cols_; ++i)
  {
    bool zero_col = true;

    if (!is_zero(tmp[i][i]))
      zero_col = false;
    else
      for (size_t j = i + 1; j < rows_; ++j)
        if (!is_zero(tmp[j][i]))
        {
          tmp.swap_lines(j, i);
          zero_col = false;
          sign = -sign;
          break;
        }

    if (zero_col)
      return 0;

    for (size_t k = i + 1; k < rows_; ++k)
    {
      if (is_zero(tmp[i][i]))
        continue;

      long double mul = tmp[k][i] / tmp[i][i];
      tmp.add_line(k, i, -mul);
    }
  }

  long double res = sign;
  for (size_t i = 0; i < cols_; ++i)
    res *= tmp[i][i];

  return res;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::transpose() &
{
  Matrix<DataT> tmp{cols_, rows_};

  for (size_t i = 0; i < tmp.rows_; ++i)
    for (size_t j = 0; j < tmp.cols_; ++j)
      std::swap(tmp.arr_[i].arr_[j], arr_[j].arr_[i]);

  swap(tmp);
  return *this;
}

template <typename DataT> Matrix<DataT> Matrix<DataT>::operator-() const
{
  Matrix<DataT> res{rows_, cols_};

  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j)
      res.set(i, j, -arr_[i][j]);

  return res;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::operator=(const Matrix &orig)
{
  Matrix<DataT> tmp{orig};

  swap(tmp);
  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::operator+=(const Matrix &matr)
{
  if (cols_ != matr.cols_ || rows_ != matr.rows_)
    throw std::range_error("Wrong matrix size.");

  Matrix<DataT> tmp{*this};

  for (size_t i = 0; i < rows_; ++i)
    tmp.arr_[i] += matr.arr_[i];

  swap(tmp);
  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::operator-=(const Matrix &matr)
{
  if (cols_ != matr.cols_ || rows_ != matr.rows_)
    throw std::range_error("Wrong matrix size.");

  Matrix<DataT> tmp{*this};

  for (size_t i = 0; i < rows_; ++i)
    tmp.arr_[i] -= matr.arr_[i];

  swap(tmp);
  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::operator*=(const Matrix &matr)
{
  if (cols_ != matr.rows_)
    throw std::range_error("Wrong matrix size.");

  Matrix<DataT> res{rows_, matr.cols_};
  Matrix<DataT> tmp{matr};
  tmp.transpose();

  for (size_t i = 0; i < res.rows_; ++i)
    for (size_t j = 0; j < res.cols_; ++j)
    {
      int k = 0;
      for (int end = static_cast<int>(cols_) - 8; k < end; k += 8)
        res.set(i, j,
                res.arr_[i][j] + arr_[i][k] * tmp.arr_[j][k] + arr_[i][k + 1] * tmp.arr_[j][k + 1] +
                    arr_[i][k + 2] * tmp.arr_[j][k + 2] + arr_[i][k + 3] * tmp.arr_[j][k + 3] +
                    arr_[i][k + 4] * tmp.arr_[j][k + 4] + arr_[i][k + 5] * tmp.arr_[j][k + 5] +
                    arr_[i][k + 6] * tmp.arr_[j][k + 6] + arr_[i][k + 7] * tmp.arr_[j][k + 7]);
      while (k < static_cast<int>(cols_))
      {
        res.set(i, j, res.arr_[i][j] + arr_[i][k] * tmp.arr_[j][k]);
        ++k;
      }
    }

  swap(res);
  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::operator*=(DataT mul)
{
  Matrix<DataT> tmp{*this};

  for (size_t i = 0; i < rows_; ++i)
    tmp.arr_[i] *= mul;

  swap(tmp);

  return *this;
}

template <typename DataT> bool Matrix<DataT>::operator==(const Matrix &matr) const
{
  if (rows_ != matr.rows_ || cols_ != matr.cols_)
    return false;

  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j)
      if (arr_[i][j] != matr.arr_[i][j])
        return false;

  return true;
}

template <typename DataT> bool Matrix<DataT>::operator!=(const Matrix &matr) const
{
  return !operator==(matr);
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::glue_dn(const Matrix &matr)
{
  if (cols_ != matr.cols_)
    throw std::runtime_error("Cols num doesn't match");

  Matrix<DataT> tmp{rows_ + matr.rows_, cols_};

  for (size_t i = rows_; i < tmp.rows_; ++i)
    tmp.arr_[i] = matr.arr_[i - rows_];

  swap(tmp);

  for (size_t i = 0; i < tmp.rows_; ++i)
    std::swap(arr_[i], tmp.arr_[i]);

  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::glue_rt(const Matrix &matr)
{
  if (rows_ != matr.rows_)
    throw std::runtime_error("Rows num doesn't match");

  Matrix<DataT> this_tr{MX::transpose(*this)};
  Matrix<DataT> matr_tr{MX::transpose(matr)};

  this_tr.glue_dn(matr_tr);

  swap(this_tr.transpose());

  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::swap_lines(size_t l1, size_t l2)
{
  if (l1 >= rows_ || l2 >= rows_)
    throw std::range_error("Lines' numbers wrong.");

  std::swap(arr_[l1], arr_[l2]);

  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::swap_columns(size_t c1, size_t c2)
{
  if (c1 >= rows_ || c2 >= rows_)
    throw std::range_error("Columns' numbers wrong.");

  transpose();
  std::swap(arr_[c1], arr_[c2]);
  transpose();

  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::add_line(size_t to, size_t from, DataT mul)
{
  if (to >= rows_ || from >= rows_)
    throw std::range_error("Lines' numbers wrong.");

  Matrix<DataT> tmp{*this};

  tmp.arr_[to] += (tmp.arr_[from] * mul);

  swap(tmp);
  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::add_column(size_t to, size_t from, DataT mul)
{
  if (to >= cols_ || from >= cols_)
    throw std::range_error("Cols' numbers wrong.");

  transpose();
  add_line(to, from, mul);
  transpose();

  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::mul_line(size_t l, DataT mul)
{
  if (l >= cols_)
    throw std::range_error("Line number wrong.");

  Matrix<DataT> tmp{*this};

  tmp.arr_[l] *= mul;

  swap(tmp);

  return *this;
}

template <typename DataT> Matrix<DataT> Matrix<DataT>::GaussFWD() const
{
  Matrix<DataT> mat_cpy{*this};

  if (!std::is_floating_point<DataT>::value)
    throw std::runtime_error("GaussFWD method only for floating point types");

  for (size_t d_idx = 0, end = std::min(rows_, cols_ - 1); d_idx < end; ++d_idx)
  {
    if (is_zero(mat_cpy[d_idx][d_idx]))
    {
      for (size_t down = d_idx + 1; down < rows_; ++down)
        if (!is_zero(mat_cpy[down][d_idx]))
        {
          mat_cpy.swap_lines(down, d_idx);
          break;
        }

      if (is_zero(mat_cpy[d_idx][d_idx]))
        throw rank_lack("Matrix rank less then number of cols");
    }

    mat_cpy.mul_line(d_idx, 1.0 / mat_cpy[d_idx][d_idx]);

    for (size_t down = d_idx + 1; down < rows_; ++down)
      mat_cpy.add_line(down, d_idx, -mat_cpy[down][d_idx]);

    for (int up = static_cast<int>(d_idx) - 1; up >= 0; --up)
      mat_cpy.add_line(up, d_idx, -mat_cpy[up][d_idx]);
  }

  return mat_cpy;
}

template <typename DataT> Matrix<DataT> Matrix<DataT>::diag() const
{
  return GaussFWD();
}

template <typename DataT> bool Matrix<DataT>::sum_suitable(const Matrix<DataT> &matr) const
{
  return (cols_ == matr.cols_) && (rows_ == matr.rows_);
}

template <typename DataT> Matrix<DataT> operator+(const Matrix<DataT> &lhs, const Matrix<DataT> &rhs)
{
  return Matrix<DataT>{lhs} += rhs;
}

template <typename DataT> Matrix<DataT> operator-(const Matrix<DataT> &lhs, const Matrix<DataT> &rhs)
{
  return Matrix<DataT>{lhs} -= rhs;
}

template <typename DataT> Matrix<DataT> operator*(const Matrix<DataT> &lhs, const Matrix<DataT> &rhs)
{
  return Matrix<DataT>{lhs} *= rhs;
}

template <typename DataT> Matrix<DataT> operator*(const Matrix<DataT> &matr, DataT mul)
{
  return Matrix<DataT>{matr} *= mul;
}

template <typename DataT> Matrix<DataT> operator*(DataT mul, const Matrix<DataT> &matr)
{
  return Matrix<DataT>{matr} *= mul;
}

template <typename DataT> Matrix<DataT> transpose(const Matrix<DataT> &matr)
{
  Matrix<DataT> tmp{matr};
  tmp.transpose();
  return tmp;
}

template <typename DataT> Matrix<DataT> glue_side(const Matrix<DataT> &matr1, const Matrix<DataT> &matr2)
{
  return Matrix<DataT>{matr1}.glue_rt(matr2);
}

template <typename DataT> Matrix<DataT> glue_bott(const Matrix<DataT> &matr1, const Matrix<DataT> &matr2)
{
  return Matrix<DataT>{matr1}.glue_dn(matr2);
}

template <typename DataT> std::ostream &operator<<(std::ostream &ost, const Matrix<DataT> &matr)
{
  ost << "   | ";
  for (size_t i = 0, cols = matr.cols(); i < cols; ++i)
    ost << std::setw(4) << i;

  ost << std::endl;

  ost << "   +-";
  for (size_t i = 0, cols = matr.cols(); i < cols; ++i)
    ost << "----";

  ost << std::endl;

  for (size_t i = 0, cols = matr.cols(), rows = matr.rows(); i < rows; ++i)
  {
    ost << std::setw(3) << i << "| ";

    for (size_t j = 0; j < cols; ++j)
      ost << std::setw(4) << matr[i][j] << ";";

    ost << "|" << std::endl;
  }

  return ost;
}

template <> std::ostream &operator<<(std::ostream &ost, const Matrix<double> &matr);

} // namespace MX

#endif // MATRIX_HH
