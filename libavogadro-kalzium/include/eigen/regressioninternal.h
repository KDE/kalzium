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

/** \file regressioninternal.h
  * \brief Internal file
  */

#ifndef EIGEN_REGRESSIONINTERNAL_H
#define EIGEN_REGRESSIONINTERNAL_H

#include "util.h"

namespace Eigen
{

/** \internal
  *
  * \ingroup internalbases
  *
  * Internal function for linearRegression() and linearRegressionX().
  *
  * See the documentation of linearRegression() for details.
  */
template< typename T,
          typename VectorType,
          typename MatrixType >
void linearRegression_internal( int numPoints,
                                const VectorType * points,
                                VectorType * retCoefficients,
                                int funcOfOthers )
{
    assert( numPoints >= 1 );
    int i, size = points[0].size();
    assert( funcOfOthers >= 0 && funcOfOthers < size );
    assert( size == retCoefficients->size() );

    MatrixType matrix( size );
    matrix.loadZero();
    VectorType vector( size );
    vector.loadZero();

    for( i = 0; i < numPoints; i++)
    {
        assert( size == points[i].size() );
        VectorType cur_vec( size );
        T coord_funcOfOthers = points[i]( funcOfOthers );
        for( int j = 0; j < funcOfOthers; j++)
            cur_vec(j) = points[i](j);
        for( int j = funcOfOthers; j < size - 1; j++ )
            cur_vec(j) = points[i]( j + 1 );
        cur_vec( size - 1 ) = static_cast<T>(1);

        for( int row = 0; row < size; row++ )
        {
            T cur_vec_row = cur_vec(row);
            vector(row) += Util::conj( cur_vec_row ) * coord_funcOfOthers;
            matrix( row, row ) += Util::conj( cur_vec_row ) * cur_vec_row;
            for( int col = 0; col < row; col++ )
            {
                T product = Util::conj( cur_vec_row ) * cur_vec(col);
                matrix( row, col ) += product;
                matrix( col, row ) += Util::conj( product );
            }
        }
    }

    matrix.computeSomeAntecedent( vector,
                                  retCoefficients );
}

/** \internal
  *
  * \ingroup internalbases
  *
  * Base function for computeFittingHyperplane() and
  * computeFittingHyperplaneX().
  *
  * See the documentation of computeFittingHyperplane() and
  * linearRegression() for details.
  */
template< typename T,
          typename VectorType,
          typename BigVecType,
          typename MatrixType >
void computeFittingHyperplane_internal( int numPoints,
                                        const VectorType * points,
                                        BigVecType * retCoefficients )
{
    assert( numPoints >= 1 );
    int i, size = points[0].size();
    assert( ( size + 1 ) == retCoefficients->size() );

    // let's compute roughly, for each coord, how much it varies
    // across points. Since a rough estimate is enough, a linear algorithm
    // fixing a base point will do.
    VectorType amplitude( size );
    amplitude.loadZero();
    for( i = 1; i < numPoints; i++ )
    {
        VectorType diff( points[i] - points[0] );

        for( int j = 0; j < size; j++ )
        {
            if( std::abs(diff(j)) > std::abs(amplitude(j)) )
                amplitude(j) = diff(j);
        }
    }

    // now let's find out which coord varies the least. Again, this is
    // approximative. All that matters is that we don't pick a coordinate
    // that varies orders of magnitude more than another one.
    T min_amplitude = amplitude(0);
    int index_min_amplitude = 0;
    for( i = 1; i < size; i++ )
    {
        if( std::abs(amplitude(i)) < std::abs(min_amplitude) )
        {
            min_amplitude = amplitude(i);
            index_min_amplitude = i;
        }
    }

    // let's now perform a linear regression with respect to that
    // not-too-much-varying coord
    VectorType affineCoefficients( size );
    linearRegression_internal< T, VectorType, MatrixType >
        ( numPoints, points, & affineCoefficients, index_min_amplitude );

    // let's now contruct retCoefficients
    for( i = 0; i < index_min_amplitude; i++ )
        (*retCoefficients)(i) = affineCoefficients(i);
    (*retCoefficients)(index_min_amplitude) = static_cast<T>(-1);
    for( i = index_min_amplitude + 1; i < size + 1; i++ )
        (*retCoefficients)(i) = affineCoefficients( i - 1 );
}

} // namespace Eigen

#endif // EIGEN_REGRESSIONINTERNAL_H
