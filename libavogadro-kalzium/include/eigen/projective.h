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

/** \file projective.h
  * \brief Projective geometry
  */

#ifndef EIGEN_MATRIXP_H
#define EIGEN_MATRIXP_H

#include "matrix.h"

namespace Eigen
{

/** \ingroup projective
  *
  * Converts an "affine" vector into the equivalent "projective" vector.
  * Thus, if the input affine vector is \f$(x_1,\ldots,x_n)\f$, then the
  * output projective vector is \f$(x_1,\ldots,x_n,1)\f$.
  *
  * \sa projToAff()
  */
template< typename T, int Size>
Vector<T, Size+1> & affToProj( const Vector<T, Size> & vector,
                             Vector<T, Size+1> * ret )
{
    for( int i = 0; i < Size; i++ ) (*ret)(i) = vector(i);
    (*ret)(Size) = static_cast<T>(1);
    return *ret;
}

/** \ingroup projective
  *
  * Converts a "projective" vector into the equivalent "affine" vector.
  * Thus, if the input projective vector is \f$(x_1,\ldots,x_{n+1})\f$,
  * then the output affine vector is
  * \f[(\frac{x_1}{x_{n+1}},\ldots,\frac{x_n}{x_{n+1}}).\f]
  *
  * \sa affToProj()
  */
template< typename T, int Size>
Vector<T, Size> & projToAff( const Vector<T, Size+1> & vector,
                             Vector<T, Size> * ret )
{
    for( int i = 0; i < Size; i++ ) (*ret)(i) = vector(i) / vector(Size);
    return *ret;
}

/** \ingroup projective
  *
  * \ingroup matrices
  *
  * \ingroup fixedsize
  *
  * \brief Matrix in projective geometry, fixed-size only
  *
  * It's sometimes useful to add one dimension to the space you're working in.
  * For instance, that allows to represent translations as matrices, which is
  * otherwise impossible.
  *
  * Internally, a MatrixP<T, n> is just a Matrix<T, n+1>. The difference is
  * that it's regarded as a homography acting on the n-dimensional projective
  * space, instead of being regarded as a linear map acting on the n-dimensional
  * vector space.
  *
  * The correspondence between "ordinary" vectors of size n and
  * "projective" vectors of size n+1 is as follows. Given an ordinary vector
  * \f$(x_1,\ldots,x_n)\f$, the corresponding projective vector is
  * \f$(x_1,\ldots,x_n,1)\f$. Conversely, given a projective vector
  * \f$(x_1,\ldots,x_n,x_{n+1})\f$, the corresponding ordinary vector is
  * \f$(\frac{x_1}{x_{n+1}},\ldots,\frac{x_n}{x_{n+1}})\f$. If \f$x_{n+1}=0\f$,
  * then the projective vector doesn't correspond to any ordinary one, and
  * is called a "direction at infinity".
  *
  * The following typedefs are provided to cover the usual cases:
  * @code
    typedef MatrixP<double, 2>               MatrixP2d;
    typedef MatrixP<double, 3>               MatrixP3d;
    typedef MatrixP<double, 4>               MatrixP4d;
    typedef MatrixP<float,  2>               MatrixP2f;
    typedef MatrixP<float,  3>               MatrixP3f;
    typedef MatrixP<float,  4>               MatrixP4f;
    typedef MatrixP<std::complex<double>, 2> MatrixP2cd;
    typedef MatrixP<std::complex<double>, 3> MatrixP3cd;
    typedef MatrixP<std::complex<double>, 4> MatrixP4cd;
    typedef MatrixP<std::complex<float>,  2> MatrixP2cf;
    typedef MatrixP<std::complex<float>,  3> MatrixP3cf;
    typedef MatrixP<std::complex<float>,  4> MatrixP4cf;
  * @endcode
  *
  * For example, in an OpenGL application, the Modelview and Projection matrices
  * can be represented as objects of MatrixP3d or MatrixP3f according to whether
  * one prefers to work over doubles or over floats.
  *
  * Another example is Qt's QMatrix class: it is equivalent to Eigen's MatrixP2d.
  *
  * Tthe meaning of the entries in a MatrixP is as follows:
  * \f[\left(\begin{array}{ccc|c} & & & \\ & L & & T \\ & & & \\ \hline 0 & \cdots & 0 & 1 \end{array}\right).\f]
  * Here,
  *  - \f$L\f$, the topleft (Size x Size) block, is called the "linear component". It is a Matrix<T,Size>.
  *  - \f$T\f$, the last column vector without the last entry, is called the "translation vector". It is a Vector<T,Size>.
  *
  * Note that the last row is supposed to always be equal to 0,...,0,1. Don't alter it unless you know what you're doing.
  * However keeping it is still useful for instance for OpenGL compatibility.
  *
  * When multiplying another Vector<T, Size> V by such a MatrixP, the linear component is first applied, and then the result
  * is translated by the translation vector. Thus, the result is \f[LV+T.\f]
  */
template< typename T, int Size >
class MatrixP
{

protected:

    /// The matrix itself.
    Matrix<T, Size+1> m_mat;

public:

    /// Returns a reference to the matrix as a Matrix<T, Size+1>
    const Matrix<T, Size+1> & matrix() const { return m_mat; }
    /// Returns a reference to the matrix as a Matrix<T, Size+1>
    Matrix<T, Size+1> & matrix() { return m_mat; }

    /// Has the same meaning as in class Matrix<T, Size+1>.
    const T * array() const { return m_mat.array(); }
    /// Has the same meaning as in class Matrix<T, Size+1>.
    T * array() { return m_mat.array(); }
    /// Has the same meaning as in class Matrix<T, Size+1>.
    const T & operator() ( int row, int col ) const { return m_mat(row,col); }
    /// Has the same meaning as in class Matrix<T, Size+1>.
    T & operator() ( int row, int col ) { return m_mat(row,col); }
    /// Has the same meaning as in class Matrix<T, Size+1>.
    const T & operator[] ( int i) const { return m_mat[i]; }
    /// Has the same meaning as in class Matrix<T, Size+1>.
    T & operator[] ( int i) { return m_mat[i]; }

    /** Sets the last row to be 0,...,0,1. */
    void resetLastRow();

    /** Sets the last column entries to 0, except for the last row which is
      * left unmodified.
      *
      * \sa setTranslationVector(), translationVector(), getTranslationVector()
      */
    void resetTranslationVector();

    /**
      * Copies into *res the Size first entries of the
      * last column of the matrix.
      *
      * \sa setTranslationVector(), translationVector(), resetTranslationVector()
      */
    void getTranslationVector( Vector<T, Size> * res ) const;

    /**
      * Copies v into the Size first entries of the
      * last column of the matrix.
      *
      * \sa getTranslationVector(), translationVector(), resetTranslationVector()
      */
    void setTranslationVector( const Vector<T, Size> & v );

    /**
      * Returns a vector whose coords are the Size first entries of the
      * last column of the matrix.
      *
      * \sa getTranslationVector(), setTranslationVector(), resetTranslationVector()
      */
    Vector<T, Size> translationVector() const
    {
        Vector<T, Size> res;
        getTranslationVector( & res );
        return res;
    }

    /** Loads into *this a MatrixP constructed from a Matrix<T, Size>, by
      * calling setLinearComponent(), and then resetLastRow() and resetTranslationVector().
      * The last row and last column are filled with 0's, except for the
      * bottom-right corner entry which is set to 1. The resulting MatrixP
      * has the same action on vectors as the original Matrix had.
      *
      * \sa setLinearComponent()
      */
    MatrixP & loadMatrix( const Matrix<T, Size> & matrix );

    /** Constructs an uninitialized MatrixP.
      */
    MatrixP() : m_mat() {}

    /** Copy constructor. */
    MatrixP( const MatrixP & other ) : m_mat( other.m_mat ) {}

    /** Constructs a MatrixP from a Matrix<T, Size+1>. Simply copies the
      * entries.
      */
    MatrixP( const Matrix<T, Size+1> & other ) : m_mat( other ) {}

    /** Constructs a MatrixP from a Matrix<T, Size> by calling loadMatrix().
      */
    MatrixP( const Matrix<T, Size> & other )
    {
        loadMatrix( other );
    }

    /** Calls Matrix::operator=(). */
    MatrixP & operator = ( const MatrixP & other )
    { m_mat = other.m_mat; return *this; }

    /** Matrix-matrix product. Calls Matrix::multiply(). This stores in *res
      * the product (*this) * other. This method is faster than operator*=
      * and operator* because it doesn't perform useless copies.
      *
      * \sa linearMultiply()
      */
    void multiply( const MatrixP & other, MatrixP * res ) const
    { m_mat.multiply( other.m_mat, &( res->m_mat ) ); }

    /** Matrix-matrix product. Calls Matrix::operator*=(). */
    MatrixP & operator *= ( const MatrixP & other )
    { m_mat *= other.m_mat; return *this; }

    /** Matrix-matrix product. Calls Matrix::operator*(). For better performance use 
      * multiply(const MatrixP &, MatrixP *) const instead.
      *
      * \sa multiply(const MatrixP &, MatrixP *) const
      */
    MatrixP operator * ( const MatrixP & other ) const
    { return MatrixP( m_mat * other.m_mat ); }

    /** Applies the homography represented by *this to \a vector,
      * and stores the result in *res. Thus the linear component of
      * *this is applied to \a vector, and then \a vector is translated
      * by the translation coefficients of *this. This method is faster than
      * operator* because it doesn't perform useless copies.*/
    void multiply( const Vector<T, Size> & vector,
                   Vector<T, Size> * res ) const;

    /** Returns the product of \a vector by *this, as computed by
      * multiply( const Vector<T, Size> &, Vector<T, Size> * ).
      *
      * \sa multiply(const Vector<T, Size> &, Vector<T, Size> *) const
      */
    Vector<T, Size> operator * ( const Vector<T, Size> & vector ) const
    {
        Vector<T, Size> res;
        multiply( vector, & res );
        return res;
    }

    /** Calls Matrix::multiply(). Thus, for vectors of size \a Size+1,
      * the multiplication is done as if *this were an ordinary matrix. This method is faster than
      * operator* because it doesn't perform useless copies.*/
    void multiply( const Vector<T, Size+1> & vector,
                   Vector<T, Size+1> * res ) const
    { m_mat.multiply( vector, res ); }

    /** Calls Matrix::operator*(). Thus, for vectors of size \a Size+1,
      * the multiplication is done as if *this were an ordinary matrix.
      *
      * \sa multiply(const Vector<T, Size+1> &, Vector<T, Size+1> *) const
      */
    Vector<T, Size+1> operator * ( const Vector<T, Size+1> & vector ) const
    { return m_mat * vector; }

    /** Applies the linear component of the homography represented by *this
      * to \a vector,
      * and stores the result in *res. This is the same thing as
      * multiply( const Vector<T,Size> &, Vector<T, Size> *) const,
      * except that the translation coefficients of *this are ignored.
      * In other words the last column of *this is ignored and the
      * computation is done as if it were 0,...0,1.
      *
      * \sa multiply(const MatrixP &, MatrixP *) const
      */
    void linearMultiply( const Vector<T, Size> & vector,
                   Vector<T, Size> * res ) const;

    /**
      * Stores into *res the linear component, i.e. the (Size x Size) topleft block.
      *
      * \sa setLinearComponent(), linearComponent()
      */
    void getLinearComponent( Matrix<T, Size> * res ) const;

    /**
      * Sets the linear component of *this, i.e. the (Size x Size) topleft block.
      * The last row and column are unaffected
      *
      * \sa loadMatrix(), getLinearComponent(), linearComponent()
      */
    void setLinearComponent( const Matrix<T, Size> & matrix );

    /**
      * Returns the linear component, i.e. the (Size x Size) topleft block.
      *
      *  \sa setLinearComponent(), getLinearComponent()
      */
    Matrix<T, Size> linearComponent() const
    {
        Matrix<T, Size> res;
        getLinearComponent( & res );
        return res;
    }

    /** Calls Matrix::loadZero(). */
    MatrixP & loadZero() { m_mat.loadZero(); return *this;}

    /** Calls Matrix::loadIdentity(). */
    MatrixP & loadIdentity() { m_mat.loadIdentity(); return *this;}

    /** Sets *this to be a translation matrix, with translation vector given
      * by \a v.
      *
      * \sa translate(), pretranslate(), setTranslationVector()
      */
    MatrixP & loadTranslation( const Vector<T, Size> & v );

    /** Multiplies *this on the right by the translation matrix
      * with translation vector given by \a v.
      *
      * \sa pretranslate(), loadTranslation(), setTranslationVector()
      */
    MatrixP & translate( const Vector<T, Size> & v );

    /** Multiplies *this on the left by the translation matrix
      * with translation vector given by \a v.
      *
      * \sa translate(), loadTranslation(), setTranslationVector()
      */
    MatrixP & pretranslate( const Vector<T, Size> & v );

    /** Sets *this to be the scaling matrix with given homogeneous
      * scaling coefficient.
      *
      * \sa loadScaling(const Vector<T,Size>&), scale(const T &), prescale(const T &)
      */
    MatrixP & loadScaling( const T & coeff )
    {
        Matrix<T, Size> m;
        return loadMatrix( m.loadScaling( coeff ) );
    }

    /** Multiplies *this on the right by the scaling matrix
      * with given homogeneous scaling coefficient.
      *
      * \sa scale(const Vector<T,Size>&), prescale(const T &), loadScaling(const T &) */
    MatrixP & scale( const T & coeff )
    {
        MatrixP<T, Size> m;
        return *this *= m.loadScaling( coeff );
    }

    /** Multiplies *this on the left by the scaling matrix
      * with given homogeneous scaling coefficient.
      *
      * \sa prescale(const Vector<T,Size>&), scale(const T &), loadScaling(const T &)
      */
    MatrixP & prescale( const T & coeff )
    {
        MatrixP<T, Size> m;
        return( *this = m.loadScaling( coeff ) * (*this) );
    }

    /** Sets *this to be the scaling matrix with given vector of
      * scaling coefficients.
      *
      * \sa loadScaling(const T&), scale(const Vector<T, Size> &), prescale(const Vector<T, Size> &)
      */
    MatrixP & loadScaling( const Vector<T, Size> & coeffs )
    {
        Matrix<T, Size> m;
        return loadMatrix( m.loadScaling( coeffs ) );
    }

    /** Multiplies *this on the right by the scaling matrix
      * with given vector of scaling coefficients.
      *
      * \sa scale(const T&), prescale(const Vector<T, Size> &), loadScaling(const Vector<T, Size> &)
      */
    MatrixP & scale( const Vector<T, Size> & coeffs )
    {
        MatrixP<T, Size> m;
        return *this *= m.loadScaling( coeffs );
    }

    /** Multiplies *this on the left by the scaling matrix
      * with given vector of scaling coefficients.
      *
      * \sa prescale( const T&), scale(const Vector<T, Size> &), loadScaling(const Vector<T, Size> &)
      */
    MatrixP & prescale( const Vector<T, Size> & coeffs )
    {
        MatrixP<T, Size> m;
        return( *this = m.loadScaling( coeffs ) * (*this) );
    }

    /** Sets *this to be the rotation matrix of given angle in radians.
      * See Matrix::loadRotation2(). The template parameter Size must equal 2.
      *
      * \sa rotate2(), prerotate2()
      */
    MatrixP & loadRotation2( const T & angle )
    {
        Matrix<T, Size> m;
        return loadMatrix( m.loadRotation2( angle ) );
    }

    /** Multiplies *this on the right by the rotation matrix of given angle
      * in radians.
      * See Matrix::loadRotation2(). The template parameter Size must equal 2.
      *
      * \sa prerotate2(), loadRotation2()
      */
    MatrixP & rotate2( const T & angle )
    {
        MatrixP<T, Size> m;
        return *this *= m.loadRotation2( angle );
    }

    /** Multiplies *this on the left by the rotation matrix of given angle
      * in radians.
      * See Matrix::loadRotation2(). The template parameter Size must equal 2.
      *
      * \sa rotate2(), loadRotation2()
      */
    MatrixP & prerotate2( const T & angle )
    {
        MatrixP<T, Size> m;
        return( *this = m.loadRotation2( angle ) * (*this) );
    }

    /** Sets *this to be the rotation matrix of given angle in radians
      * around given axis vector. See Matrix::loadRotation3(). The
      * template parameter Size must equal 3.
      *
      * \sa rotate3(), prerotate3()
      */
    MatrixP & loadRotation3( const T & angle, const Vector<T, Size> & axis )
    {
        Matrix<T, Size> m;
        return loadMatrix( m.loadRotation3( angle, axis ) );
    }

    /** Multiplies *this on the right by the rotation matrix of given angle
      * in radians around given axis vector. See Matrix::loadRotation3(). The
      * template parameter Size must equal 3.
      *
      * \sa prerotate3(), loadRotation3()
      */
    MatrixP & rotate3( const T & angle, const Vector<T, Size> & axis )
    {
        MatrixP<T, Size> m;
        return *this *= m.loadRotation3( angle, axis );
    }

    /** Multiplies *this on the left by the rotation matrix of given angle
      * in radians around given axis vector. See Matrix::loadRotation3(). The
      * template parameter Size must equal 3.
      *
      * \sa rotate3(), loadRotation3()
      */
    MatrixP & prerotate3( const T & angle, const Vector<T, Size> & axis )
    {
        MatrixP<T, Size> m;
        return( *this = m.loadRotation3( angle, axis ) * (*this) );
    }
};

template< typename T, int Size >
void MatrixP<T, Size>::getLinearComponent( Matrix<T, Size> * res ) const
{
    if( Size > 3 )
        for( int i = 0; i < Size; i++ )
            for( int j = 0; j < Size; j++ )
                (*res)( j, i ) = m_mat( j, i );
    else
    {
        (*res)(0, 0) = m_mat(0, 0);
        if( Size >= 2 )
        {
            (*res)(1, 0) = m_mat(1, 0);
            (*res)(0, 1) = m_mat(0, 1);
            (*res)(1, 1) = m_mat(1, 1);
        }
        if( Size == 3 )
        {
            (*res)(2, 0) = m_mat(2, 0);
            (*res)(2, 1) = m_mat(2, 1);
            (*res)(0, 2) = m_mat(0, 2);
            (*res)(1, 2) = m_mat(1, 2);
            (*res)(2, 2) = m_mat(2, 2);
        }
    }
}

template< typename T, int Size >
void MatrixP<T, Size>::setLinearComponent( const Matrix<T, Size> & matrix )
{
    if( Size > 3 )
        for( int i = 0; i < Size; i++ )
            for( int j = 0; j < Size; j++ )
                m_mat( j, i ) = matrix( j, i );
    else
    {
        m_mat(0, 0) = matrix(0, 0);
        if( Size >= 2 )
        {
            m_mat(1, 0) = matrix(1, 0);
            m_mat(0, 1) = matrix(0, 1);
            m_mat(1, 1) = matrix(1, 1);
        }
        if( Size == 3 )
        {
            m_mat(2, 0) = matrix(2, 0);
            m_mat(2, 1) = matrix(2, 1);
            m_mat(0, 2) = matrix(0, 2);
            m_mat(1, 2) = matrix(1, 2);
            m_mat(2, 2) = matrix(2, 2);
        }
    }
}

template< typename T, int Size >
void MatrixP<T, Size>::resetLastRow()
{
    for( int i = 0; i < Size; i++ )
        m_mat( Size, i ) = static_cast<T>(0);
    m_mat( Size, Size ) = static_cast<T>(1);
}

template< typename T, int Size >
void MatrixP<T, Size>::resetTranslationVector()
{
    for( int i = 0; i < Size; i++ )
        m_mat( i, Size ) = static_cast<T>(0);
}

template< typename T, int Size >
void MatrixP<T, Size>::setTranslationVector( const Vector<T, Size> & v )
{
    for( int i = 0; i < Size; i++ )
        m_mat( i, Size ) = v(i);
}

template< typename T, int Size >
void MatrixP<T, Size>::getTranslationVector( Vector<T, Size> * res ) const
{
    for( int i = 0; i < Size; i++ )
        (*res)(i) = m_mat( i, Size );
}

template< typename T, int Size >
MatrixP<T, Size> &
MatrixP<T, Size>::loadMatrix( const Matrix<T, Size> & matrix )
{
    setLinearComponent(matrix);
    resetTranslationVector();
    resetLastRow();
    return *this;
}

template< typename T, int Size >
void MatrixP<T, Size>::multiply( const Vector<T, Size> & vector,
               Vector<T, Size> * res ) const
{
    if( Size > 3 )
        for( int i = 0; i < Size; i++ )
        {
            (*res)[i] = static_cast<T>(0);
            for( int j = 0; j < Size; j++ )
                (*res)[i] += m_mat(i, j) * vector(j);
            (*res)[i] += m_mat( i, Size );
        }
    else
    {
        if( Size == 1 )
            (*res)(0) = m_mat(0, 0) * vector(0) + m_mat(0, 1);
        else if( Size == 2 )
        {
            (*res)(0) = m_mat(0, 0) * vector(0)
                      + m_mat(0, 1) * vector(1)
                      + m_mat(0, 2);
            (*res)(1) = m_mat(1, 0) * vector(0)
                      + m_mat(1, 1) * vector(1)
                      + m_mat(1, 2);
        }
        else // Size == 3
        {
            (*res)(0) = m_mat(0, 0) * vector(0)
                      + m_mat(0, 1) * vector(1)
                      + m_mat(0, 2) * vector(2)
                      + m_mat(0, 3);
            (*res)(1) = m_mat(1, 0) * vector(0)
                      + m_mat(1, 1) * vector(1)
                      + m_mat(1, 2) * vector(2)
                      + m_mat(1, 3);
            (*res)(2) = m_mat(2, 0) * vector(0)
                      + m_mat(2, 1) * vector(1)
                      + m_mat(2, 2) * vector(2)
                      + m_mat(2, 3);
        }
    }
}

template< typename T, int Size >
void MatrixP<T, Size>::linearMultiply( const Vector<T, Size> & vector,
               Vector<T, Size> * res ) const
{
    if( Size > 3 )
        for( int i = 0; i < Size; i++ )
        {
            (*res)[i] = static_cast<T>(0);
            for( int j = 0; j < Size; j++ )
                (*res)[i] += m_mat(i, j) * vector(j);
        }
    else
    {
        if( Size == 1 )
            (*res)(0) = m_mat(0, 0) * vector(0);
        else if( Size == 2 )
        {
            (*res)(0) = m_mat(0, 0) * vector(0)
                      + m_mat(0, 1) * vector(1);
            (*res)(1) = m_mat(1, 0) * vector(0)
                      + m_mat(1, 1) * vector(1);
        }
        else // Size == 3
        {
            (*res)(0) = m_mat(0, 0) * vector(0)
                      + m_mat(0, 1) * vector(1)
                      + m_mat(0, 2) * vector(2);
            (*res)(1) = m_mat(1, 0) * vector(0)
                      + m_mat(1, 1) * vector(1)
                      + m_mat(1, 2) * vector(2);
            (*res)(2) = m_mat(2, 0) * vector(0)
                      + m_mat(2, 1) * vector(1)
                      + m_mat(2, 2) * vector(2);
        }
    }
}

template< typename T, int Size >
MatrixP<T, Size> &
MatrixP<T, Size>::loadTranslation( const Vector<T, Size> & v )
{
    resetLastRow();
    if( Size > 3 )
    {
        for( int i = 0; i < Size; i++ )
        {
            m_mat( i, i ) = static_cast<T>(1);
            m_mat( i, Size ) = v(i);
            for( int j = 0; j < i; j++ )
            {
                m_mat( j, i ) = static_cast<T>(0);
                m_mat( i, j ) = static_cast<T>(0);
            }
        }
    }
    else
    {
        if( Size == 1 )
        {
            m_mat( 0, 0 ) = static_cast<T>(1);
            m_mat( 0, 1 ) = v(0);
        }
        else if( Size == 2 )
        {
            m_mat( 0, 0 ) = static_cast<T>(1);
            m_mat( 1, 1 ) = static_cast<T>(1);
            m_mat( 1, 0 ) = static_cast<T>(0);
            m_mat( 0, 1 ) = static_cast<T>(0);
            m_mat( 0, 2 ) = v(0);
            m_mat( 1, 2 ) = v(1);
        }
        else // Size == 3
        {
            m_mat( 0, 0 ) = static_cast<T>(1);
            m_mat( 1, 1 ) = static_cast<T>(1);
            m_mat( 2, 2 ) = static_cast<T>(1);
            m_mat( 1, 0 ) = static_cast<T>(0);
            m_mat( 2, 0 ) = static_cast<T>(0);
            m_mat( 0, 1 ) = static_cast<T>(0);
            m_mat( 2, 1 ) = static_cast<T>(0);
            m_mat( 0, 2 ) = static_cast<T>(0);
            m_mat( 1, 2 ) = static_cast<T>(0);
            m_mat( 0, 3 ) = v(0);
            m_mat( 1, 3 ) = v(1);
            m_mat( 2, 3 ) = v(2);
        }
    }
    return *this;
}

template< typename T, int Size >
MatrixP<T, Size> &
MatrixP<T, Size>::translate( const Vector<T, Size> & v )
{
    if( Size > 3 )
    {
        MatrixP<T, Size> m;
        return *this *= m.loadTranslation(v);
    }
    else
    {
        if( Size == 1 )
            m_mat( 0, 1 ) += m_mat( 0, 0 ) * v(0);
        else if( Size == 2 )
        {
            m_mat( 0, 2 ) += m_mat( 0, 0 ) * v(0)
                           + m_mat( 0, 1 ) * v(1);
            m_mat( 1, 2 ) += m_mat( 1, 0 ) * v(0)
                           + m_mat( 1, 1 ) * v(1);
        }
        else // Size == 3
        {
            m_mat( 0, 3 ) += m_mat( 0, 0 ) * v(0)
                           + m_mat( 0, 1 ) * v(1)
                           + m_mat( 0, 2 ) * v(2);
            m_mat( 1, 3 ) += m_mat( 1, 0 ) * v(0)
                           + m_mat( 1, 1 ) * v(1)
                           + m_mat( 1, 2 ) * v(2);
            m_mat( 2, 3 ) += m_mat( 2, 0 ) * v(0)
                           + m_mat( 2, 1 ) * v(1)
                           + m_mat( 2, 2 ) * v(2);
        }
    }
    return *this;
}

template< typename T, int Size >
MatrixP<T, Size> &
MatrixP<T, Size>::pretranslate( const Vector<T, Size> & v )
{
    if( Size > 3 )
    {
        MatrixP<T, Size> m;
        return( *this = m.loadTranslation(v) * (*this) );
    }
    else
    {
        if( Size == 1 )
            m_mat( 0, 1 ) += v(0);
        else if( Size == 2 )
        {
            m_mat( 0, 2 ) += v(0);
            m_mat( 1, 2 ) += v(1);
        }
        else // Size == 3
        {
            m_mat( 0, 3 ) += v(0);
            m_mat( 1, 3 ) += v(1);
            m_mat( 2, 3 ) += v(2);
        }
    }
    return *this;
}

EIGEN_MAKE_FIXEDSIZE_TYPEDEFS(MatrixP)

} // namespace Eigen

#endif // EIGEN_MATRIXP_H
