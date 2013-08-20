// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2006 Casper Boemann <cbr@boemann.dk>
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

#ifndef MATRIXH3D_H
#define MATRIXH3D_H


namespace Eigen
{

/** \ingroup h3d
 *
 * \brief Projective (3+1) matrix (homography on P^3)
 *
 * \author Casper Boemann, Inge Wallin
 *
 * A four dimensional matrix class. This is for linear algebra
 * 
 * Basically this implements everything you would expect of such a class.
 *
 **/
class MatrixH3D
{
    //protected:
public:
    MatrixH3D() {}

public:
    // These should be used everywhere for the size of the matrix.
    // No numeric constants, please.
    static const int  MATSIZE2 = 16;
    static const int  MATSIZE  = 4;

    union
    {
        /** currently, we are storing the matrices in column-major order.
        In other words, the matrix elements are stored in the following
        order:

        ( 0  4  8  12 )
        ( 1  5  9  13 )
        ( 2  6  10 14 )
        ( 3  7  11 15 )

        Hence a MatrixH3D can be directly passed to OpenGL.

        So here's the meaning of the 3 members of this union:

        ( m11 m21 m31 m41 )
        ( m12 m22 m32 m42 )
        ( m13 m23 m33 m43 )
        ( m14 m24 m34 m44 )

        ( array[0]  array[4]  array[8]  array[12] )
        ( array[1]  array[5]  array[9]  array[13] )
        ( array[2]  array[6]  array[10] array[14] )
        ( array[3]  array[7]  array[11] array[15] )

        ( array2[0][0] array2[1][0] array2[2][0] array2[3][0] )
        ( array2[0][1] array2[1][1] array2[2][1] array2[3][1] )
        ( array2[0][2] array2[1][2] array2[2][2] array2[3][2] )
        ( array2[0][3] array2[1][3] array2[2][3] array2[3][3] )

        Of course the latter relies on how the C++ language stores
        two-dimensional arrays. I hope I got it right.
        **/
        struct
        {
            double  m11;
            double  m12;
            double  m13;
            double  m14;
            double  m21;
            double  m22;
            double  m23;
            double  m24;
            double  m31;
            double  m32;
            double  m33;
            double  m34;
            double  m41;
            double  m42;
            double  m43;
            double  m44;
        };
        double  array[MATSIZE2];
        double  array2[MATSIZE][MATSIZE];
    };

    MatrixH3D(const MatrixH3D &m);

    MatrixH3D(const double *a);
    MatrixH3D(const double **a);

    /**
      * Copy of the individual elements
      */
    MatrixH3D& operator =(const MatrixH3D &m);


    /**
      * Normal addition of the individual elements
      */
    MatrixH3D& operator +=(const MatrixH3D &m);

    /**
      * Normal subtraction of the individual elements
      */
    MatrixH3D& operator -=(const MatrixH3D &m);

    /**
      * Scalar multiplication of the individual elements by the factor
      */
    MatrixH3D& operator *=(double factor);

    /**
      * Scalar division of the individual elements by the divisor
      */
    MatrixH3D& operator /=(double divisor);

    /**
      * Right matrix mulitiplication
      * "Right" means that a *= b amounts to
      *     a = a * b
      * and not to
      *     a = b * a.
      * Note that in OpenGL, the glMultMatrix function also performs
      * a right multiplication, so that the code snippet:
      *     a *= b;
      *     glMultMatrixd(a);
      * is equivalent to:
      *     glMultMatrixd(a);
      *     glMultMatrixd(b);
      *     a *= b;
      */
    MatrixH3D& operator *=(const MatrixH3D &m);

private:
        
};


/**
 * Normal addition of the individual elements
 */
MatrixH3D operator +(const MatrixH3D &m1, const MatrixH3D &m2);


/**
 * Normal subtraction of the individual elements
 */
MatrixH3D operator -(const MatrixH3D &m1, const MatrixH3D &m2);


/**
 * Matrix mulitiplication
 * This operator calls MatrixH3D::operator*=, so it's slightly slower
 * than MatrixH3D::operator*=
 */
MatrixH3D operator *(const MatrixH3D &m1, const MatrixH3D &m2);


} // Eigen namespace

#endif // MATRIXH3D_H
