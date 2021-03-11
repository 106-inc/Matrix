#ifndef MATRIX_HH
#define MATRIX_HH

#include "mem.hh"

namespace MX
{

/*
 *
 * Matrix class
 *
 */

using std::initializer_list;

static long double EPS = 1e-12;

/**
 * @brief Compares floating point numbers with zero
 * @param num Number you need to compare
 */
bool is_zero(long double num)
{
  return std::abs(num) < EPS;
}

/**
 * @class Template matrix
 */
template <typename DataT> class Matrix : public VBuf<Row<DataT>>
{

protected:
  using VBuf<Row<DataT>>::arr_;
  using VBuf<Row<DataT>>::size_;
  using VBuf<Row<DataT>>::used_;

  size_t &rows_ = size_;
  size_t cols_{};

public:
  /**
   * @brief Empty matrix ctor
   */
  Matrix(size_t rows, size_t cols);

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
   * @brief Add line indexed as 'from' multiplied by 'mul' to line indexed as 'to'
   * @param to Line which will be changed
   * @param from Line which will not be changed
   * @throw std::runtime_error if 'to' or 'from' is greater (or equal) then 'rows_'
   * @return Self reference
   */
  Matrix &add_line(size_t to, size_t from, DataT mul);

  /**
   * @brief Multiply line indexed as 'l' by mul
   * @param l Line index
   * @throw std::runtime_error if 'l' is greater (or equal) then 'rows_'
   * @return Self reference
   */
  Matrix &mul_line(size_t l, DataT mul);

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

protected:
  /**
   * @brief Backward part of Gauss method
   * @return Self reference
   */
  Matrix &GaussBWD();
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

template <typename DataT> Matrix<DataT>::Matrix(size_t rows, size_t cols) : VBuf<Row<DataT>>(rows), cols_(cols)
{
  Row<DataT> tmp{cols_};
  for (; used_ < size_; ++used_)
    copy_construct(arr_ + used_, tmp);
}

template <typename DataT>
template <typename Func>
Matrix<DataT>::Matrix(size_t rows, size_t cols, Func action) : Matrix(rows, cols)
{
  Matrix<DataT> tmp{rows_, cols_};

  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j)
      tmp.set(i, j, action(i, j));

  swap(tmp);
}

template <typename DataT>
Matrix<DataT>::Matrix(size_t rows, size_t cols, const initializer_list<DataT> &data) : Matrix(rows, cols)
{
  size_t elem_num = rows_ * cols_;

  auto cur = data.begin(), end = data.end();

  for (size_t i = 0; i < elem_num && cur != end; ++i, ++cur)
    arr_[i / cols_].set(i % cols_, *cur);
}

template <typename DataT> Matrix<DataT>::Matrix(const Matrix &orig) : Matrix(orig.rows_, orig.cols_)
{
  Matrix<DataT> tmp{rows_, cols_};

  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j)
      tmp.set(i, j, orig[i][j]);

  swap(tmp);
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
    throw std::out_of_range("Row index too big.");
  else if (col >= cols_)
    throw std::out_of_range("Column index too big.");

  return arr_[row].get(col);
}

template <typename DataT> void Matrix<DataT>::set(size_t row, size_t col, DataT val)
{
  if (row >= rows_)
    throw std::out_of_range("Row index too big.");
  else if (col >= cols_)
    throw std::out_of_range("Column index too big.");

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
    throw std::bad_typeid();

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
    for (size_t j = 0; j < cols_; ++j)
      tmp.set(i, j, tmp[i][j] + matr[i][j]);

  swap(tmp);
  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::operator-=(const Matrix &matr)
{
  if (cols_ != matr.cols_ || rows_ != matr.rows_)
    throw std::range_error("Wrong matrix size.");

  Matrix<DataT> tmp{*this};

  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j)
      tmp.set(i, j, tmp[i][j] - matr[i][j]);

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
      for (size_t k = 0; k < cols_; ++k)
        res.set(i, j, res[i][j] + arr_[i][k] * tmp[j][k]);

  swap(res);
  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::operator*=(DataT mul)
{
  Matrix<DataT> tmp{*this};

  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j)
      tmp.set(i, j, tmp[i][j] * mul);

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
  if (l1 >= cols_ || l2 >= cols_)
    throw std::range_error("Lines' numbers wrong.");

  std::swap(arr_[l1], arr_[l2]);

  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::add_line(size_t to, size_t from, DataT mul)
{
  if (to >= cols_ || from >= cols_)
    throw std::range_error("Lines' numbers wrong.");

  Matrix<DataT> tmp{*this};

  for (size_t i = 0; i < cols_; ++i)
    tmp.set(to, i, tmp[to][i] + mul * arr_[from][i]);

  swap(tmp);
  return *this;
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::mul_line(size_t l, DataT mul)
{
  if (l >= cols_)
    throw std::range_error("Line number wrong.");

  Matrix<DataT> tmp{*this};

  for (size_t i = 0; i < cols_; ++i)
    tmp.set(l, i, tmp[l][i] * mul);

  swap(tmp);

  return *this;
}

template <typename DataT> Matrix<DataT> Matrix<DataT>::GaussFWD() const
{
  Matrix<DataT> mat_cpy{*this};

  if (!std::is_floating_point<DataT>::value)
    throw std::bad_typeid();

  for (size_t i = 0; i < mat_cpy.rows_; ++i)
  {
    bool zero_col = true;

    if (!is_zero(mat_cpy[i][i]))
      zero_col = false;
    else
      for (size_t j = i + 1; j < rows_; ++j)
        if (!is_zero(mat_cpy[j][i]))
        {
          mat_cpy.swap_lines(j, i);
          zero_col = false;
          break;
        }

    if (zero_col) // TODO: работает не в общем случае, но для кв. матрицы с ед. решением норм
      throw std::runtime_error("Here we need to swap cols");

    for (size_t k = i + 1; k < rows_; ++k)
    {
      if (is_zero(mat_cpy[k][i]))
        continue;

      DataT mul = mat_cpy[k][i] / mat_cpy[i][i];
      mat_cpy.add_line(k, i, -mul);
    }
  }

  return mat_cpy;
}

template <typename DataT> Matrix<DataT> Matrix<DataT>::diag() const
{
  return GaussFWD(*this).GaussBWD();
}

template <typename DataT> Matrix<DataT> &Matrix<DataT>::GaussBWD()
{
  if (!std::is_floating_point<DataT>::value)
    throw std::bad_typeid();

  for (size_t i = 1; i < rows_; ++i)
  {
    /*bool zero_col = true;

    if (std::abs(arr_[i][i]) > 1e-12) // TODO: EPSILON
        zero_col = false;
    else
        for (size_t j = i + 1; j < rows_; ++j)
            if (std::abs(arr_[j][i]) > 1e-12)
            {
                swap_lines(j, i);
                zero_col = false;
                break;
            }

    if (zero_col)
        throw std::runtime_error("Here we need to swap cols");*/

    for (size_t k = 0; k < i; ++k)
    {
      if (is_zero(arr_[k][i]))
        continue;

      DataT mul = arr_[k][i] / arr_[i][i];
      add_line(k, i, -mul);
    }
  }
  return *this;
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
  for (size_t i = 0, cols = matr.cols(), rows = matr.rows(); i < rows; ++i)
  {
    ost << "| ";

    for (size_t j = 0; j < cols; ++j)
      ost << matr[i][j] << " ";

    ost << "|" << std::endl;
  }

  return ost;
}

template <> std::ostream &operator<<(std::ostream &ost, const Matrix<double> &matr)
{
  for (size_t i = 0, cols = matr.cols(), rows = matr.rows(); i < rows; ++i)
  {
    ost << "| ";

    for (size_t j = 0; j < cols; ++j)
    {
      auto tmp = matr[i][j];

      if (is_zero(tmp))
        ost << "0 ";
      else
        ost << tmp << " ";
    }

    ost << "|" << std::endl;
  }

  return ost;
}

} // namespace MX

#endif // MATRIX_HH
