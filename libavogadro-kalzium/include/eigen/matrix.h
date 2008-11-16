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

/** \file matrix.h
  * \brief Matrix and MatrixX class templates
  */

#ifndef EIGEN_MATRIX_H
#define EIGEN_MATRIX_H

#include "ludecomposition.h"
#include "matrixbase.h"
#include "vector.h"

namespace Eigen
{

/** \ingroup fixedsize
  *
  * \ingroup matrices
  *
  * \brief Fixed-size matrix
  *
  * A class for fixed-size square matrices. Thus, a Matrix<T,Size> is the
  * same as a T[Size*Size] array, except that it has convenient
  * operators and methods for basic matrix math. This class is intended to
  * be zero-overhead, as opposed to the slower dynamic-size class MatrixX.
  *
  * The template parameter T is the type of the entries of the matrix.
  * It can be any type representing either real or complex numbers.
  * The template parameter Size is the size of the matrix (both height and
  * width, as Eigen only allows square matrices).
  * The following typedefs are provided to cover the usual cases:
  * @code
    typedef Matrix<double, 2>               Matrix2d;
    typedef Matrix<double, 3>               Matrix3d;
    typedef Matrix<double, 4>               Matrix4d;
    typedef Matrix<float,  2>               Matrix2f;
    typedef Matrix<float,  3>               Matrix3f;
    typedef Matrix<float,  4>               Matrix4f;
    typedef Matrix<std::complex<double>, 2> Matrix2cd;
    typedef Matrix<std::complex<double>, 3> Matrix3cd;
    typedef Matrix<std::complex<double>, 4> Matrix4cd;
    typedef Matrix<std::complex<float>,  2> Matrix2cf;
    typedef Matrix<std::complex<float>,  3> Matrix3cf;
    typedef Matrix<std::complex<float>,  4> Matrix4cf;
  * @endcode
  *
  * If you prefer dynamic-size matrices (they are slower), see the MatrixX
  * class template, which provides exactly the same functionality and API
  * in dynamic-size version.
  *
  * The Matrix class template provides all the usual operators and methods
  * to manipulate square matrices, and can do some more advanced stuff using
  * involving a LU decomposition, like invert a matrix.
  *
  * Here are some examples of usage of Matrix:
  * @code
    using namespace Eigen;
    using namespace std; // we'll use cout for outputting matrices

    Matrix3d mat1, mat2; // constructs two new uninitialized 3x3 matrices
    double array[] = { 2.4, 3.1, -0.7,
                       -1.1, 2.9, 4.3,
                       6.7, -3.7, 2.7 };
    mat1.readArray(array);
        // now mat1 is the following matrix:
        // (  2.4  -1.1   6.7 )
        // (  3.1   2.9  -3.7 )
        // ( -0.7   4.3   2.7 )
    mat2.readRows(array);
        // now mat2 is the following matrix:
        // (  2.4   3.1  -0.7 )
        // ( -1.1   2.9   4.3 )
        // (  6.7  -3.7   2.7 )

    mat1 *= mat2; // computes the matrix product mat1 * mat2, stores it in mat1
    mat1 = mat2 + mat1 * mat2; // there are also non-assignment operators
    mat1 = 0.9 * mat1 + mat2 / 2.6; // you can also multiply/divide by numbers

    Matrix<double,5> mat3; // construct a new uninitialized 5x5 matrix
    mat3.loadIdentity(); // loads the identity matrix of size 5 into mat3

    mat1(2,3) = -1.4; // Stores the value -1.4 at row 2, column 3 of mat1.
    cout << mat1 << endl;

    double vec_coords[3] = {1.1, -2.5, 0.8};
    Vector3d vec( vec_coords ); // construct a vector of size 3.
    vec = mat1 * vec; // multiply vec by mat1

    cout << "determinant of mat1: " << mat1.determinant() << endl;
        // as mat1 has size 3, the determinant is computed by brute force.
        // For larger matrices, this would use a LU decomposition.
  * @endcode
  */
template <typename T, int Size>
class Matrix: public MatrixBase< T,
                                 Matrix<T, Size>,
                                 Vector<T, Size>,
                                 LUDecomposition<T, Size> >
{

    friend  class MatrixBase< T,
                              Matrix<T, Size>,
                              Vector<T, Size>,
                              LUDecomposition<T, Size>
                            >;
    typedef class MatrixBase< T,
                              Matrix<T, Size>,
                              Vector<T, Size>,
                              LUDecomposition<T, Size> >
                  Base;

private:

    /**
      * Returns false. A Matrix<T,Size,Size> doesn't have dynamic size.
      */
    static bool _hasDynamicSize()
    { return false; }

    /**
      * Returns the size of the matrix.
      */
    int _size() const
    { return Size; }

    /**
      * Does nothing. A Matrix<T,Size> can't be resized.
      *
      * if size != size(),
      * a debug message is generated.
      */
    void _resize( int size ) const
    {
        assert( size == this->size() );
    }

public:

    /**
      * Constructs an uninitialized matrix. Really does nothing.
      */
    Matrix() {}

    /**
      * Copy constructor.
      */
    Matrix( const Matrix & other )
    {
        readArray( other.array() );
    }

    /**
      * Constructs a matrix from an array. The matrix entries are read
      * in column-dominant order in the array.
      */
    Matrix( const T * array )
    {
        readArray( array );
    }

    /**
      * Constructs an uninitialized matrix. Really does nothing.
      * This constructor is provided only for compatibility reasons.
      * Of course the size must match the template parameter.
      */
    explicit Matrix( int unused_size )
    {
        assert( unused_size == Size );
    }

    /** \internal \latexonly */
    Matrix & operator = ( const Matrix & other )
    { return Base::operator = ( other ); }

    /** \internal \latexonly */
    Matrix & operator += ( const Matrix & other )
    { return Base::operator += ( other ); }

    /** \internal \latexonly */
    Matrix & operator -= ( const Matrix & other )
    { return Base::operator -= ( other ); }

    /** \internal \latexonly */
    Matrix& operator *=( const T & factor )
    { return Base::operator *= ( factor ); }

    /** \internal \latexonly */
    Matrix& operator /=( const T & factor )
    { return Base::operator /= ( factor ); }

    /** \internal \latexonly */
    Matrix& operator *=( const Matrix & other )
    { return Base::operator *= ( other ); }

protected:

    /** This array stores the size*size entries of the matrix.
      * Currently, we are storing them in column-major order.
      */
    T m_array[ Size * Size ];

};

/** \ingroup dynamicsize
  *
  * \ingroup matrices
  *
  * \brief Dynamic-size matrix
  *
  * A class for dynamic-size square matrices.
  *
  * The template parameter, T, is the type of the entries of the matrix.
  * It can be any type representing either real or complex numbers.
  * The following typedefs are provided to cover the usual cases:
  * @code
    typedef MatrixX<double>                 MatrixXd;
    typedef MatrixX<float>                  MatrixXf;
    typedef MatrixX< std::complex<double> > MatrixXcd;
    typedef MatrixX< std::complex<float> >  MatrixXcf;
  * @endcode
  *
  * If you prefer fixed-size matrices (they are faster), see the Matrix
  * class template, which provides exactly the same functionality and API
  * in fixed-size version.
  *
  * The MatrixX class template provides all the usual operators and methods
  * to manipulate square matrices, and can do some more advanced stuff using
  * involving a LU decomposition, like invert a matrix.
  *
  * Here are some examples of usage of MatrixX:
  * @code
    using namespace Eigen;
    using namespace std; // we'll use cout for outputting matrices

    MatrixXd mat1(3), mat2(3); // constructs two new uninitialized 3x3 matrices
    double array[] = { 2.4, 3.1, -0.7,
                       -1.1, 2.9, 4.3,
                       6.7, -3.7, 2.7 };
    mat1.readArray(array);
        // now mat1 is the following matrix:
        // (  2.4  -1.1   6.7 )
        // (  3.1   2.9  -3.7 )
        // ( -0.7   4.3   2.7 )
    mat2.readRows(array);
        // now mat2 is the following matrix:
        // (  2.4   3.1  -0.7 )
        // ( -1.1   2.9   4.3 )
        // (  6.7  -3.7   2.7 )

    mat1 *= mat2; // computes the matrix product mat1 * mat2, stores it in mat1
    mat1 = mat2 + mat1 * mat2; // there are also non-assignment operators
    mat1 = 0.9 * mat1 + mat2 / 2.6; // you can also multiply/divide by numbers

    MatrixXd mat3(5); // construct a new uninitialized 5x5 matrix
    mat3.loadIdentity(); // loads the identity matrix of size 5 into mat3

    mat1(2,3) = -1.4; // Stores the value -1.4 at row 2, column 3 of mat1.
    cout << mat1 << endl;

    double vec_coords[] = {1.1, -2.5, 0.8};
    VectorXd vec( 3, vec_coords ); // construct a vector of size 3.
    vec = mat1 * vec; // multiply vec by mat1

    cout << "determinant of mat1: " << mat1.determinant() << endl;
        // as mat1 has size 3, the determinant is computed by brute force.
        // For larger matrices, this would use a LU decomposition.
  * @endcode
  */
template<typename T>
class MatrixX : public MatrixBase< T,
                                   MatrixX<T>,
                                   VectorX<T>,
                                   LUDecompositionX<T>
                                 >
{
    friend  class MatrixBase< T,
                              MatrixX<T>,
                              VectorX<T>,
                              LUDecompositionX<T> >;
    typedef class MatrixBase< T,
                              MatrixX<T>,
                              VectorX<T>,
                              LUDecompositionX<T> >
                  Base;

public:

    /**
      * Constructs an uninitialized square matrix with given size (dimension).
      * The default value for size is 1.
      */
    explicit MatrixX(int size = 1)
    { init(size); }

    /**
      * Copy constructor.
      */
    MatrixX( const MatrixX & other )
    {
        init( other.size() );
        readArray( other.array() );
    }

    /**
      * Constructs a matrix with given size from an array.
      * The matrix entries must be stored in column-dominant order in the array.
      * @param array the array. It must have dimension
      *              at least size*size.
      * @param size the size of the matrix (number of rows, or equivalently
      *             number of columns).
      */
    MatrixX( int size, const T * array )
    {
        init( size );
        readArray( array );
    }

    /**
      * Destructor, frees the memory allocated for the matrix's array
      */
    ~MatrixX()
    { delete[] m_array; }

    /** \internal \latexonly */
    MatrixX & operator = ( const MatrixX & other )
    { return Base::operator = ( other ); }

    /** \internal \latexonly */
    MatrixX & operator += ( const MatrixX & other )
    { return Base::operator += ( other ); }

    /** \internal \latexonly */
    MatrixX & operator -= ( const MatrixX & other )
    { return Base::operator -= ( other ); }

    /** \internal \latexonly */
    MatrixX& operator *=( const T & factor )
    { return Base::operator *= ( factor ); }

    /** \internal \latexonly */
    MatrixX& operator /=( const T & factor )
    { return Base::operator /= ( factor ); }

    /** \internal \latexonly */
    MatrixX& operator *=( const MatrixX & other )
    { return Base::operator *= ( other ); }

protected:

    /** The size (number of rows, or equivalently number of columns)
      * of the matrix. */
    int m_size;

    /** This array stores the size*size entries of the matrix.
      * Currently, we are storing them in column-major order.
      */
    T *m_array;

    /**
      * Small helper function for the constructors
      */
    void init( int size )
    {
        m_size = size;
        m_array  = new T[m_size * m_size];
    }

private:

    /** \internal \latexonly
      * @returns the matrix size (number of rows)
      */
    int _size() const
    { return m_size; }

    /** \internal \latexonly
      * Returns true. A MatrixX has dynamic size.
      */
    static bool _hasDynamicSize()
    { return true; }

    /**
      * Resizes the matrix. The matrix entries are not kept, they're left
      * with undefined values after resizing.
      *
      * @param size the new matrix size
      */
    void _resize( int size );
};

template<typename T>
void MatrixX<T>::_resize( int size )
{
    if( size == m_size ) return;
    if( size > m_size )
    {
        delete[] m_array;
        m_array  = new T[size * size];
    }
    m_size = size;
}

EIGEN_MAKE_FIXEDSIZE_TYPEDEFS(Matrix)
EIGEN_MAKE_DYNAMICSIZE_TYPEDEFS(MatrixX)

}

#endif // EIGEN_MATRIX_H
