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

/** \file linearsolverbase.h
  * \brief Internal file
  */

#ifndef EIGEN_LINEARSOLVERBASE_H
#define EIGEN_LINEARSOLVERBASE_H

#include "util.h"

namespace Eigen
{

/** \ingroup internalbases
  *
  * \ingroup solving
  *
  * \brief internal base class
  *
  * This class template is only internally used in Eigen.
  */
template<typename T, typename MatrixType,
         typename VectorType, typename LUDecompositionType>
class LinearSolverBase
{

public:

    ~LinearSolverBase() {}

    /**
      * Returns a reference to a vector (kept alive by this class)
      * that is a solution of the system of
      * equations, if any solution exists. If there exist no solutions, the
      * return value is undefined.
      */
    const VectorType & someSolution()
    {
        return m_someSolution;
    }

    /**
      * In short, this method returns a reference to a solution vector
      * (kept alive by this class) of the system, corresponding
      * to a given parameter. The parameter must be an array of size equal to
      * dimSolutions(). Any such parameter array will give a solution, and
      * each solution corresponds to a unique parameter. Thus this function
      * gives a complete description of the space of solutions.
      *
      * @param parameter An array with size equal to dimSolutions().
      * @returns A pointer to the solution associated to this parameter
      * vector. If no such solution exists (that is, if dimSolutions()==0),
      * the return value is undefined.
      */
    const VectorType & genericSolution( const T * parameter );

    /**
      * Assuming that there exist solutions (which can be checked with
      * thereExistSolutions()), this function returns the dimension of the
      * space of solutions. Thus, 0 means that there exists exactly one
      * solution, and a positive value means that there exist infinitely
      * many solutions, forming a space of the given dimension.
      */
    int dimSolutions()
    {
        return m_luDecomposition.dimKer();
    }

    /**
      * This function returns true if there exists at least one solution.
      * It returns false if there are no solutions.
      */
    bool thereExistSolutions()
    {
        return( m_thereExistSolutions );
    }

protected:

    /** helper for the constructors */
    void init( const MatrixType & leftHandSide,
               const VectorType & rightHandSide );

    /** Equals true if at least one solution exists */
    bool m_thereExistSolutions;

    /** Stores the LU decomposition that is used for computations. */
    LUDecompositionType m_luDecomposition;

    /** Stores some solution of the system, or 0 if there are
      * no solutions. Used as a base point of the space of solutions.
      * Returned by someSolution().
      */
    VectorType m_someSolution;

    /** Stores the vector that was last returned by genericSolution().
      */
    VectorType m_genericSolution;

    /** The right hand side vector. */
    VectorType m_rightHandSide;

    /** A matrix whose column vectors form a basis of the kernel of the
        left hand side. */
    MatrixType m_basisKer;

    /** Equals true if m_basisKer has already been computed */
    bool m_computedBasisKer;
};

template<typename T, typename MatrixType,
         typename VectorType, typename LUDecompositionType>
void
LinearSolverBase<T, MatrixType,
                  VectorType, LUDecompositionType>
    ::init( const MatrixType & leftHandSide,
               const VectorType & rightHandSide )
{
    m_computedBasisKer = false;
    m_luDecomposition.perform( leftHandSide );
    m_rightHandSide = rightHandSide ;
    m_someSolution.resize( m_rightHandSide.size() );
    m_thereExistSolutions
        = m_luDecomposition.computeSomeAntecedent
                                ( m_rightHandSide, &m_someSolution );
}

template<typename T, typename MatrixType,
         typename VectorType, typename LUDecompositionType>
const VectorType &
LinearSolverBase<T, MatrixType,
                  VectorType, LUDecompositionType>
    ::genericSolution( const T * parameter )
{
    if( ! m_computedBasisKer )
    {
        m_basisKer.resize( m_luDecomposition.dim() );
        m_genericSolution.resize( m_luDecomposition.dim() );
        m_luDecomposition.computeBasisKer( & m_basisKer );
        m_computedBasisKer = true;
    }
    for( int i = 0; i < m_genericSolution.size(); i++ )
    {
        T & coord = m_genericSolution[i];
        coord = m_someSolution[i];
        for( int j = 0; j < dimSolutions(); j++ )
            coord += parameter[j] * m_basisKer( i, j );
    }
    return m_genericSolution;
}

} // namespace Eigen

#endif // EIGEN_LINEARSOLVERBASE_H
