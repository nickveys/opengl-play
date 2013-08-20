// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2006 Inge Wallin <inge@lysator.liu.se>
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

#include <string.h>

#include "matrixh3d.h"


namespace Eigen
{


MatrixH3D::MatrixH3D(const MatrixH3D &m)
{
    memcpy(&array[0], &m.array[0], sizeof(double) * MATSIZE2);
}


MatrixH3D::MatrixH3D(const double *a)
{
    memcpy(&array[0], &a[0], sizeof(double) * MATSIZE2);
}


MatrixH3D::MatrixH3D(const double **a)
{
    memcpy(&array2[0][0], &a[0][0], sizeof(double) * MATSIZE2);
}


MatrixH3D& MatrixH3D::operator =(const MatrixH3D &m)
{
    memcpy(&array[0], &m.array[0], sizeof(double) * MATSIZE2);

    return *this;
}


// ----------------------------------------------------------------


/**
 * Normal addition of the individual elements
 */
MatrixH3D operator +(const MatrixH3D &m1, const MatrixH3D &m2)
{
    MatrixH3D res;

    for (int i = 0; i < MatrixH3D::MATSIZE2; i++) {
        res.array[i] = m1.array[i] + m2.array[i];
    }
    return res;
}


/**
 * Normal subtraction of the individual elements
 */
MatrixH3D operator -(const MatrixH3D &m1, const MatrixH3D &m2)
{
    MatrixH3D res;

    for (int i = 0; i < MatrixH3D::MATSIZE2; i++) {
        res.array[i] = m1.array[i] - m2.array[i];
    }
    return res;
}


/**
 * Matrix mulitiplication
 */

MatrixH3D operator *(const MatrixH3D &m1, const MatrixH3D &m2)
{
    MatrixH3D  res(m1);
    res *= m2;

    return res;
}




MatrixH3D& MatrixH3D::operator +=(const MatrixH3D &m)
{
    for (int i = 0; i < MATSIZE2; i++) {
        array[i] += m.array[i];
    }

    return *this;
}



MatrixH3D& MatrixH3D::operator -=(const MatrixH3D &m)
{
    for (int i = 0; i < MATSIZE2; i++) {
        array[i] -= m.array[i];
    }

    return *this;
}



MatrixH3D& MatrixH3D::operator *=(double factor)
{
    for (int i = 0; i < MATSIZE2; i++) {
	array[i] *= factor;
    }

    return *this;
}



MatrixH3D& MatrixH3D::operator /=(double divisor)
{
    for (int i = 0; i < MATSIZE2; i++) {
	array[i] /= divisor;
    }

    return *this;
}



MatrixH3D& MatrixH3D::operator *=(const MatrixH3D &m)
{
    MatrixH3D res;

    for (int x = 0; x < MatrixH3D::MATSIZE; x++) {
        for (int y = 0; y < MatrixH3D::MATSIZE; y++) {
                res.array2[x][y] = array2[0][y] * m.array2[x][0]
                                 + array2[1][y] * m.array2[x][1]
                                 + array2[2][y] * m.array2[x][2]
                                 + array2[3][y] * m.array2[x][3];
        }
    }

    *this = res;
    return *this;
}




}  // Eigen namespace
