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

#ifndef EIGEN_UTIL_H
#define EIGEN_UTIL_H

#include <math.h>
#include <complex>
#include <iostream>

#ifndef NDEBUG
# include <features.h>
#endif

namespace Eigen
{

namespace Util
{

/**
  * overloaded inline function returning the epsilon for floats,
  * that is, a small float that is still big enough so that
  * epsilon != 1 + epsilon
  * reasonnably stably in floating-point arithmetic.
  */
inline float epsilon( const float& x )
{
    (void) x;
    return 1e-6;
}

/**
  * overloaded inline function returning the epsilon for doubles,
  * that is, a small double that is still big enough so that
  * epsilon != 1 + epsilon
  * reasonnably stably in floating-point arithmetic.
  */
inline double epsilon( const double& x )
{
    (void) x;
    return 1e-12;
}

/**
  * overloaded inline function returning the epsilon for complex numbers.
  * For complex numbers over typename T, this just returns the epsilon for T.
  */
template<typename T>
inline std::complex<T> epsilon ( const std::complex<T> & x )
{
    return epsilon( x.real() );
}

/**
  * overloaded inline function that calls the fabsf function for floats.
  * Returns the absolute value of x.
  */
inline float abs( const float& x )
{
    return fabsf( x );
}

/**
  * overloaded inline function that calls the fabs function for doubles.
  * Returns the absolute value of x.
  */
inline double abs( const double& x )
{
    return fabs( x );
}

/**
  * overloaded inline function that calls the std::abs function for complexes.
  * Returns the absolute value of x.
  */
template<typename T>
inline T abs ( const std::complex<T> & x )
{
    return std::abs( x );
}

/**
  * overloaded inline function that calls the sqrtf function for floats.
  * Returns the square root of x.
  *
  * Note: there is no need for a similar function for doubles as the
  * square-root function for doubles is already named sqrt.
  */
inline float sqrt( const float& x )
{
    return sqrtf( x );
}

/**
  * overloaded inline function that calls the std::sqrt function for complexes.
  * Returns the square root of x.
  *
  */
template<typename T>
inline std::complex<T> sqrt( const std::complex<T> & x )
{
    return std::sqrt( x );
}

/**
  * overloaded function that returns the complex conjugate of a float x.
  * Of course, as floats are reals, this is just the trivial function
  * returning x. But that'll become useful to handle matrices of complex
  * numbers.
  */
inline float conj( const float& x )
{
    return x;
}

/**
  * overloaded function that returns the complex conjugate of a double x.
  * Of course, as doubles are reals, this is just the trivial function
  * returning x. But that'll become useful to handle matrices of complex
  * numbers.
  */
inline double conj( const double& x )
{
    return x;
}

/**
  * overloaded function that returns the complex conjugate of a complex x.
  */
template<typename T>
inline std::complex<T> conj( const std::complex<T> & x )
{
    return std::conj(x);
}


/**
  * overloaded inline function that calls the fminf function for floats.
  * Returns the minimum of two floats
  */
inline float min( const float& x, const float& y )
{
    return fminf( x, y );
}

/**
  * overloaded inline function that calls the fmin function for doubles.
  * Returns the minimum of two doubles
  */
inline double min( const double& x, const float& y )
{
    return fmin( x, y );
}

/**
  * overloaded inline function that calls the fmaxf function for floats.
  * Returns the maximum of two floats
  */
inline float max( const float& x, const float& y )
{
    return fmaxf( x, y );
}

/**
  * overloaded inline function that calls the fmax function for doubles.
  * Returns the maximum of two doubles
  */
inline double max( const double& x, const float& y )
{
    return fmax( x, y );
}

/**
  * Short version: returns true if a is much smaller than to b, false otherwise.
  * a and b must be >= 0.
  *
  * Long version: returns ( a <= b * epsilon(b) ).
  * In other words, this function returns true, if a is so small compared to b
  * that in floating-point arithmetic, 1 + a/b is not reasonnably stably
  * different from 1.
  * @param a,b must be NONNEGATIVE REAL numbers.
  *
  * This function uses the epsilon overloaded function
  * to determine what's "small".
  */
template<typename T> inline bool isNegligible( const T& a, const T& b )
{
    return( a <= b * epsilon(b) );
}

/**
  * Returns true if a is very close to b, false otherwise.
  *
  * In other words: returns isNegligible( abs( a - b ), min( abs(a), abs(b) ) ).
  *
  * @param a,b can be real or complex numbers (std::complex).
  */
template<typename T> inline bool isApprox( const T& a, const T& b )
{
    return isNegligible( abs( a - b ), min( abs(a), abs(b) ) );
}

#ifndef NDEBUG
//-------------------------------------------
// The following is mostly copied & pasted from the assert.h file
// in the GNU C Library.
//-------------------------------------------

/* Version 2.4 and later of GCC define a magical variable `__PRETTY_FUNCTION__'
   which contains the name of the function currently being defined.
   This is broken in G++ before version 2.6.
   C9x has a similar variable called __func__, but prefer the GCC one since
   it demangles C++ function names.  */
# if defined __cplusplus ? __GNUC_PREREQ (2, 6) : __GNUC_PREREQ (2, 4)
#   define __EIGEN_ASSERT_FUNCTION    __PRETTY_FUNCTION__
# else
#  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#   define __EIGEN_ASSERT_FUNCTION    __func__
#  else
#   define __EIGEN_ASSERT_FUNCTION    ((__const char *) 0)
#  endif
# endif

//-------------------------------------------
// End of code mostly copied & pasted from the assert.h file
// in the GNU C Library.
//-------------------------------------------

/** \internal
  * Helper for the EIGEN_ASSERT_NOABORT macro. Prints the error message.
  */
template<typename unused>
void _eigen_assert_noabort_helper( const char *expr, int line,
                                   const char *file, const char *function )
{
    std::cout << "*** ERROR in Eigen: failed assertion line "
              << line << " of file " << file << std::endl;
    std::cout << "*** asserted expression: " << expr << std::endl;
    std::cout << "*** in function: " << function << std::endl;
}

/**
  * This macro does mostly the same as the usual assert macro defined in
  * assert.h, except that it doesn't abort on a failed assertion. It only
  * prints an error message on stdout.
  *
  * This is useful because some distros leave debug enabled. Hence assert can
  * abort on the user, which is bad because from the user's point of view this
  * amounts to a crash.
  *
  * There are obvious security implications with not aborting on a failed
  * assert, but at the end of the day it is up to the application programmer
  * to make sure that there won't ever be failed asserts.
  *
  * Another advantage of not aborting is that this gives an opportunity to
  * get more debug messages before the application eventually crashes, thus
  * providing more debugging information to help understand the problem.
  *
  * Another difference with assert is that this macro prepends
  * the filename with "eigen/" to underline the fact that the problem occurred
  * in Eigen.
  *
  * Just like the standard assert macro, this macro depends on the NDEBUG
  * macro. If NDEBUG is defined, then this macro does nothing (NDEBUG means
  * no debugging).
  *
  * Many thanks go to Mark Kretschmann for explaining to me why aborting is bad.
  */
#define EIGEN_ASSERT_NOABORT(expr) \
    if( ! (expr) ) \
        Eigen::Util::_eigen_assert_noabort_helper<int> \
            ( __STRING(expr), __LINE__, \
              __FILE__, __EIGEN_ASSERT_FUNCTION )

#else // ! NDEBUG
// if NDEBUG is defined, we're compiling without debugging support,
// so the asserts should be ignored as well.

#define EIGEN_ASSERT_NOABORT(expr)
#endif // ! NDEBUG

#define EIGEN_MAKE_FIXEDSIZE_TYPEDEFS(Class) \
    typedef Class<double, 2>               Class##2d;\
    typedef Class<double, 3>               Class##3d;\
    typedef Class<double, 4>               Class##4d;\
    typedef Class<float,  2>               Class##2f;\
    typedef Class<float,  3>               Class##3f;\
    typedef Class<float,  4>               Class##4f;\
    typedef Class<std::complex<double>, 2> Class##2cd;\
    typedef Class<std::complex<double>, 3> Class##3cd;\
    typedef Class<std::complex<double>, 4> Class##4cd;\
    typedef Class<std::complex<float>,  2> Class##2cf;\
    typedef Class<std::complex<float>,  3> Class##3cf;\
    typedef Class<std::complex<float>,  4> Class##4cf;

#define EIGEN_MAKE_DYNAMICSIZE_TYPEDEFS(Class) \
    typedef Class<double>                 Class##d;\
    typedef Class<float>                  Class##f;\
    typedef Class< std::complex<double> > Class##cd;\
    typedef Class< std::complex<float> >  Class##cf;

} // namespace Util

} // namespace Eigen

#endif // EIGEN_UTIL_H

/** @defgroup vectors Vectors*/
/** @defgroup matrices Matrices*/
/** @defgroup fixedsize Fixed-size classes*/
/** @defgroup dynamicsize Dynamic-size classes*/
/** @defgroup solving Solving systems of equations*/
/** @defgroup regression Linear regression analysis*/
/** @defgroup h3d H3D classes*/
/** @defgroup internalbases Internal base classes and functions*/

