// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2006 Inge Wallin <inge@lysator.liu.se>
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

#include "vectorh3d.h"

namespace Eigen
{


/**
  * Normal addition of the individual elements
  */
VectorH3D operator +(const VectorH3D &v1, const VectorH3D &v2)
{
        return VectorH3D(v1) += v2;
}

/**
  * Normal subtraction of the individual elements
  */
VectorH3D operator -(const VectorH3D &v1, const VectorH3D &v2)
{
    return VectorH3D(v1) -= v2;
}

/**
  * Scalar multiplication of the individual elements by the factor
  */
VectorH3D operator *(const VectorH3D &v, double factor)
{
    return VectorH3D(v) *= factor;
}

/**
  * Scalar multiplication of the individual elements by the factor
  */
VectorH3D operator *(double factor, const VectorH3D &v)
{
    return VectorH3D(v) *= factor;
}

/**
  * Scalar division of the individual elements by the divisor
  */
VectorH3D operator /(const VectorH3D &v, double divisor)
{
    // should we speed up by multiplying with 1/divisor ?
    return VectorH3D(v) /= divisor;
}

/**
  * Cross product
  */
VectorH3D cross(const VectorH3D &v1, const VectorH3D &v2)
{
    return VectorH3D(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

/**
  * Dot product
  */
double dot(const VectorH3D &v1, const VectorH3D &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

VectorH3D::VectorH3D(const VectorH3D &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

VectorH3D::VectorH3D(double _x, double _y, double _z, double _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

VectorH3D& VectorH3D::operator =(const VectorH3D &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;

    return *this;
}

VectorH3D& VectorH3D::operator +=(const VectorH3D &v)
{
    double tmpW = w;
    if (v.w != 1) {
        x *=v.w;
        y *= v.w;
        z *= v.w;
        w *= v.w;
    }

    if (tmpW != 1 ) {
        x += v.x * tmpW;
        y += v.y * tmpW;
        z += v.z * tmpW;
    } else {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    return *this;
}

VectorH3D& VectorH3D::operator -=(const VectorH3D &v)
{
    double tmpW = w;
    if (v.w != 1) {
        x *=v.w;
        y *= v.w;
        z *= v.w;
        w *= v.w;
    }

    if (tmpW != 1 ) {
        x -= v.x * tmpW;
        y -= v.y * tmpW;
        z -= v.z * tmpW;
    } else {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    return *this;
}

VectorH3D& VectorH3D::operator *=(double factor)
{
    x *= factor;
    y *= factor;
    z *= factor;

    return *this;
}

VectorH3D& VectorH3D::operator /=(double divisor)
{
    x /= divisor;
    y /= divisor;
    z /= divisor;

    return *this;
}


}  // Eigen namespace
