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

/** \file regression.h
  * \brief Linear regression analysis
  */

#ifndef EIGEN_REGRESSION_H
#define EIGEN_REGRESSION_H

#include "matrix.h"
#include "regressioninternal.h"

namespace Eigen
{

/** \ingroup regression
  *
  * Performs a multiple linear regression on a set of points, as explained
  * here:
  *
  * http://en.wikipedia.org/wiki/Linear_regression#Multiple_linear_regression
  *
  * In other words, for a set of points, this function tries to express
  * one of the coords as a linear (affine) function of the other coords.
  *
  * This is best explained by an example. This function works in full
  * generality, for points in a space of arbitrary dimension, and also over
  * the complex numbers, but for this example we will work in dimension 3
  * over the real numbers (doubles).
  *
  * So let us work with the following set of 5 points given by their
  * \f$(x,y,z)\f$ coordinates:
  * @code
    Vector3d points[5];
    points[0] = Vector3d( 3.02, 6.89, -4.32 );
    points[1] = Vector3d( 2.01, 5.39, -3.79 );
    points[2] = Vector3d( 2.41, 6.01, -4.01 );
    points[3] = Vector3d( 2.09, 5.55, -3.86 );
    points[4] = Vector3d( 2.58, 6.32, -4.10 );
  * @endcode
  * Suppose that we want to express the second coordinate (\f$y\f$) as a linear
  * expression in \f$x\f$ and \f$z\f$, that is,
  * \f[ y=ax+bz+c \f]
  * for some constants \f$a,b,c\f$. Thus, we want to find the best possible
  * constants \f$a,b,c\f$ so that the plane of equation \f$y=ax+bz+c\f$ fits
  * best the five above points. To do that, call this function as follows:
  * @code
    Vector3d coeffs; // will store the coefficients a, b, c
    linearRegression< double, 3 >( 5, points, & coeffs,
                                   1 // the coord to express as a function of
    // the other ones. 0 means x, 1 means y, 2 means z.
                                 );
  * @endcode
  * Now the vector \a coeffs is approximately
  * \f$( 0.495 ,  -1.927 ,  -2.906 )\f$.
  * Thus, we get \f$a=0.495, b = -1.927, c = -2.906\f$. Let us check for
  * instance how near points[0] is from the plane of equation \f$y=ax+bz+c\f$.
  * Looking at the coords of points[0], we see that:
  * \f[ax+bz+c = 0.495 * 3.02 + (-1.927) * (-4.32) + (-2.906) = 6.91.\f]
  * On the other hand, we have \f$y=6.89\f$. We see that the values
  * \f$6.91\f$ and \f$6.89\f$
  * are near, so points[0] is very near the plane of equation \f$y=ax+bz+c\f$.
  *
  * Let's now describe precisely the parameters:
  * @param numPoints the number of points to read from the array
  * @param points the array of points on which to perform the linear regression
  * @param retCoefficients pointer to the vector in which to store the result.
                           This vector must be of the same type and size as the
                           data points. The meaning of its coords is as follows.
                           For brevity, let \f$n=Size\f$,
                           \f$r_i=retCoefficients[i]\f$,
                           and \f$f=funcOfOthers\f$. Denote by
                           \f$x_0,\ldots,x_{n-1}\f$
                           the n coordinates in the n-dimensional space.
                           Then the result equation is:
                           \f[ x_f = r_0 x_0 + \cdots + r_{f-1}x_{f-1}
                            + r_{f+1}x_{f+1} + \cdots + r_{n-1}x_{n-1} + r_n. \f]
  * @param funcOfOthers Determines which coord to express as a function of the
                        others. Coords are numbered starting from 0, so that a
                        value of 0 means \f$x\f$, 1 means \f$y\f$,
                        2 means \f$z\f$, ...
  *
  * \sa computeFittingHyperplane()
  */
template< typename T, int Size >
inline void linearRegression( int numPoints,
                              const Vector<T,Size> * points,
                              Vector<T,Size> * retCoefficients,
                              int funcOfOthers )
{
    linearRegression_internal< T, Vector<T,Size>, Matrix<T,Size> >
                        ( numPoints, points, retCoefficients, funcOfOthers );
}

/** \ingroup regression
  *
  * This function is quite similar to linearRegression(), so we refer to the
  * documentation of this function and only list here the differences.
  *
  * The main difference from linearRegression() is that this function doesn't
  * take a \a funcOfOthers argument. Instead, it finds a general equation
  * of the form
  * \f[ r_0 x_0 + \cdots + r_{n-1}x_{n-1} + r_n = 0, \f]
  * where \f$n=Size\f$, \f$r_i=retCoefficients[i]\f$, and we denote by
  * \f$x_0,\ldots,x_{n-1}\f$ the n coordinates in the n-dimensional space.
  *
  * Thus, the vector \a retCoefficients has size \f$n+1\f$, which is another
  * difference from linearRegression().
  *
  * \sa linearRegression()
  */
template< typename T, int Size >
inline void computeFittingHyperplane( int numPoints,
                                      const Vector<T,Size> * points,
                                      Vector<T,Size+1> * retCoefficients )
{
    computeFittingHyperplane_internal< T, Vector<T,Size>, Vector<T,Size+1>,
                                       Matrix<T,Size> >
                        ( numPoints, points, retCoefficients );
}

/** \ingroup regression
  *
  * This function is the dynamic-size counterpart to linearRegression()
  * and, aside from working with VectorX instead of Vector, is exactly the
  * same thing.
  *
  * \sa computeFittingHyperplaneX()
  */
template< typename T >
inline void linearRegressionX( int numPoints,
                              const VectorX<T> * points,
                              VectorX<T> * retCoefficients,
                              int funcOfOthers )
{
    linearRegression_internal< T, VectorX<T>, MatrixX<T> >
                        ( numPoints, points, retCoefficients, funcOfOthers );
}

/** \ingroup regression
  *
  * This function is the dynamic-size counterpart to computeFittingHyperplane()
  * and, aside from working with VectorX instead of Vector, is exactly the
  * same thing.
  *
  * \sa linearRegressionX()
  */
template< typename T >
inline void computeFittingHyperplaneX( int numPoints,
                                      const VectorX<T> * points,
                                      VectorX<T> * retCoefficients )
{
    computeFittingHyperplane_internal< T, VectorX<T>, VectorX<T>, MatrixX<T> >
                        ( numPoints, points, retCoefficients );
}

} // namespace Eigen

#endif // EIGEN_REGRESSION_H
