#include "mem.hh"

namespace MX
{

/*
 *
 * Matrix class
 *
 */

using std::initializer_list;

template <typename DataT> class Matrix : VBuf<Row<DataT>>
{
  using VBuf<Row<DataT>>::arr_;
  using VBuf<Row<DataT>>::size_;
  using VBuf<Row<DataT>>::used_;

  using func = DataT (*)(size_t, size_t);

  size_t &rows_ = size_;
  size_t cols_{};

public:
  Matrix(size_t rows, size_t cols);
  Matrix(size_t rows, size_t cols, func action);
  Matrix(size_t rows, size_t cols, const initializer_list<DataT> &dat);
  Matrix(const Matrix &orig);

  template <typename It> Matrix(size_t rows, size_t cols, It beg, It end);

  void swap(Matrix &rhs) noexcept;

  size_t cols() const noexcept;
  size_t rows() const noexcept;

  const DataT &get(size_t row, size_t col) const;
  void set(size_t row, size_t col, DataT val);
  const Row<DataT> &operator[](size_t row) const;

  DataT det() const;

  Matrix &transpose() &;

  Matrix operator-() const;

  Matrix &operator=(const Matrix &orig);

  Matrix &operator+=(const Matrix &matr);
  Matrix &operator-=(const Matrix &matr);
  Matrix &operator*=(const Matrix &matr);
  Matrix &operator*=(DataT mul);

  bool operator==(const Matrix &matr) const;
  bool operator!=(const Matrix &matr) const;

  Matrix &swap_lines(size_t l1, size_t l2);
  Matrix &add_line(size_t to, size_t from, DataT mul);
  Matrix &mul_line(size_t l, DataT mul);

  Matrix &GaussFWD();
  Matrix &GaussBWD();

  bool sum_suitable(const Matrix<DataT> &matr) const;
};

template <typename DataT> Matrix<DataT> operator+(const Matrix<DataT> &lhs, const Matrix<DataT> &rhs);
template <typename DataT> Matrix<DataT> operator-(const Matrix<DataT> &lhs, const Matrix<DataT> &rhs);
template <typename DataT> Matrix<DataT> operator*(const Matrix<DataT> &lhs, const Matrix<DataT> &rhs);
template <typename DataT> Matrix<DataT> operator*(const Matrix<DataT> &matr, DataT mul);
template <typename DataT> Matrix<DataT> operator*(DataT mul, const Matrix<DataT> &matr);
template <typename DataT> Matrix<DataT> transpose(const Matrix<DataT> &matr);
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

template <typename DataT> Matrix<DataT>::Matrix(size_t rows, size_t cols, func action) : Matrix(rows, cols)
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

    if (std::abs(tmp[i][i]) > 1e-12) // TODO: EPSILON
      zero_col = false;
    else
      for (size_t j = i + 1; j < rows_; ++j)
        if (std::abs(tmp[j][i]) > 1e-12)
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
      if (std::abs(tmp[k][i]) < 1e-12) // TODO: EPSILON
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
      if (arr_[i][j] != matr.data_[i][j])
        return false;

  return true;
}

template <typename DataT> bool Matrix<DataT>::operator!=(const Matrix &matr) const
{
  return !operator==(matr);
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

template <typename DataT> Matrix<DataT> &Matrix<DataT>::GaussFWD()
{
  if (!std::is_floating_point<DataT>::value)
    throw std::bad_typeid();

  for (size_t i = 0; i < rows_; ++i)
  {
    bool zero_col = true;

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

    if (zero_col) // TODO: работает не в общем случае, но для кв. матрицы с ед. решением норм
      throw std::runtime_error("Here we need to swap cols");

    for (size_t k = i + 1; k < rows_; ++k)
    {
      if (std::abs(arr_[k][i]) < 1e-12) // TODO: EPSILON
        continue;

      DataT mul = arr_[k][i] / arr_[i][i];
      add_line(k, i, -mul);
    }
  }

  return *this;
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
      if (std::abs(arr_[k][i]) < 1e-12) // TODO: EPSILON
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

      if (std::abs(tmp) < 1e-12) // TODO: EPSILON, IS_ZERO
        ost << "0 ";
      else
        ost << tmp << " ";
    }

    ost << "|" << std::endl;
  }

  return ost;
}

} // namespace MX
