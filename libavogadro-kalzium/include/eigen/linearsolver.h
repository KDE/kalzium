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

/** \file linearsolver.h
  * \brief Solving systems of linear equations
  */

#ifndef EIGEN_LINEARSOLVER_H
#define EIGEN_LINEARSOLVER_H

#include "matrix.h"
#include "ludecomposition.h"
#include "linearsolverbase.h"

namespace Eigen
{

/** \ingroup fixedsize
  *
  * \ingroup solving
  *
  * \brief Solver for systems of linear equations (fixed-size)
  *
  * This class provides an easy way to solve systems of linear equations.
  *
  * The template parameter T is the type of the numbers over which the
  * equations are to be solved.
  * It can be any type representing either real or complex numbers.
  * The template parameter Size is the number of equations, or
  * equivalently the number of indeterminates, as Eigen only allows square
  * matrices.
  * The following typedefs are provided to cover the usual cases:
  * @code
    typedef LinearSolver<double, 2>               LinearSolver2d;
    typedef LinearSolver<double, 3>               LinearSolver3d;
    typedef LinearSolver<double, 4>               LinearSolver4d;
    typedef LinearSolver<float,  2>               LinearSolver2f;
    typedef LinearSolver<float,  3>               LinearSolver3f;
    typedef LinearSolver<float,  4>               LinearSolver4f;
    typedef LinearSolver<std::complex<double>, 2> LinearSolver2cd;
    typedef LinearSolver<std::complex<double>, 3> LinearSolver3cd;
    typedef LinearSolver<std::complex<double>, 4> LinearSolver4cd;
    typedef LinearSolver<std::complex<float>,  2> LinearSolver2cf;
    typedef LinearSolver<std::complex<float>,  3> LinearSolver3cf;
    typedef LinearSolver<std::complex<float>,  4> LinearSolver4cf;
  * @endcode
  *
  * For instance, suppose that you want to solve the system of equations:
  * @code
        2x + 3y -  z  = 1
        4x -  y + 7z  = -6
        -x + 2y + 5z  = 2
  * @endcode
  * First, store the left hand side of this system in a matrix:
  * @code
        double lhs_rows[3][3] = { { 2,  3, -1 },
                                  { 4, -1,  7 },
                                  { -1, 2,  5 } };
        Matrix3d lhs;
        lhs.readRows( &lhs_rows[0][0] );
  * @endcode
  * Next, store the right hand side of this system in a vector:
  * @code
        double rhs_coords[3] = { 1, -6, 2 };
        Vector3d rhs( rhs_coords );
  * @endcode
  * Next, construct a LinearSolver:
  * @code
        LinearSolver3d solver( lhs, rhs );
  * @endcode
  * Now, you can use the methods of LinearSolver to compute the solutions.
  * If all you need is to know whether there exist solutions and to compute
  * a solution if any exists, then just do:
  * @code
        if( solver.thereExistSolutions() )
        {
            cout << "There are solutions! Here's one:" << endl;
            cout << solver.someSolution() << endl;
        }
        else cout << "There are no solutions." << endl;
  * @endcode
  *
  * If you want a complete analysis of the space of solutions,
  * you need to use the methods genericSolution() and dimSolutions().
  * The following example shows how to use them:
  * @code
        if( solver.thereExistSolutions() )
        {
            cout << "There are solutions! Here's one:" << endl;
            cout << solver.someSolution() << endl;

            if( solver.dimSolutions() == 0 )
                cout << "And this is the only solution." << endl;
            else
            {
                cout << "The space of solutions is of dimension "
                     << solver.dimSolutions() << endl;
                cout << "Here's another solution:" << endl;

                // let's construct a random parameter vector. This vector needs
                // to have size equal to dimSolutions(), but as this can't be
                // larger than 3, it's simpler to allocate an array of size 3.
                double param [3];
                for( int i = 0; i < solver.dimSolutions(); i++ )
                    param[i] = -10.0 + 20.0 * rand() / RAND_MAX;

                cout << solver.genericSolution( param ) << endl;
            }
        }
        else cout << "There are no solutions." << endl;
  * @endcode
  *
  * NOTE: As Eigen only handles square matrices, the number of equations
  * must be equal to the number of variables.
  * If you need to solve systems with more variables than equations, you
  * can simply add trivial equations like 0x+0y+0z=0 to the system, in order
  * to have as many equations as you have variables.
  */
template<typename T, int Size>
class LinearSolver
    : public LinearSolverBase< T,
                               Matrix<T, Size>,
                               Vector<T, Size>,
                               LUDecomposition<T, Size> >
{
public:

    /**
      * Constructor recommended for convenient usage of this class:
      * directly sets both sides of the system of equations.
      */
    LinearSolver( const Matrix<T, Size> & leftHandSide,
                  const Vector<T, Size> & rightHandSide )
    {
        init( leftHandSide, rightHandSide );
    }
};

/** \ingroup dynamicsize
  *
  * \ingroup solving
  *
  * \brief Solver for systems of linear equations (dynamic-size)
  *
  * This class provides an easy way to solve systems of linear equations.
  *
  * The template parameter T is the type of the numbers over which the
  * equations are to be solved.
  * It can be any type representing either real or complex numbers.
  * The following typedefs are provided to cover the usual cases:
  * @code
    typedef LinearSolverX<double>                 LinearSolverXd;
    typedef LinearSolverX<float>                  LinearSolverXf;
    typedef LinearSolverX< std::complex<double> > LinearSolverXcd;
    typedef LinearSolverX< std::complex<float> >  LinearSolverXcf;
  * @endcode
  *
  * For instance, suppose that you want to solve the system of equations:
  * @code
        2x + 3y -  z  = 1
        4x -  y + 7z  = -6
        -x + 2y + 5z  = 2
  * @endcode
  * First, store the left hand side of this system in a matrix:
  * @code
        double lhs_rows[3][3] = { { 2,  3, -1 },
                                  { 4, -1,  7 },
                                  { -1, 2,  5 } };
        MatrixXd lhs( 3 );
        lhs.readRows( &lhs_rows[0][0] );
  * @endcode
  * Next, store the right hand side of this system in a vector:
  * @code
        double rhs_coords[3] = { 1, -6, 2 };
        VectorXd rhs( 3, rhs_coords );
  * @endcode
  * Next, construct a LinearSolverX:
  * @code
        LinearSolverXd solver( lhs, rhs );
  * @endcode
  * Now, you can use the methods of LinearSolverX to compute the solutions.
  * If all you need is to know whether there exist solutions and to compute
  * a solution if any exists, then just do:
  * @code
        if( solver.thereExistSolutions() )
        {
            cout << "There are solutions! Here's one:" << endl;
            cout << solver.someSolution() << endl;
        }
        else cout << "There are no solutions." << endl;
  * @endcode
  *
  * If you want a complete analysis of the space of solutions,
  * you need to use the methods genericSolution() and dimSolutions().
  * The following example shows how to use them:
  * @code
        if( solver.thereExistSolutions() )
        {
            cout << "There are solutions! Here's one:" << endl;
            cout << solver.someSolution() << endl;

            if( solver.dimSolutions() == 0 )
                cout << "And this is the only solution." << endl;
            else
            {
                cout << "The space of solutions is of dimension "
                     << solver.dimSolutions() << endl;
                cout << "Here's another solution:" << endl;

                // let's construct a random parameter vector. This vector needs
                // to have size equal to dimSolutions(), but as this can't be
                // larger than 3, it's simpler to allocate an array of size 3.
                double param [3];
                for( int i = 0; i < solver.dimSolutions(); i++ )
                    param[i] = -10.0 + 20.0 * rand() / RAND_MAX;

                cout << solver.genericSolution( param ) << endl;
            }
        }
        else cout << "There are no solutions." << endl;
  * @endcode
  *
  * NOTE: As Eigen only handles square matrices, the number of equations
  * must be equal to the number of variables.
  * If you need to solve systems with more variables than equations, you
  * can simply add trivial equations like 0x+0y+0z=0 to the system, in order
  * to have as many equations as you have variables.
  */
template<typename T>
class LinearSolverX
    : public LinearSolverBase< T,
                               MatrixX<T>,
                               VectorX<T>,
                               LUDecompositionX<T> >
{
public:

    /**
      * Default constructor.
      */
    LinearSolverX() { this->init(); }

    /**
      * Constructor recommended for convenient usage of this class:
      * directly sets both sides of the system of equations.
      */
    LinearSolverX( const MatrixX<T> & leftHandSide,
                  const VectorX<T> & rightHandSide )
    {
        init( leftHandSide, rightHandSide);
    }
};

EIGEN_MAKE_FIXEDSIZE_TYPEDEFS(LinearSolver)
EIGEN_MAKE_DYNAMICSIZE_TYPEDEFS(LinearSolverX)

} // namespace Eigen

#endif // EIGEN_LINEARSOLVERX_H
