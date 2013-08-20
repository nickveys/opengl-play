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


#ifndef VECTORH3D_H
#define VECTORH3D_H

namespace Eigen
{


/** \ingroup h3d
 *
 * \brief Projective (3+1) vector (point in P^3)
 *
 * \author Casper Boemann, Inge Wallin
 *
 * A 3D homogeneus coordinate vector class. This is for linear algebra and not a container
 * 
 * Basically this implements everything you would expect of such a class.
 *
 **/
class VectorH3D
{
public:
    union
    {
        struct
        {
            double x;
            double y;
            double z;
            double w;
        };
        double array[4];
    };

    VectorH3D(const VectorH3D &v);

    VectorH3D(double x=0, double y=0, double z=0, double w=1);

    /**
      * assignment
      */
    VectorH3D&  operator =(const VectorH3D &v);

    /**
      * Normal addition of the individual elements
      */
    VectorH3D & operator +=(const VectorH3D &v);

    /**
      * Normal subtraction of the individual elements
      */
    VectorH3D & operator -=(const VectorH3D &v);

    /**
      * Scalar multiplication of the individual elements by the factor
      */
    VectorH3D & operator *=(double factor);

    /**
      * Scalar division of the individual elements by the divisor
      */
    VectorH3D & operator /=(double divisor);
};

/**
  * Normal addition of the individual elements
  */
VectorH3D operator +(const VectorH3D &v1, const VectorH3D &v2);

/**
  * Normal subtraction of the individual elements
  */
VectorH3D operator -(const VectorH3D &v1, const VectorH3D &v2);
/**
  * Scalar multiplication of the individual elements by the factor
  */
VectorH3D operator *(const VectorH3D &v, double factor);

/**
  * Scalar multiplication of the individual elements by the factor
  */
VectorH3D operator *(double factor, const VectorH3D &v);

/**
  * Scalar division of the individual elements by the divisor
  */
VectorH3D operator /(const VectorH3D &v, double divisor);

/**
  * Cross product
  */
VectorH3D cross(const VectorH3D &v1, const VectorH3D &v2);

/**
  * Dot product
  */
double dot(const VectorH3D &v1, const VectorH3D &v2);

} // Eigen namespace

#endif //VECTORH3D_H
