#ifndef MATRIX_MEM_HH
#define MATRIX_MEM_HH

#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace Buf
{
    template <typename T> void copy_construct(T *ptr, const T &val)
    {
        new (ptr) T{val};
    }

    template <class T> void destr(T *ptr)
    {
        ptr->~T();
    }

    template <typename It> void destr(It beg, It end)
    {
        while (beg != end)
            destr(&*beg++);
    }

/*
 *
 * VBuf class
 *
 */

    template <typename DataT> struct VBuf
    {
        VBuf(const VBuf &) = delete;

        VBuf &operator=(const VBuf &) = delete;

    protected:
        DataT *arr_;
        size_t size_, used_;

        explicit VBuf(size_t size = 0);

        ~VBuf();

        void swap(VBuf &rhs) noexcept;
    };

/*
 *
 * VBuf class methods realistaions
 *
 */

    template <typename DataT>
    VBuf<DataT>::VBuf(size_t size)
            : arr_(size == 0 ? nullptr : static_cast<DataT *>(::operator new(sizeof(DataT) * size))), size_(size), used_(0)
    {
    }

    template <typename DataT> VBuf<DataT>::~VBuf()
    {
        destr(arr_, arr_ + used_);
        ::operator delete(arr_);
    }

    template <typename DataT> void VBuf<DataT>::swap(VBuf &rhs) noexcept
    {
        std::swap(arr_, rhs.arr_);
        std::swap(size_, rhs.size_);
        std::swap(used_, rhs.used_);
    }
} // namespace Buf

namespace MX
{
    using namespace Buf;

    template <typename DataT>
    class Matrix;

/*
 *
 * Row class
 *
 */

    template <typename DataT>
    struct Row : private VBuf<DataT>
    {
        using VBuf<DataT>::arr_;
        using VBuf<DataT>::size_;
        using VBuf<DataT>::used_;

        friend class Matrix<DataT>;

        explicit Row( size_t size );

        Row( const Row & rhs );

        template <typename It>
        Row( size_t size, It beg, It end );

        Row & operator =( const Row & rhs );

    public:
        const DataT & get( size_t idx ) const;

        void set( size_t idx, DataT new_val );

        const DataT & operator []( size_t idx ) const;
    };

/*
 *
 * Row class method realisations
 *
 */

    template <typename DataT>
    Row<DataT>::Row( size_t size ) : VBuf<DataT>(size)
    {
        DataT tmp{};
        for (; used_ < size_; ++used_)
            copy_construct(arr_ + used_, tmp);
    }

    template <typename DataT>
    Row<DataT>::Row( const Row & rhs ) : VBuf<DataT>(rhs.size_)
    {
        for (; used_ < rhs.used_; ++used_)
            copy_construct(arr_ + used_, *(rhs.arr_ + used_));

        DataT tmp{};
        for (; used_ < size_; ++used_)
            copy_construct(arr_ + used_, tmp);
    }

    template <typename DataT>
    template <typename It>
    Row<DataT>::Row( size_t size, It beg, It end ) : VBuf<DataT>(size)
    {
        It cur = beg;

        for (; used_ < size_ && cur != end; ++used_, ++cur)
            copy_construct(arr_ + used_, *cur);

        DataT tmp{};
        for (; used_ < size_; ++used_)
            copy_construct(arr_ + used_, tmp);
    }

    template <typename DataT>
    Row<DataT> & Row<DataT>::operator =( const Row & rhs )
    {
        Row tmp{rhs};
        //------------ Kalb line
        this->swap(tmp);
        return *this;
    }

    template <typename DataT>
    const DataT & Row<DataT>::get( size_t idx ) const
    {
        if (idx >= size_)
            throw std::out_of_range("Get index too big.");

        return arr_[idx];
    }

    template <typename DataT>
    void Row<DataT>::set( size_t idx, DataT new_val )
    {
        if (idx >= size_)
            throw std::out_of_range("Set index too big.");

        arr_[idx] = new_val;
    }

    template <typename DataT>
    const DataT & Row<DataT>::operator []( size_t idx ) const
    {
        return get(idx);
    }
}


#endif //MATRIX_MEM_HH
