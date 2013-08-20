// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2006 Benoit Jacob <jacob@math.jussieu.fr>
//
// Eigen is free software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along
// with Eigen; if not, write to the Free Software Foundation, Inc., 51
// Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. This exception does not invalidate any other reasons why a work
// based on this file might be covered by the GNU General Public License.


#ifndef EIGEN_MATRIXBASE_H
#define EIGEN_MATRIXBASE_H

#include "util.h"

namespace Eigen
{

/** \ingroup internalbases
  *
  * \ingroup matrices
  *
  * \brief internal base class
  *
  * This class template is only internally used in Eigen.
  * This provides the base that the Matrix and MatrixX
  * class templates inherit.
  *
  * Note that one template parameter is Derived: this is a
  * C++ trick knows as Curiously Recursive Template Pattern.
  * Here, it allows us to implement in MatrixBase the code
  * of both Matrix and MatrixX, which are very different
  * (Matrix stores the size/size of the matrix as template argument,
  * while MatrixX stores them as member data, thus storing the
  * array itself on the heap).
  */
template< typename T,
          typename Derived,
          typename VectorType,
          typename LUDecompositionType >
class MatrixBase
{

private:

    /** \internal
      * Helper method, computes *this + other and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void addition_helper
    ( const Derived & other, Derived * res ) const
    {
        for( int i = 0; i < res->size() * res->size(); i++ )
            (*res)[i] = (*this)[i] + other[i];
    }
    
    /** \internal
      * Helper method, computes *this - other and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void substraction_helper
    ( const Derived & other, Derived * res ) const
    {
        for( int i = 0; i < res->size() * res->size(); i++ )
            (*res)[i] = (*this)[i] - other[i];
    }
    
    /** \internal
      * Helper method, computes *this * factor and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void scalar_multiplication_helper
    ( const T & factor, Derived * res ) const
    {
        for( int i = 0; i < res->size() * res->size(); i++ )
            (*res)[i] = (*this)[i] * factor;
    }

    /** \internal
      * Helper method, computes *this / factor and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    inline void scalar_division_helper
    ( const T & factor, Derived * res ) const
    {
        scalar_multiplication_helper( static_cast<const T>(1) / factor, res );
    }

    /** \internal
      * Helper method, computes *this * other and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void matrix_multiplication_helper
    ( const Derived & other, Derived * res ) const;

    /** \internal
      * Helper method, computes *this * vector and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void vector_multiplication_helper
    ( const VectorType & vector, VectorType * res ) const;

public:

    /**
      * Computes the matrix product *this * other and stores the
      * result into *res.
      *
      * For dynamic-size matrices, this method resizes *res if necessary.
      * For fixed-size matrices, it is required that *res already has the
      * right size, that is: res->size() == this->size().
      *
      * This method is faster than operator* and operator*=.
      *
      * Note for dynamic-size matrices:
      * For optimal performance, the matrix res should be
      * initialized with the correct size before calling this method,
      * otherwise it will have to be resized, which is costly.
      *
      * Note that for the product to make sense, it is required
      * that this->size() == other.size().
      */
    inline void multiply( const Derived & other, Derived * res ) const
    {
        EIGEN_ASSERT_NOABORT( this->size() == other.size() );
        res->resize( this->size() );
        matrix_multiplication_helper( other, & res );
    }

    /**
      * Returns the matrix product *this * other.
      *
      * Note that for the product to make sense, it is required
      * that this->size() == other.size().
      */
    Derived operator * ( const Derived & other ) const
    {
        EIGEN_ASSERT_NOABORT( this->size() == other.size() );
        Derived res( this->size() );
        matrix_multiplication_helper( other, & res );
        return res;
    }

    /**
      * Matrix multiplication: does *this = *this * other.
      *
      * This only makes sense if this->size() == other.size().
      */
    Derived & operator *=( const Derived & other )
    {
        EIGEN_ASSERT_NOABORT( this->size() == other.size() );
        Derived res( this->size() );
        matrix_multiplication_helper( other, & res );
        return( *this = res );
    }

    /**
      * @returns true if the matrix has dynamic size (i.e. is an
      * object of class MatrixX), false if the matrix has fixed size
      * (i.e. is an object of class Matrix).
      */
    static inline bool hasDynamicSize()
    {
        return Derived::_hasDynamicSize();
    }

    /**
      * @returns the size (number of rows, or equivalently number of
      * columns) of the matrix.
      */
    inline int size() const
    {
        return static_cast<const Derived*>(this)->_size();
    }

    /**
      * Resizes the matrix. That is only possible if the matrix
      * has dynamic size, i.e. is an object of class MatrixX.
      *
      * Resizing a fixed-size matrix is not possible, and attempting
      * to do so will only generate a debug message (unless the new size
      * equals the old one).
      *
      * The matrix entries are not kept, they
      * are left with undefined values after resizing.
      */
    inline void resize( int newsize )
    {
        static_cast<Derived*>(this)->_resize( newsize );
    }

    /**
      * @returns the array of the matrix, as constant.
      */
    inline const T * array() const
    {
        return static_cast<const Derived*>(this)->m_array;
    }

    /**
      * @returns the array of the matrix, as non-constant.
      */
    inline T * array()
    {
        return static_cast<Derived*>(this)->m_array;
    }

    /**
      * @returns a constant reference to the entry of the matrix at
      * given row and column.
      */
    inline const T & operator () ( int row, int col ) const
    {
        return array() [ row + col * size() ];
    }

    /**
      * @returns a non-constant reference to the entry of the matrix at
      * given row and column.
      */
    inline T & operator () ( int row, int col )
    {
        return array() [ row + col * size() ];
    }

    /**
      * @returns a constant reference to the i-th entry of the array
      * of the matrix (which stores the matrix entries in column-major order).
      */
    inline const T & operator [] ( int i ) const
    {
        return array() [i];
    }

    /**
      * @returns a non-constant reference to the i-th entry of the array
      * of the matrix (which stores the matrix entries in column-major order).
      */
    inline T & operator [] ( int i )
    {
        return array() [i];
    }

    /**
      * Copies other into *this. If *this has dynamic size,
      * it will get resized if necessary. If *this has static size,
      * it is required that other has the same size.
      */
    Derived & operator = ( const Derived & other );

    /**
      * Stores *this + other into *this (entry-wise addition).
      *
      * *this and other must have the same size.
      */
    inline Derived & operator += ( const Derived & other )
    {
        EIGEN_ASSERT_NOABORT( other.size() == this->size() );
        addition_helper( other, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Stores *this - other into *this (entry-wise substraction).
      *
      * *this and other must have the same size.
      */
    inline Derived & operator -= ( const Derived & other )
    {
        EIGEN_ASSERT_NOABORT( other.size() == this->size() );
        substraction_helper( other, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Stores *this * factor into *this (multiplication of each entry).
      */
    inline Derived & operator *= ( const T & factor )
    {
        scalar_multiplication_helper( factor, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Stores *this / factor into *this (division of each entry).
      */
    inline Derived & operator /= ( const T & factor )
    {
        scalar_division_helper( factor, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Reads the entries of *this from an array. The number of entries
      * read from the array is equal to size()*size().
      */
    void readArray( const T * src )
    {
        for (int i = 0; i < size() * size(); i++)
            (*this)[i] = src[i];
    }

    /**
      * @returns the array data of a column of the matrix, as constant
      */
    inline const T *columnPtr( int col ) const
    { return array() + col * size(); }

    /**
      * @returns the array data of a column of the matrix, as non-constant
      */
    inline T *columnPtr( int col )
    { return array() + col * size(); }

    /**
      * @returns a column of the matrix as a VectorX.
      */
    const VectorType columnVector( int col ) const
    {
        return VectorType( size() ).readArray( columnPtr( col ) );
    }

    /**
      * @returns a row of the matrix as a VectorX.
      */
    VectorType rowVector( int row ) const;

    /**
      * Copies an array into a column of the matrix.
      * The array must have size at least equal to the size of
      * the matrix.
      */
    void setColumn( int col, const T * array );

    /**
      * Copies an array into a row of the matrix.
      * The array must have size at least equal to the size of
      * the matrix.
      */
    void setRow( int row, const T * array );

    /**
      * Copies a vector into a column of the matrix.
      * The size of the vector must equal the size of the matrix.
      */
    inline void setColumn( int col, const VectorType & v )
    {
        EIGEN_ASSERT_NOABORT( v.size() == size() );
        setColumn( col, v.array() );
    }

    /**
      * Copies a vector into a row of the matrix.
      * The size of the vector must equal the size of the matrix.
      */
    inline void setRow( int row, const VectorType &v )
    {
        EIGEN_ASSERT_NOABORT( v.size() == size() );
        setRow( row, v.array() );
    }

    /**
      * Reads the rows of the matrix from a row-dominant array.
      * For instance, C/C++ two-dimensional arrays are stored
      * in row-dominant order.
      */
    void readRows( const T * rows );

    /**
      * Sets *this to be the identity matrix.
      */
    void loadIdentity();

    /**
      * Sets *this to be the zero matrix.
      */
    void loadZero();

    /**
      * Returns *this + other (entry-wise addition).
      * The matrices *this and other must have
      * the same size.
      */
    inline Derived operator + ( const Derived & other ) const
    {
        EIGEN_ASSERT_NOABORT( other.size() == size()
                           && other.size()  == size() );
        Derived res( size() );
        addition_helper( other, & res );
        return res;
    }

    /**
      * Returns *this - other (entry-wise substraction).
      * The matrices *this and other must have
      * the same size.
      */
    inline Derived operator - ( const Derived & other ) const
    {
        EIGEN_ASSERT_NOABORT( other.size() == size()
                           && other.size()  == size() );
        Derived res( size() );
        substraction_helper( other, & res );
        return res;
    }

    /**
      * Returns *this * factor (multiplication of each coord).
      */
    inline Derived operator * ( const T & factor ) const
    {
        Derived res( size() );
        scalar_multiplication_helper( factor, & res );
        return res;
    }

    /**
      * Returns *this / factor (division of each coord).
      */
    inline Derived operator / ( const T & factor ) const
    {
        Derived res( size() );
        scalar_division_helper( factor, & res );
        return res;
    }

    /**
      * Returns *this * vector (multiplication of vector by matrix).
      * The size of *this must equal the size of vector.
      */
    inline VectorType operator * ( const VectorType & vector ) const
    {
        EIGEN_ASSERT_NOABORT( vector.size() == size() );
        VectorType res( size() );
        vector_multiplication_helper( vector, & res );
        return res;
    }

    /**
      * Computes the product *this * vector and stores the
      * result into *res.
      *
      * For dynamic-size classes, this method resizes *res if necessary.
      * For fixed-size classes, it is required that *res already has the
      * right size, that is: res->size() == this->size().
      *
      * This method is faster than operator*.
      *
      * Note for dynamic-size classes:
      * For optimal performance, the vector *res should be
      * initialized with the correct size before calling this method,
      * otherwise it will have to be resized, which is costly.
      *
      * Note that for the product to make sense, it is required
      * that this->size() == other.size().
      */
    inline void multiply( const VectorType & vector, VectorType * res ) const
    {
        EIGEN_ASSERT_NOABORT( this->size() == vector.size() );
        res->resize( this->size() );
        vector_multiplication_helper( vector, & res );
    }

    /**
      * @returns the determinant of the matrix (must be a square matrix).
      *
      * If the matrix has fixed size <= 3, the determinant is obtained by
      * direct computation. If the matrix has size >= 4 and/or has dynamic
      * size, a LU
      * decomposition is computed, and the determinant is obtained from it.
      * In this case, if you plan to perform other operations
      * that use a LU decomposition, it's better for performance to compute the
      * LU decomposition once and then do the computations from it.
      */
    T determinant() const;

    /**
      * @returns true if the matrix is invertible, false if it is singular.
      *
      * If the matrix has fixed size <= 3, the invertibility is obtained by
      * direct computation. If the matrix has size >= 4 and/or has dynamic
      * size, a LU
      * decomposition is computed, and the invertibility is obtained from it.
      * In this case, if you plan to perform other operations
      * that use a LU decomposition, it's better for performance to compute the
      * LU decomposition once and then do the computations from it.
      */
   inline bool isInvertible() const;

    /**
      * @returns the dimension of the kernel of the matrix.
      *
      * This function always performs a LU decomposition, even for small
      * matrices. If you plan to perform other operations
      * that use a LU decomposition, it's better for performance to compute the
      * LU decomposition once and then do the computations from it.
      */
    inline int dimKer() const
    {
        return LUDecompositionType( *static_cast<const Derived*>(this) )
                   .dimKer();
    }


    /**
      * @returns the rank of the matrix.
      *
      * This function always performs a LU decomposition, even for small
      * matrices. If you plan to perform other operations
      * that use a LU decomposition, it's better for performance to compute the
      * LU decomposition once and then do the computations from it.
      */
    inline int rank() const
    {
        return LUDecompositionType( *static_cast<const Derived*>(this) )
                   .rank();
    }

    /**
      * This method computes a basis of the kernel of the matrix *this.
      *
      * The \a result argument is a pointer to the matrix in which the result
      * will be stored. After calling this method, the \a dimKer() first
      * columns of \a result form a basis of the kernel of *this. If *this
      * is invertible, then dimKer()==0 and this method does nothing.
      *
      * @returns true if the matrix is non-invertible, hence has a nonzero
      * kernel; false if the matrix is invertible.
      *
      * Note:
      *
      * 1. This function always performs a LU decomposition, even for small
      * matrices. If you plan to perform other operations
      * that use a LU decomposition, it's better for performance to compute the
      * LU decomposition once and then do the computations from it.
      */
    inline bool computeBasisKer( Derived * result ) const
    {
        return LUDecompositionType( *static_cast<const Derived*>(this) )
                   .computeBasisKer(result);
    }

    /**
      * Computes an antecedent of v by *this, that is, a vector u
      * such that Au=v, where A is the matrix *this.
      * If such an antecedent doesn't exist, this method does nothing.
      *
      * @returns true if an antecedent exists, false if no antecedent exists.
      *
      * Notes:
      *
      * 1. The returned vector u is only one solution of the equation Au=v,
      * which can have more than one solution if A is non-invertible. To get
      * a basis of the whole (affine) space of solutions, use basisKer().
      *
      * 2. This function always performs a LU decomposition, even for small
      * matrices. If you plan to perform other operations
      * that use a LU decomposition, it's better for performance to compute the
      * LU decomposition once and then do the computations from it.
      */
    inline bool computeSomeAntecedent
    ( const VectorType & v, VectorType * result ) const
    {
        return LUDecompositionType( *static_cast<const Derived*>(this) )
                   .computeSomeAntecedent(v, result);
    }

    /**
      * Computes the inverse matrix of *this, and
      * stores it in *result.
      *
      * If *this is non-invertible, this method does nothing.
      *
      * \returns true if *this is invertible, false otherwise.
      *
      * If the matrix has fixed size <= 3, the inverse is obtained by
      * direct computation. If the matrix has size >= 4 and/or has dynamic
      * size, a LU
      * decomposition is computed, and the inverse is obtained from it.
      * In this case, if you plan to perform other operations
      * that use a LU decomposition, it's better for performance to compute the
      * LU decomposition once and then do the computations from it.
      */
    inline bool computeInverse( Derived * result ) const;

    /**
      * Stores in *res_row and *res_col the position of the entry of the
      * matrix that has biggest absolute value. Skips the \a skip first rows
      * and columns (default value for \a skip is 0).
      */
    void findBiggestEntry( int * res_row, int * res_col, int skip = 0 ) const;

    /**
      * Computes the adjoint (conjugate transpose, equals transpose unless
      * T is complex numbers) of *this and stores it in *result.
      *
      * Don't try to pass \a this as \a result , this won't work. To replace
      * *this by its adjoint, use replaceByAdjoint() instead. 
      */
    void computeAdjoint( Derived * result ) const;

    /**
      * Returns the adjoint (conjugate transpose, equals transpose unless
      * T is complex numbers) of *this.
      */
    inline Derived adjoint() const
    {
        Derived res( size() );
        computeAdjoint( & res );
        return res;
    };

    /**
      * Replaces *this by its adjoint (conjugate transpose, equals
      * transpose unless T is complex numbers).
      */
    void replaceByAdjoint();
};

/**
  * Returns factor * v (multiplication of each entry of v by factor).
  */
template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
inline Derived operator *
( const T & factor,
  const MatrixBase<T, Derived, VectorType, LUDecompositionType> &
        v )
{
    return v * factor;
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::loadIdentity()
{
    for( int col = 0; col < size(); col++ )
        for( int row = 0; row < size(); row++ )
            (*this)( row, col ) = static_cast<T>( col == row );
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::loadZero()
{
    for( int i = 0; i < size() * size(); i++ )
        (*this)[i] = static_cast<T>(0);
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
Derived & MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::operator =
( const Derived & other )
{
    if( hasDynamicSize() )
        if( static_cast<Derived*>(this) == &other )
            return *static_cast<Derived*>(this);
    resize( other.size() );
    readArray( other.array() );
    return *static_cast<Derived*>(this);
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
VectorType
MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::rowVector( int row ) const
{
    VectorType ret( size() );
    const T *rowptr = array() + row;
    for( int i = 0; i < ret.size(); i++ )
    {
        ret[i] = *rowptr;
        rowptr += size();
    }
    return ret;
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::setColumn
( int col, const T *array )
{
    T *colptr = columnPtr( col );
    for (int i = 0; i < size(); i++) {
        *colptr = array[i];
        colptr++;
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::setRow
( int row, const T *array )
{
    T *rowptr = this->array() + row;
    for( int i = 0; i < size(); i++ )
    {
        *rowptr = array[i];
        rowptr += size();
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::readRows( const T * rows )
{
    const T * ptr = rows;
    for( int i = 0; i < size(); i++ )
    {
        setRow( i, ptr );
        ptr += size();
    }
}


template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void
MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::matrix_multiplication_helper
( const Derived & other, Derived * res ) const
{
    T *destptr = res->array();
    const T *colptr = other.array();
    for (int col = 0; col < res->size(); col++) {
        const T *rowptr = array();
        for (int row = 0; row < res->size(); row++) {
            *destptr = static_cast<T>(0);
            const T *entry1ptr = colptr;
            const T *entry2ptr = rowptr;
            for (int entry = 0; entry < size(); entry++) {
                *destptr += (*entry1ptr) * (*entry2ptr);
                entry1ptr++;
                entry2ptr += size();
            }
            destptr++;
            rowptr++;
        }
        colptr += other.size();
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void
MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::vector_multiplication_helper
( const VectorType & vector, VectorType * res ) const
{
    T *destptr = res->array();
    for( int i = 0; i < size(); i++)
    {
        const T *entryptr = array() + i;
        *destptr = static_cast<T>(0);
        for( int j = 0; j < size(); j++)
        {
            *destptr += (*entryptr) * vector[j];
            entryptr += size();
        }
        destptr++;
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
T MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::determinant() const
{
    if( (!hasDynamicSize()) && size() <= 3 )
    {
        if( size() == 1 )
            return (*this)[0];
        else if( size() == 2 )
            return (*this)[0] * (*this)[3] - (*this)[1] * (*this)[2];
        else
            return
              (*this)[0] * ( (*this)[4] * (*this)[8] - (*this)[5] * (*this)[7] )
            - (*this)[1] * ( (*this)[3] * (*this)[8] - (*this)[5] * (*this)[6] )
            + (*this)[2] * ( (*this)[3] * (*this)[7] - (*this)[4] * (*this)[6] );
    }
    else
        return LUDecompositionType(
                        *static_cast<const Derived*>(this)
                                  ).determinant();
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
inline bool MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::computeInverse( Derived * result ) const
{
    EIGEN_ASSERT_NOABORT( result->size() == size() );
    if( (!hasDynamicSize()) && size() <= 3 )
    {
        T det = determinant();
        int row_max_abs, col_max_abs;
        findBiggestEntry( &row_max_abs, &col_max_abs );
        if( Util::isNegligible( det, (*this)( row_max_abs, col_max_abs ) ) )
            return false;
        if( size() == 1 )
        {
            (*result)(0, 0) = static_cast<T>(1) / det;
            return true;
        }
        else if( size() == 2 )
        {
            (*result)(0, 0) = (*this)(1,1);
            (*result)(1, 0) = - (*this)(1,0);
            (*result)(0, 1) = - (*this)(0,1);
            (*result)(1, 1) = (*this)(0,0);
            (*result) /= det;
            return true;
        }
        else if( size() == 3 )
        {
            (*result)(0, 0) =
                (*this)(1,1) * (*this)(2,2) - (*this)(1,2) * (*this)(2,1);
            (*result)(0, 1) =
              - (*this)(0,1) * (*this)(2,2) + (*this)(0,2) * (*this)(2,1);
            (*result)(0, 2) =
                (*this)(0,1) * (*this)(1,2) - (*this)(1,1) * (*this)(0,2);
            (*result)(1, 0) =
              - (*this)(1,0) * (*this)(2,2) + (*this)(1,2) * (*this)(2,0);
            (*result)(1, 1) =
                (*this)(0,0) * (*this)(2,2) - (*this)(0,2) * (*this)(2,0);
            (*result)(1, 2) =
              - (*this)(0,0) * (*this)(1,2) + (*this)(1,0) * (*this)(0,2);
            (*result)(2, 0) =
                (*this)(1,0) * (*this)(2,1) - (*this)(1,1) * (*this)(2,0);
            (*result)(2, 1) =
              - (*this)(0,0) * (*this)(2,1) + (*this)(0,1) * (*this)(2,0);
            (*result)(2, 2) =
                (*this)(0,0) * (*this)(1,1) - (*this)(1,0) * (*this)(0,1);
            (*result) /= det;
            return true;
        }
    }
    else return LUDecompositionType(
                 *static_cast<const Derived*>(this)
                                   ).computeInverse(result);
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
inline bool MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::isInvertible() const
{
    if( (!hasDynamicSize()) && size() <= 3 )
    {
        T det = determinant();
        int row_max_abs, col_max_abs;
        findBiggestEntry( &row_max_abs, &col_max_abs );
        return( ! Util::isNegligible(
                    det, (*this)( row_max_abs, col_max_abs )
                                    ) );
    }
    else return LUDecompositionType(
                 *static_cast<const Derived*>(this)
                                   ).isInvertible();
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
inline void MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::findBiggestEntry( int * res_row, int * res_col, int skip ) const
{
    int row_max_abs = skip, col_max_abs = skip;
    const T *entry_ptr = &( (*this)(skip, skip) );
    const T *entry_max_abs_ptr = entry_ptr;
    for( int col = skip; col < size(); col++)
    {
        for( int row = skip; row < size(); row++)
        {
            if( Util::abs( *entry_ptr ) > Util::abs( *entry_max_abs_ptr ) )
            {
                row_max_abs = row;
                col_max_abs = col;
                entry_max_abs_ptr = entry_ptr;
            }
            entry_ptr++;
        }
        entry_ptr += skip;
    }
    *res_row = row_max_abs;
    *res_col = col_max_abs;
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
inline void MatrixBase<T, Derived, VectorType, LUDecompositionType>
    ::computeAdjoint( Derived * result ) const
{
    EIGEN_ASSERT_NOABORT( size() == result->size() );
    for( int row = 0; row < size(); row++ )
        for( int col = 0; col < size(); col++ )
            (*result)( col, row ) = Util::conj( (*this)( row, col ) );
}


template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
    ::replaceByAdjoint()
{
    for( int row = 0; row < size(); row++ )
    {
        (*this)( row, row ) = Util::conj( (*this)( row, row ) );
        for( int col = row+1; col < size(); col++ )
        {
            T tmp = (*this)( row, col );
            (*this)( row, col ) = Util::conj( (*this)( col, row ) );
            (*this)( col, row ) = Util::conj( tmp );
        }
    }
}

/**
  * Allows to print a matrix by simply doing
  * @code
    cout << mymatrix << endl;
  * @endcode
  */
template<typename T,
         typename Derived,
         typename VectorType,
         typename LUDecompositionType>
std::ostream & operator <<
( std::ostream & s,
  const MatrixBase<T, Derived, VectorType, LUDecompositionType> & m )
{
    for( int i = 0; i < m.size(); i++ )
    {
        s << m( i, 0 );
        for (int j = 1; j < m.size(); j++ )
            s << " " << m( i, j );
        if( i < m.size() - 1)
            s << std::endl;
    }
    return s;
}

} // namespace Eigen

#endif // EIGEN_MATRIXBASE_H
