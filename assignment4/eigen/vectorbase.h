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


#ifndef EIGEN_VECTORBASE_H
#define EIGEN_VECTORBASE_H

#include "util.h"

namespace Eigen
{

/** \ingroup internalbases
  *
  * \ingroup vectors
  *
  * \brief internal base class
  *
  * This class template is only internally used in Eigen.
  * It provides the base that the Vector and VectorX
  * class templates inherit.
  *
  * Note that one template parameter is Derived: this is a
  * C++ trick knows as Curiously Recursive Template Pattern.
  * Here, it allows us to implement in VectorBase the code
  * of both Vector and VectorX, which are very different
  * (Vector stores the size of the vector as template argument,
  * while VectorX stores it as member data, thus storing the
  * array itself on the heap).
  */
template<typename T, typename Derived>
class VectorBase
{

private:

    /** \internal
      * Helper method, computes *this + other and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void addition_helper
    ( const Derived & other, Derived * res ) const
    {
        for( int i = 0; i < res->size(); i++ )
            (*res)[i] = (*this)[i] + other[i];
    }
    
    /** \internal
      * Helper method, computes *this - other and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void substraction_helper
    ( const Derived & other, Derived * res ) const
    {
        for( int i = 0; i < res->size(); i++ )
            (*res)[i] = (*this)[i] - other[i];
    }
    
    /** \internal
      * Helper method, computes *this * factor and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void multiplication_helper
    ( const T & factor, Derived * res ) const
    {
        for( int i = 0; i < res->size(); i++ )
            (*res)[i] = (*this)[i] * factor;
    }

    /** \internal
      * Helper method, computes *this / factor and stores the result in res.
      * Doesn't attempt to resize res. Doesn't check that the sizes match.
      */
    inline void division_helper
    ( const T & factor, Derived * res ) const
    {
        multiplication_helper( static_cast<const T>(1) / factor, res );
    }

    /** \internal
      * Helper method, computes the cross product of *this and factor
      * and stores the result in res_array (an array, not a vector).
      * Doesn't check the sizes (must be 3).
      */
    void crossproduct_helper
    ( const Derived & other, T * res_array ) const
    {
        res_array[0] =  (*this)[1] * other[2] - (*this)[2] * other[1];
        res_array[1] =  (*this)[2] * other[0] - (*this)[0] * other[2];
        res_array[2] =  (*this)[0] * other[1] - (*this)[1] * other[0];
    }

public:

    /**
      * @returns true if the vector has dynamic size (i.e. is an
      * object of class VectorX), false if the vector has fixed size
      * (i.e. is an object of class Vector).
      */
    inline bool hasDynamicSize() const
    {
        return static_cast<const Derived*>(this)->_hasDynamicSize();
    }

    /**
      * @returns the size (dimension) of the vector.
      */
    inline int size() const
    {
        return static_cast<const Derived*>(this)->_size();
    }

    /**
      * Tries to resize the vector. That is only possible if the vector
      * has dynamic size, i.e. is an object of class VectorX. Otherwise,
      * nothing is done.
      *
      * The vector coords are not kept, they
      * are left with undefined values after resizing.
      */
    inline void resize( int newsize )
    {
        static_cast<Derived*>(this)->_resize(newsize);
    }

    /**
      * @returns the array of the vector, as constant.
      */
    inline const T * array() const
    {
        return static_cast<const Derived*>(this)->m_array;
    }

    /**
      * @returns the array of the vector, as non-constant.
      */
    inline T * array()
    {
        return static_cast<Derived*>(this)->m_array;
    }

    /**
      * @returns a constant reference to the i-th coord of the vector.
      *
      * Same as operator[].
      */
    inline const T & operator () ( int i ) const
    {
        return array() [i];
    }

    /**
      * @returns a non-constant reference to the i-th coord of the vector.
      *
      * Same as operator[].
      */
    inline T & operator () ( int i )
    {
        return array() [i];
    }

    /**
      * @returns a constant reference to the i-th coord of the vector.
      *
      * Same as operator().
      */
    inline const T & operator [] ( int i ) const
    {
        return array() [i];
    }

    /**
      * @returns a non-constant reference to the i-th coord of the vector.
      *
      * Same as operator().
      */
    inline T & operator [] ( int i )
    {
        return array() [i];
    }

    /**
      * Returns the dot product of *this by other.
      *
      * *this and other must have the same size (the compiler will check that
      * for fixed-size vectors, but not for dynamic-size vectors).
      *
      * If T is std::complex, the dot product is hermitian, i.e.
      * the coords of *this get complex-conjugated in the formula.
      */
    T dot( const Derived & other ) const
    {
        EIGEN_ASSERT_NOABORT( size() == other.size() );
        T ret = Util::conj((*this)[0]) * other[0];
        for( int i = 1; i < size(); i++ )
            ret += Util::conj((*this)[i]) * other[i];
        return ret;
    }

    /**
      * Returns the cross product of *this by other.
      * *this and other must have size exactly 3.
      */
    inline Derived cross( const Derived & other ) const
    {
        EIGEN_ASSERT_NOABORT( other.size() == 3 && size() == 3 );
        Derived res(3);
        crossproduct_helper( other, res.array() );
        return res;
    }

    /**
      * Returns the squared norm of *this, that is, the dot product
      * of *this with itself.
      */
    inline T norm2() const
    {
        return dot( *static_cast<const Derived*>(this) );
    }

    /**
      * Returns the norm of *this.
      */
    inline T norm() const
    {
        return sqrt( norm2() );
    }

    /**
      * Normalizes *this, that is, divides *this by norm().
      */
    inline Derived & normalize()
    {
        (*this) /= norm();
        return *static_cast<Derived *>(this);
    }

    /**
      * Constructs a unit vector that is orthogonal to *this,
      * and stores it into *res.
      *
      * *res and *this must have the same size, and that size must
      * be at least 2.
      *
      * @returns a reference to *res.
      */
    Derived & makeOrthoVector( Derived * res ) const;

    /**
      * Sets all coords of *this to zero.
      */
    void loadZero()
    {
        for (int i = 0; i < size(); i++)
            (*this)[i] = static_cast<T>(0);
    }

    /**
      * Reads the coords of *this from an array. The number of entries
      * read from the array is equal to size().
      */
    void readArray( const T * src )
    {
        for (int i = 0; i < size(); i++)
            (*this)[i] = src[i];
    }

    /**
      * Copies other into *this.
      *
      * *this gets resized if it didn't already have the same size as other.
      */
    Derived & operator = ( const Derived & other )
    {
        if( hasDynamicSize() )
            if( &other == static_cast<Derived*>(this) )
                return *static_cast<Derived*>(this);
        resize( other.size() );
        readArray( other.array() );
        return *static_cast<Derived*>(this);
    }

    /**
      * Stores *this + other into *this (coordinate-wise addition).
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
      * Stores *this - other into *this (coordinate-wise substraction).
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
      * Stores *this * factor into *this (multiplication of each coord).
      */
    inline Derived & operator *= ( const T & factor )
    {
        multiplication_helper( factor, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Stores *this / factor into *this (division of each coord).
      */
    inline Derived & operator /= ( const T & factor )
    {
        division_helper( factor, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Cross product with self-assignment.
      *
      * In other words, a.cross_equal(b) sets a to be the crossed product
      * of a and b.
      *
      * Of course, *this and other must have size exactly 3.
      */
    Derived & cross_equal( const Derived & other )
    {
        EIGEN_ASSERT_NOABORT( size() == 3 && other.size() == 3 );
        T tmp[3];
        crossproduct_helper( other, tmp );
        readArray( tmp );
        return *static_cast<Derived*>(this);
    }

    /**
      * Returns *this + other (coordinate-wise addition).
      * The vectors *this and other must have
      * the same size.
      */
    inline Derived operator + ( const Derived & other ) const
    {
        EIGEN_ASSERT_NOABORT( other.size() == size() );
        Derived res( size() );
        addition_helper( other, &res );
        return res;
    }

    /**
      * Returns *this - other (coordinate-wise addition).
      * The vectors *this and other must have
      * the same size.
      */
    inline Derived operator - ( const Derived & other ) const
    {
        EIGEN_ASSERT_NOABORT( other.size() == size() );
        Derived res( size() );
        substraction_helper( other, &res );
        return res;
    }

    /**
      * Returns *this * factor (multiplication of each coord).
      */
    inline Derived operator * ( const T & factor ) const
    {
        Derived res( size() );
        multiplication_helper( factor, &res );
        return res;
    }

    /**
      * Returns *this / factor (division of each coord).
      */
    inline Derived operator / ( const T & factor ) const
    {
        Derived res( size() );
        division_helper( factor, &res );
        return res;
    }

    /**
      * Returns a reference to the first coord of *this.
      */
    inline T & x() { return (*this)[0]; }

    /**
      * Returns a constant reference to the first coord of *this.
      */
    inline const T & x() const { return (*this)[0]; }

    /**
      * Returns a reference to the second coord of *this.
      *
      * The size of *this must be at least 2.
      */
    inline T & y()
    {
        EIGEN_ASSERT_NOABORT( size() >= 2 );
        return (*this)[1];
    }

    /**
      * Returns a constant reference to the second coord of *this.
      *
      * The size of *this must be at least 2.
      */
    inline const T & y() const
    {
        EIGEN_ASSERT_NOABORT( size() >= 2 );
        return (*this)[1];
    }

    /**
      * Returns a reference to the third coord of *this.
      *
      * The size of *this must be at least 3.
      */
    inline T & z()
    {
        EIGEN_ASSERT_NOABORT( size() >= 3 );
        return (*this)[2];
    }

    /**
      * Returns a constant reference to the third coord of *this.
      *
      * The size of *this must be at least 3.
      */
    inline const T & z() const
    {
        EIGEN_ASSERT_NOABORT( size() >= 3 );
        return (*this)[2];
    }

    /**
      * Returns a reference to the fourth coord of *this.
      *
      * The size of *this must be at least 4.
      */
    inline T & w()
    {
        EIGEN_ASSERT_NOABORT( size() >= 4 );
        return (*this)[3];
    }

    /**
      * Returns a constant reference to the fourth coord of *this.
      *
      * The size of *this must be at least 4.
      */
    inline const T & w() const
    {
        EIGEN_ASSERT_NOABORT( size() >= 4 );
        return (*this)[3];
    }

};

template<typename T, typename Derived>
Derived & VectorBase<T, Derived>::makeOrthoVector( Derived * res ) const
{
    EIGEN_ASSERT_NOABORT( size() >= 2 );
    EIGEN_ASSERT_NOABORT( size() == res->size() );

    // let u be a normalized copy of *this
    Derived u = *static_cast<const Derived *>(this);
    u.normalize();

    /* next we want to construct another vector v
    that is not colinear to u. But actually we need more than that:
    we need that u and v are not "too close" to being colinear.
    */
    Derived v( u );

    /* if the absolute values of the two first coords of v are not of the same
    order of magnitude, then it is enough to swap them.
    */
    if( Util::abs( v(0) - v(1) ) > 0.1 * Util::abs(v(0)) )
    {
        T tmp = v(0);
        v(0) = v(1);
        v(1) = tmp;
    }
    /* on the other hand, if they are of the same order of magnitude, then
       v is far from being colinear to any vector whose two first coords are
       ( 1, 0, ... )
    */
    else 
    {
        v(0) = static_cast<T>(1);
        v(1) = static_cast<T>(0);
    }

    // now, v is far from colinear to u.
    // so let's project it on the orthogonal subspace of u
    // and normalize the result.
    (*res) = ( v - u.dot(v) * u ).normalize();
    return *res;
}

/**
  * Returns factor * v (multiplication of each coord of v by factor).
  */
template<typename T, typename Derived>
inline Derived operator *
( const T & factor, const VectorBase<T, Derived> & v )
{
    return v * factor;
}

/**
  * Dot product
  *
  * If T is std::complex, the dot product is hermitian, i.e.
  * the coords of *this get complex-conjugated in the formula.
  */
template<typename T, typename Derived>
inline T dot
( const VectorBase<T, Derived> & v1, const Derived & v2 )
{
    return v1.dot(v2);
}

/**
  * Cross product.
  *
  * Of course, *this and other must have size exactly 3.
  */
template<typename T, typename Derived>
inline Derived cross
( const VectorBase<T, Derived> & v1, const Derived & v2 )
{
    return v1.cross(v2);
}

/**
  * Allows to print a vector by simply doing
  * @code
    cout << myvector << endl;
  * @endcode
  */
template<typename T, typename Derived>
std::ostream & operator <<
( std::ostream & s, const VectorBase<T, Derived> & v )
{
    s << "(" << v(0);
    for( int i = 1; i < v.size(); i++ )
        s << ", " << v( i );
    s << ")";
    return s;
}

} // namespace Eigen

#endif // EIGEN_VECTORBASE_H
