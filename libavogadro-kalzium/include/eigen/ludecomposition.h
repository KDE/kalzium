// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2006-2007 Benoit Jacob <jacob@math.jussieu.fr>
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

/** \file ludecomposition.h
  * \brief LU decompositions and related computations
  */

#ifndef EIGEN_LUDECOMPOSITION_H
#define EIGEN_LUDECOMPOSITION_H

#include "ludecompositionbase.h"

namespace Eigen
{

template<typename T, typename MatrixType,
         typename VectorType, typename LUDecompositionType>
    class LinearSolverBase;

template<typename T, int Size> class Matrix;
template<typename T, int Size> class Vector;

/** \ingroup fixedsize
  *
  * \ingroup ludecomp
  *
  * \brief LU decomposition of a fixed-size matrix
  *
  * The template parameter T is the type of the entries of the matrix to be
  * decomposed.
  * It can be any type representing either real or complex numbers.
  * The template parameter Size is the size of the matrix (both height and
  * width, as Eigen only allows square matrices).
  * The following typedefs are provided to cover the usual cases:
  * @code
    typedef LUDecomposition<double, 2>               LUDecomposition2d;
    typedef LUDecomposition<double, 3>               LUDecomposition3d;
    typedef LUDecomposition<double, 4>               LUDecomposition4d;
    typedef LUDecomposition<float,  2>               LUDecomposition2f;
    typedef LUDecomposition<float,  3>               LUDecomposition3f;
    typedef LUDecomposition<float,  4>               LUDecomposition4f;
    typedef LUDecomposition<std::complex<double>, 2> LUDecomposition2cd;
    typedef LUDecomposition<std::complex<double>, 3> LUDecomposition3cd;
    typedef LUDecomposition<std::complex<double>, 4> LUDecomposition4cd;
    typedef LUDecomposition<std::complex<float>,  2> LUDecomposition2cf;
    typedef LUDecomposition<std::complex<float>,  3> LUDecomposition3cf;
    typedef LUDecomposition<std::complex<float>,  4> LUDecomposition4cf;
  * @endcode
  */
template<typename T, int Size>
class LUDecomposition
    : public LUDecompositionBase< T,
                                  Matrix<T, Size>,
                                  Vector<T, Size>,
                                  Vector<int, Size> >
{

    friend class LinearSolverBase< T,
                                   Matrix<T, Size>,
                                   Vector<T, Size>,
                                   LUDecomposition<T, Size> >;

public:

    /** Performs the LU Decomposition of mat. Use this constructor. */
    LUDecomposition( const Matrix<T, Size> & mat ) { perform( mat ); }

protected:
    /** Default constructor. Does nothing. \internal
      */
    LUDecomposition() {}
};

template<typename T> class MatrixX;
template<typename T> class VectorX;

/** \ingroup dynamicsize
  *
  * \ingroup ludecomp
  *
  * \brief LU decomposition of a dynamic-size matrix
  *
  * The template parameter T is the type of the entries of the matrix to be
  * decomposed.
  * It can be any type representing either real or complex numbers.
  * The following typedefs are provided to cover the usual cases:
  * @code
    typedef LUDecompositionX<double>                 LUDecompositionXd;
    typedef LUDecompositionX<float>                  LUDecompositionXf;
    typedef LUDecompositionX< std::complex<double> > LUDecompositionXcd;
    typedef LUDecompositionX< std::complex<float> >  LUDecompositionXcf;
  * @endcode
  */
template<typename T>
class LUDecompositionX
    : public LUDecompositionBase< T,
                                  MatrixX<T>,
                                  VectorX<T>,
                                  VectorX<int> >
{

    friend class LinearSolverBase< T,
                                   MatrixX<T>,
                                   VectorX<T>,
                                   LUDecompositionX<T> >;

public:
    /** Performs the LU Decomposition of mat. Use this constructor. */
    LUDecompositionX( const MatrixX<T> & mat ) { perform( mat ); }

protected:
    /** Default constructor. Does nothing. \internal
      */
    LUDecompositionX() {}
};

EIGEN_MAKE_FIXEDSIZE_TYPEDEFS(LUDecomposition)
EIGEN_MAKE_DYNAMICSIZE_TYPEDEFS(LUDecompositionX)

} // namespace Eigen

#endif // EIGEN_LUDECOMPOSITION_H
