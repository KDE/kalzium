// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2006-2007 Benoit Jacob <jacob@math.jussieu.fr>
// Some portions Copyright (C) 2007 Franz Keferboeck
//                                  <franz.keferboeck@gmail.com>
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

/** \file matrixbase.h
  * \brief Internal file
  */

#ifndef EIGEN_MATRIXBASE_H
#define EIGEN_MATRIXBASE_H

#include "util.h"
#undef _T

namespace Eigen
{

/** \ingroup internalbases
  *
  * \ingroup matrices
  *
  * \brief internal base class
  *
  * This class template is only internally used in Eigen.
  * This provides the base that the Matrix and MatrixX
  * class templates inherit.
  *
  * Note that one template parameter is Derived: this is a
  * C++ trick knows as Curiously Recursive Template Pattern.
  * Here, it allows us to implement in MatrixBase the code
  * of both Matrix and MatrixX, which are very different
  * (Matrix has the size as a template argument,
  * while MatrixX stores it as member data, thus storing the
  * array itself on the heap).
  */
template< typename T,
          typename Derived,
          typename VectorType,
          typename LUDecompositionType >
class MatrixBase
{
public:

    typedef VectorType VecType;
    typedef T ScalType;

private:

    /** \internal
      * Helper method, computes *this + other and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void addition_helper
    ( const Derived & other, Derived * res ) const;
    
    /** \internal
      * Helper method, computes *this - other and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void substraction_helper
    ( const Derived & other, Derived * res ) const;
    
    /** \internal
      * Helper method, computes *this * factor and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void scalar_multiplication_helper
    ( const T & factor, Derived * res ) const;

    /** \internal
      * Helper method, computes *this / factor and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void scalar_division_helper
    ( const T & factor, Derived * res ) const
    {
        scalar_multiplication_helper( static_cast<const T>(1) / factor, res );
    }

    /** \internal
      * Helper method, computes *this * other and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void matrix_multiplication_helper
    ( const Derived & other, Derived * res ) const;

    /** \internal
      * Helper method, computes *this * vector and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void vector_multiplication_helper
    ( const VectorType & vector, VectorType * res ) const;

    /** \internal
      * Helper method, computes vector * *this and stores the result in *res.
      * Doesn't attempt to resize *res. Doesn't check that the sizes match.
      */
    void vector_leftmultiplication_helper
    ( const VectorType & vector, VectorType * res ) const;

    template< typename _T, typename _Derived,
              typename _VectorType, typename _LUDecompositionType >
    friend _VectorType operator *
    ( const _VectorType & v,
      const MatrixBase<_T, _Derived, _VectorType, _LUDecompositionType> & m );

    template< typename _T, typename _Derived,
              typename _VectorType, typename _LUDecompositionType >
    friend _VectorType & operator *=
    ( _VectorType & v,
      const MatrixBase<_T, _Derived, _VectorType, _LUDecompositionType> & m );

public:

    /**
      * Computes the matrix product *this * other and stores the
      * result into *res.
      *
      * For dynamic-size matrices, this method resizes *res if necessary.
      * For fixed-size matrices, it is required that *res already has the
      * right size, that is: res->size() == this->size().
      *
      * This method is faster than operator* and operator*=.
      *
      * Note for dynamic-size matrices:
      * For optimal performance, the matrix res should be
      * initialized with the correct size before calling this method,
      * otherwise it will have to be resized, which is costly.
      *
      * Note that for the product to make sense, it is required
      * that this->size() == other.size().
      *
      * \sa operator*(const Derived &) const,
      *     operator*=(const Derived &)
      */
    void multiply( const Derived & other, Derived * res ) const
    {
        assert( this->size() == other.size() );
        res->resize( this->size() );
        matrix_multiplication_helper( other, res );
    }

    /** Entry-wise multiplication of matrices. This is NOT the standard matrix-matrix
      * product.
      *
      * *res becomes the matrix such that (*res)(i,j) = (*this)(i,j) * other(i,j).
      *
      * \sa multiply(const Derived &, Derived *) const
      */
    void multiplyEntries( const Derived & other, Derived * res ) const
    {
        assert( this->size() == other.size() );
        res->resize( this->size() );
        for( int i = 0; i < size() * size(); i++ )
            (*res)[i] = (*this)[i] * other[i];
    }


    /**
      * Returns the matrix product *this * other.
      *
      * Note that for the product to make sense, it is required
      * that this->size() == other.size().
      *
      * \return_by_value \perf_use
      * multiply(const Derived &, Derived *) const.
      *
      * \sa multiply(const Derived &, Derived *) const,
      *     operator*=(const Derived &)
      */
    Derived operator * ( const Derived & other ) const
    {
        assert( this->size() == other.size() );
        Derived res( this->size() );
        matrix_multiplication_helper( other, & res );
        return res;
    }

    /**
      * Matrix multiplication on the right: does *this = (*this) * other.
      *
      * This only makes sense if this->size() == other.size().
      *
      * \sa multiply(const Derived &, Derived *) const,
      *     operator*(const Derived &) const
      */
    Derived & operator *=( const Derived & other )
    {
        assert( this->size() == other.size() );
        Derived res( this->size() );
        matrix_multiplication_helper( other, & res );
        return( *this = res );
    }

    /**
      * @returns true if the matrix has dynamic size (i.e. is an
      * object of class MatrixX), false if the matrix has fixed size
      * (i.e. is an object of class Matrix).
      *
      * \sa size(), resize()
      */
    static bool hasDynamicSize()
    {
        return Derived::_hasDynamicSize();
    }

    /**
      * @returns the size (number of rows, or equivalently number of
      * columns) of the matrix.
      *
      * \sa hasDynamicSize(), resize()
      */
    int size() const
    {
        return static_cast<const Derived*>(this)->_size();
    }

    /**
      * Resizes the matrix. That is only possible if the matrix
      * has dynamic size, i.e. is an object of class MatrixX.
      *
      * Resizing a fixed-size matrix is not possible, and attempting
      * to do so will only generate a debug message (unless the new size
      * equals the old one).
      *
      * The matrix entries are not kept, they
      * are left with undefined values after resizing.
      *
      * \sa hasDynamicSize(), size()
      */
    void resize( int newsize )
    {
        static_cast<Derived*>(this)->_resize( newsize );
    }

    /**
      * @returns the array of the matrix, as constant.
      *
      * \sa operator()(int,int) const, operator[](int) const
      */
    const T * array() const
    {
        return static_cast<const Derived*>(this)->m_array;
    }

    /**
      * @returns the array of the matrix, as non-constant.
      *
      * \sa operator()(int,int), operator[](int)
      */
    T * array()
    {
        return static_cast<Derived*>(this)->m_array;
    }

    /**
      * @returns a constant reference to the entry of the matrix at
      * given row and column.
      *
      * \sa array() const, operator[](int) const
      */
    const T & operator () ( int row, int col ) const
    {
        assert( row >= 0 && col >= 0 && row < size() && col < size() );
        return array() [ row + col * size() ];
    }

    /**
      * @returns a non-constant reference to the entry of the matrix at
      * given row and column.
      *
      * \sa array(int), operator[](int,int)
      */
    T & operator () ( int row, int col )
    {
        assert( row >= 0 && col >= 0 && row < size() && col < size() );
        return array() [ row + col * size() ];
    }

    /**
      * @returns a constant reference to the i-th entry of the array
      * of the matrix (which stores the matrix entries in column-major order).
      *
      * \sa array() const, operator()(int,int) const
      */
    const T & operator [] ( int i ) const
    {
        assert( i >= 0 && i < size() * size() );
        return array() [i];
    }

    /**
      * @returns a non-constant reference to the i-th entry of the array
      * of the matrix (which stores the matrix entries in column-major order).
      *
      * \sa array(), operator()(int,int)
      */
    T & operator [] ( int i )
    {
        assert( i >= 0 && i < size() * size() );
        return array() [i];
    }

    /**
      * Copies other into *this. If *this has dynamic size,
      * it will get resized if necessary. If *this has static size,
      * it is required that other has the same size.
      *
      * \sa readArray(), readRows()
      */
    Derived & operator = ( const Derived & other );

    /**
      * Stores *this + other into *this (entry-wise addition).
      *
      * *this and other must have the same size.
      *
      * \sa operator+()
      */
    Derived & operator += ( const Derived & other )
    {
        assert( other.size() == this->size() );
        addition_helper( other, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Stores *this - other into *this (entry-wise substraction).
      *
      * *this and other must have the same size.
      *
      * \sa operator-(const Derived &) const
      */
    Derived & operator -= ( const Derived & other )
    {
        assert( other.size() == this->size() );
        substraction_helper( other, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Stores *this * factor into *this (multiplication of each entry).
      *
      * \sa operator*(const T&) const
      */
    Derived & operator *= ( const T & factor )
    {
        scalar_multiplication_helper( factor, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Stores *this / factor into *this (division of each entry).
      *
      * \sa operator*=(const T&)
      */
    Derived & operator /= ( const T & factor )
    {
        scalar_division_helper( factor, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Reads the entries of *this from an array. The number of entries
      * read from the array is equal to size()*size().
      *
      * \sa operator=(), readRows()
      */
    void readArray( const T * src );

    /**
      * @returns the array data of a column of the matrix, as constant
      *
      * \sa columnPtr(int), column(), getColumn(int,T*) const,
      *     getColumn(int, VectorType *) const
      */
    const T *columnPtr( int col ) const
    { return array() + col * size(); }

    /**
      * Returns the array data of a column of the matrix, as non-constant
      *
      * \sa columnPtr(int) const, column(), setColumn(int, const T*),
      *     setColumn(int, const VectorType &)
      */
    T *columnPtr( int col )
    { return array() + col * size(); }

    /** Copies a row of the matrix into the array \a ret
      *
      * \sa getRow(int, VectorType *) const, row()
      */
    void getRow( int row, T * ret ) const;

    /** Copies a column of the matrix into the array \a ret
      *
      * \sa getColumn(int, VectorType *) const, column(), columnPtr(int) const
      */
    void getColumn( int column, T * ret ) const;

    /** Copies a row of the matrix into the vector \a ret
      *
      * \sa getRow(int, T *) const, row()
      */
    void getRow( int row, VectorType * ret ) const
    {
        ret->resize( size() );
        getRow( row, ret->array() );
    }

    /** Copies a column of the matrix into the vector \a ret
      *
      * \sa getColumn(int, T *) const, column(), columnPtr(int) const
      */
    void getColumn( int column, VectorType * ret ) const
    {
        ret->resize( size() );
        getColumn( column, ret->array() );
    }

    /** Copies the array \a src into a row of the matrix
      *
      * \sa setRow( int, const VectorType &)
      */
    void setRow( int row, const T * src );

    /** Copies the array \a src into a column of the matrix
      *
      * \sa setColumn( int, const VectorType &)
      */
    void setColumn( int column, const T * src );

    /** Copies the vector \a src into a row of the matrix
      *
      * \sa setRow( int, const T *)
      */
    void setRow( int row, const VectorType & src )
    {
        setRow( row, src.array() );
    }

    /** Copies the vector \a src into a column of the matrix
      *
      * \sa setColumn( int, const T *)
      */
    void setColumn( int column, const VectorType & src )
    {
        setColumn( column, src.array() );
    }

    /**
      * Returns a column of the matrix.
      *
      * \return_by_value \perf_use another method.
      *
      * \sa columnPtr(int) const, getColumn(int,T*) const,
      *     getColumn(int, VectorType *) const
      */
    VectorType column( int col ) const
    {
        VectorType res( size() );
        getColumn( col, res.array() );
        return res;
    }

    /**
      * Returns a row of the matrix.
      *
      * \return_by_value \perf_use another method.
      *
      * \sa getRow(int,T*) const,
      *     getRow(int, VectorType *) const
      */
    VectorType row( int row ) const
    {
        VectorType res( size() );
        getRow( row, res.array() );
        return res;
    }

    /**
      * Reads the rows of the matrix from a row-dominant array.
      * For instance, C/C++ two-dimensional arrays are stored
      * in row-dominant order.
      *
      * \perf_use readArray()
      *
      * \sa operator=(), readArray()
      */
    void readRows( const T * rows );

    /**
      * Sets *this to be \a coeff times the identity matrix.
      * In other words, the diagonal entries are set to \a coeff
      * and the non-diagonal entries are set to zero.
      *
      * \sa loadScaling(const T&), loadDiagonal(const VectorType &)
      */
    Derived & loadDiagonal( const T & coeff );

    /**
      * Other name for loadDiagonal( const T & coeff ).
      *
      * \sa loadDiagonal(const T&), loadScaling(const VectorType &)
      */
    Derived & loadScaling( const T & coeff )
    {
        return loadDiagonal( coeff );
    }

    /**
      * Sets *this to be the diagonal matrix with diagonal entries
      * given by the coords of the vector \a coeffs. If *this doesn't
      * have the same size as \a coeffs, it gets resized (if it has
      * dynamic size).
      *
      * \sa loadScaling(const VectorType &), loadDiagonal(const T &)
      */
    Derived & loadDiagonal( const VectorType & coeffs );

    /**
      * Other name for loadDiagonal( const VectorType & coeff ).
      *
      * \sa loadDiagonal(const VectorType &), loadScaling(const T&)
      */
    Derived & loadScaling( const VectorType & coeffs )
    {
        return loadDiagonal( coeffs );
    }

    /** Multiplies *this on the right by the scaling matrix
      * with given vector of scaling coefficients.
      *
      * \sa loadScaling(const VectorType &), prescale(const VectorType &)
      */
    Derived & scale( const VectorType & coeffs )
    {
        Derived m( size() );
        return *this *= m.loadScaling( coeffs );
    }

    /** Multiplies *this on the left by the scaling matrix
      * with given vector of scaling coefficients.
      *
      * \sa loadScaling(const VectorType &), scale(const VectorType &)
      */
    Derived & prescale( const VectorType & coeffs )
    {
        Derived m( size() );
        return( *this = m.loadScaling( coeffs )
                      * (*static_cast<Derived*>(this) ) );
    }

    /**
      * Sets *this to be the identity matrix.
      *
      * \sa loadZero(), loadRandom()
      */
    Derived & loadIdentity()
    {
        return loadDiagonal( static_cast<T>(1) );
    }

    /**
      * Sets *this to be the zero matrix.
      *
      * \sa loadIdentity(), loadRandom()
      */
    Derived & loadZero();

    /**
     * Sets *this to be a 2-dimensional rotation of given angle.
     * @param angle the angle expressed in radians, counter-clockwise
     *
     * \sa rotate2(), prerotate2(), loadRotation3()
     */
    Derived & loadRotation2( const T & angle );

    /** Multiplies *this on the right by the rotation matrix of given angle
      * in radians. The template parameter Size must equal 2.
      *
      * \sa loadRotation2(), prerotate2()
      */
    Derived & rotate2( const T & angle )
    {
        Derived m( size() );
        return *this *= m.loadRotation2( angle );
    }

    /** Multiplies *this on the left by the rotation matrix of given angle
      * in radians. The template parameter Size must equal 2.
      *
      * \sa loadRotation2(), rotate2()
      */
    Derived & prerotate2( const T & angle )
    {
        Derived m( size() );
        return( *this = m.loadRotation2( angle )
                      * (*static_cast<Derived*>(this)) );
    }

    /**
     * Sets *this to be a 3-dimensional rotation of given angle in radians.
     * @param angle the angle expressed in radians, counter-clockwise if the
     *              axis vector it oriented towards the observer.
     * @param axis the axis vector around which to rotate. Must be a unit
     *             vector, i.e. it is required that axis.norm() == 1.
     * \note if axis.norm() is different from 1, the result is undefined, and
     *       certainly won't be a rotation matrix.
     *
     * \sa rotate3(), prerotate3(), loadRotation2()
     */
    Derived & loadRotation3( const T & angle, const VectorType & axis );

    /** Multiplies *this on the right by the rotation matrix of given angle
      * in radians around given axis vector. Only applicable to matrices of
      * size 3.
      *
      * \sa prerotate3(), loadRotation3()
      */
    Derived & rotate3( const T & angle, const VectorType & axis )
    {
        Derived m( size() );
        return *this *= m.loadRotation3( angle, axis );
    }

    /** Multiplies *this on the left by the rotation matrix of given angle
      * in radians around given axis vector. Only applicable to matrices of
      * size 3.
      *
      * \sa rotate3(), loadRotation3()
      */
    Derived & prerotate3( const T & angle, const VectorType & axis )
    {
        Derived m( size() );
        return( *this = m.loadRotation3( angle, axis )
                      * (*static_cast<Derived*>(this)) );
    }

    /**
      * Returns *this + other (entry-wise addition).
      * The matrices *this and other must have
      * the same size.
      *
      * \return_by_value
      *
      * \sa operator+=()
      */
    Derived operator + ( const Derived & other ) const
    {
        assert( other.size() == size() );
        Derived res( size() );
        addition_helper( other, & res );
        return res;
    }

    /**
      * Returns *this - other (entry-wise substraction).
      * The matrices *this and other must have
      * the same size.
      *
      * \return_by_value
      *
      * \sa operator-=(), operator-(void) const
      */
    Derived operator - ( const Derived & other ) const
    {
        assert( other.size() == size() );
        Derived res( size() );
        substraction_helper( other, & res );
        return res;
    }

    /**
      * Returns (-(*this)).
      *
      * \return_by_value
      *
      * \sa replaceWithOpposite(), operator-(const Derived &) const
      */
    Derived operator - () const
    {
        Derived res( size() );
        for( int i = 0; i < size() * size(); i++ )
            res[i] = -( (*this)[i] );
        return res;
    }

    /** Replaces *this with (-(*this)).
      *
      * \sa operator-(void) const
      */
    void replaceWithOpposite()
    {
        for( int i = 0; i < size() * size(); i++ )
            (*this)[i] = -( (*this)[i] );
    }

    /**
      * Returns true if *this and other are approximately equal.
      *
      * The optional parameter precision allows to control the number
      * of significant digits of precision. For instance, setting precision
      * to 1e-5 results in a precision of 5 decimal digits.
      *
      * This test is for nonzero matrices. If either of the two matrices
      * being compared is zero, then it returns true if, and only if the other
      * one is also zero -- which is not what one typically wants.
      *
      * To compare a matrix with the zero matrix, i.e. to check whether a
      * matrix is approximately zero, use isZero() instead.
      *
      * \sa operator==(), operator!=(), isZero()
      */
    bool isApprox( const Derived & other, const T & precision = Util::epsilon<T>() ) const
    {
        VectorType v1(size()), v2(size());
        for( int i = 0; i < size(); i++ )
        {
            getColumn(i, &v1);
            other.getColumn(i, &v2);
            if( ! v1.isApprox(v2, precision) ) return false;
        }
        return true;
    }

    /**
      * Returns true if all entries of *this are smaller (in absolute value)
      * than other*precision. In other words, returns true if all entries are
      * much smaller than \a other. For the meaning of \a precision, see isApprox().
      *
      * \sa isApprox(), isZero()
      */
    bool isNegligible( const T & other, const T & precision = Util::epsilon<T>() ) const
    {
        bool ret = true;
        for( int i = 0; i < size(); i++ )
            if( std::abs((*this)(i)) > std::abs(other*precision) )
                ret = false;
        return ret;
    }

    /** Tests whether *this is approximately equal to the zero matrix.
      * 
      * Equivalent to isNegligible(1). In other words, returns true if
      * all entries of *this are approximately zero, in the sense that
      * they have absolute value smaller than epsilon.
      *
      * \sa isNegligible(), isApprox()
      */
    bool isZero( const T & precision = Util::epsilon<T>() ) const
    {
        return isNegligible( static_cast<T>(1) );
    }

    /**
      * Equivalent to isApprox() with the default precision.
      *
      * \note Despite the name, this operator does a fuzzy compare!
      *       It is not equivalent to operator== on each entry.
      *
      * \sa isApprox(),operator!=(),isZero()
      */
    bool operator==( const Derived & other ) const
    {
        return isApprox(other);
    }

    /**
      * Equivalent to !isApprox() with the default precision.
      *
      * \note Despite the name, this operator does a fuzzy compare!
      *       It is not equivalent to operator!= on each entry.
      *
      * \sa isApprox(),operator==(),isZero()
      */
    bool operator!=( const Derived & other ) const
    {
        return !isApprox(other);
    }

    /**
      * Returns *this * factor (multiplication of each coord).
      *
      * \return_by_value
      *
      * \sa operator*=(const T &)
      */
    Derived operator * ( const T & factor ) const
    {
        Derived res( size() );
        scalar_multiplication_helper( factor, & res );
        return res;
    }

    /**
      * Returns *this / factor (division of each coord).
      *
      * \return_by_value
      *
      * \sa operator*(const T&) const, operator/=(const T&)
      */
    Derived operator / ( const T & factor ) const
    {
        Derived res( size() );
        scalar_division_helper( factor, & res );
        return res;
    }

    /**
      * Returns *this * vector (multiplication of vector by matrix).
      * The size of *this must equal the size of vector.
      *
      * \return_by_value \perf_use
      * multiply( const VectorType &, VectorType *) const
      *
      * \sa multiply( const VectorType &, VectorType *) const
      */
    VectorType operator * ( const VectorType & vector ) const
    {
        assert( vector.size() == size() );
        VectorType res( size() );
        vector_multiplication_helper( vector, & res );
        return res;
    }

    /**
      * Computes the product *this * vector and stores the
      * result into *res.
      *
      * For dynamic-size classes, this method resizes *res if necessary.
      * For fixed-size classes, it is required that *res already has the
      * right size, that is: res->size() == this->size().
      *
      * This method is faster than operator*.
      *
      * Note for dynamic-size classes:
      * For optimal performance, the vector *res should be
      * initialized with the correct size before calling this method,
      * otherwise it will have to be resized, which is costly.
      *
      * Note that for the product to make sense, it is required
      * that this->size() == vector.size().
      *
      * \sa operator*(const VectorType &) const,
      *     leftmultiply()
      */
    void multiply( const VectorType & vector, VectorType * res ) const
    {
        assert( this->size() == vector.size() );
        res->resize( this->size() );
        vector_multiplication_helper( vector, res );
    }

    /**
      * Computes the product vector * *this and stores the
      * result into *res. Here, \a vector is regarded
      * as a row vector.
      *
      * For dynamic-size classes, this method resizes *res if necessary.
      * For fixed-size classes, it is required that *res already has the
      * right size, that is: res->size() == this->size().
      *
      * This method is faster than operator*.
      *
      * Note for dynamic-size classes:
      * For optimal performance, the vector *res should be
      * initialized with the correct size before calling this method,
      * otherwise it will have to be resized, which is costly.
      *
      * Note that for the product to make sense, it is required
      * that this->size() == vector.size().
      *
      * \sa multiply( const VectorType &, VectorType *) const
      */
    void leftmultiply( const VectorType & vector, VectorType * res ) const
    {
        assert( this->size() == vector.size() );
        res->resize( this->size() );
        vector_leftmultiplication_helper( vector, res );
    }

    /**
      * Gram-Schmidt algorithm.
      *
      * Assuming that the \a n first columns of *this are mutually orthogonal
      * unit vectors, this method fills the remaining columns with unit vectors
      * such that all the columns are mutually orthogonal. Thus, after this
      * method has returned, the columns of *this form an orthonormal basis.
      * In other words, after this function has returned, if we let \f$A\f$ denote
      * the matrix *this, then \f[ AA^* = \mathrm{Id}, \f] which is to say
      * that \f$A\f$ is an orthogonal matrix. Note that there is an inconsistency
      * in the mathematical language: an orthogonal matrix is a matrix whose
      * columns form an orthonormal basis, not just an orthogonal basis.
      *
      * \note this method assumes that the \a n first column vectors are unit
      * vectors. Otherwise, it doesn't work.
      *
      * \sa loadOrthoBasis()
      */
    void recursiveGramSchmidt( int n );

    /**
      * This method loads \a vector into the first column of *this,
      * and fills the other columns with unit vectors such that
      * all the columns of *this together form an orthogonal basis.
      * This is an orthonormal basis if, and only if \a vector is a
      * unit vector.
      *
      * If *this has dynamic size, it gets resized to match the size of
      * \a vector. If it has fixed size, then it is required that its size
      * equals the size of \a vector.
      *
      * \note this method assumes that \a vector is nonzero.
      *
      * \sa recursiveGramSchmidt(), VectorBase::makeOrthoVector()
      */
    Derived & loadOrthoBasis( const VectorType & vector );

    /**
      * @returns the determinant of the matrix (must be a square matrix).
      *
      * If the matrix has size <= 3, the determinant is obtained by
      * direct computation. If the matrix has size >= 4, a LU
      * decomposition is computed, and the determinant is obtained from it.
      *
      * \ludecomp_remark
      *
      * \sa LUDecompositionBase::determinant()
      */
    T determinant() const;

    /**
      * @returns true if the matrix is invertible, false if it is singular.
      *
      * If the matrix haS size <= 3. If the matrix has size >= 4, a LU
      * decomposition is computed, and the invertibility is obtained from it.
      *
      * \ludecomp_remark
      *
      * \sa rank(), dimKer(), LUDecompositionBase::isInvertible(),
      *     computeInverseSafely()
      */
   bool isInvertible() const;

    /**
      * @returns the dimension of the kernel of the matrix.
      *
      * This function always performs a LU decomposition, even for small
      * matrices. \ludecomp_remark
      *
      * \sa rank(), isInvertible(), LUDecompositionBase::dimKer()
      */
    int dimKer() const
    {
        return LUDecompositionType( *static_cast<const Derived*>(this) )
                   .dimKer();
    }

    /**
      * @returns the rank of the matrix.
      *
      * This function always performs a LU decomposition, even for small
      * matrices. \ludecomp_remark
      *
      * \sa dimKer(), isInvertible(), LUDecompositionBase::rank()
      */
    int rank() const
    {
        return LUDecompositionType( *static_cast<const Derived*>(this) )
                   .rank();
    }

    /**
      * This method computes a basis of the kernel of the matrix *this.
      *
      * The \a result argument is a pointer to the matrix in which the result
      * will be stored. After calling this method, the \a dimKer() first
      * columns of \a result form a basis of the kernel of *this. If *this
      * is invertible, then dimKer()==0 and this method does nothing.
      *
      * @returns true if the matrix is non-invertible, hence has a nonzero
      * kernel; false if the matrix is invertible.
      *
      * This function always performs a LU decomposition, even for small
      * matrices. \ludecomp_remark
      *
      * \sa dimKer(), LUDecompositionBase::computeBasisKer()
      */
    bool computeBasisKer( Derived * result ) const
    {
        return LUDecompositionType( *static_cast<const Derived*>(this) )
                   .computeBasisKer(result);
    }

    /**
      * Computes an antecedent of v by *this, that is, a vector u
      * such that Au=v, where A is the matrix *this.
      * If such an antecedent doesn't exist, this method does nothing.
      *
      * @returns true if an antecedent exists, false if no antecedent exists.
      *
      * Notes:
      *
      * 1. The returned vector u is only one solution of the equation Au=v,
      * which can have more than one solution if A is non-invertible. To get
      * a basis of the whole (affine) space of solutions, use computeBasisKer().
      *
      * 2. This function always performs a LU decomposition, even for small
      * matrices. \ludecomp_remark
      *
      * \sa computeBasisKer(), LUDecompositionBase::computeSomeAntecedent()
      */
    bool computeSomeAntecedent
    ( const VectorType & v, VectorType * result ) const
    {
        return LUDecompositionType( *static_cast<const Derived*>(this) )
                   .computeSomeAntecedent(v, result);
    }

    /**
      * Computes the inverse matrix of *this, and
      * stores it in *result.
      *
      * If *this is non-invertible, the *result matrix is left with
      * an undefined value. Therefore, only call this methods on matrices
      * that you know are invertible. You can check by calling isInvertible().
      *
      * If the matrix has fixed size <= 3, the inverse is obtained by
      * direct computation. If the matrix has size >= 4 and/or has dynamic
      * size, a LU
      * decomposition is computed, and the inverse is obtained from it.
      * \ludecomp_remark
      *
      * \sa inverse(), computeInverseSafely()
      */
    void computeInverse( Derived * result ) const;

    /**
      * Safely computes the inverse matrix of *this.
      *
      * If *this is invertible, its inverse is computed and stored in
      * *result. Moreover, *invertible is set to true.
      *
      * If *this is non-invertible, the *result matrix is left unchanged
      * and *invertible is set to false.
      *
      * If the matrix has size <= 3, the inverse is obtained by
      * direct computation. If the matrix has size >= 4, a LU
      * decomposition is computed, and the inverse is obtained from it.
      * \ludecomp_remark
      *
      * \sa inverse(), computeInverse()
      */
    void computeInverseSafely( Derived * result, bool * invertible ) const;

    /**
      * This methods returns the inverse matrix of *this. If *this is
      * non-invertible, the returned value is undefined.
      *
      * This method calls computeInverse(), so the same remarks as for
      * computeInverse() apply here. \ludecomp_remark
      *
      * \return_by_value
      *
      * \sa computeInverse(), computeInverseSafely()
      */
    Derived inverse() const
    {
        Derived m( size() );
        computeInverse( &m );
        return m;
    }

    /**
      * Stores in *res_row and *res_col the position of the entry of the
      * matrix that has biggest absolute value. Skips the \a skip first rows
      * and columns (default value for \a skip is 0).
      *
      * This method is not very fast currently, because it has a nested for
      * loop that doesn't get unrolled and for which we don't provide an
      * unrolled version.
      */
    void findBiggestEntry( int * res_row, int * res_col, int skip = 0 ) const;

    /**
      * Computes the adjoint (conjugate transpose, equals transpose unless
      * T is complex numbers) of *this and stores it in *result.
      *
      * Don't try to pass \a this as \a result , this won't work. To replace
      * *this by its adjoint, use replaceWithAdjoint() instead.
      *
      * \sa adjoint(), replaceWithAdjoint()
      */
    void computeAdjoint( Derived * result ) const;

    /**
      * Returns the adjoint (conjugate transpose, equals transpose unless
      * T is complex numbers) of *this.
      *
      * \return_by_value \perf_use computeAdjoint() or replaceWithAdjoint()
      *
      * \sa computeAdjoint(), replaceWithAdjoint()
      */
    Derived adjoint() const
    {
        Derived res( size() );
        computeAdjoint( & res );
        return res;
    }

    /**
      * Replaces *this with its adjoint (conjugate transpose, equals
      * transpose unless T is complex numbers).
      *
      * \sa adjoint(), computeAdjoint()
      */
    void replaceWithAdjoint();

    /**
      * Sets all entries to random values between -1.0 and 1.0. For complex
      * numbers, both the real and imaginary parts can range from -1.0 to 1.0.
      *
      * \sa loadIdentity(), loadZero()
      */
    Derived & loadRandom()
    {
        for (int i = 0; i < size() * size(); i++)
            Util::pickRandom( (*this)[i] );
        return *static_cast<Derived *>(this);
    }
};

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived,
                VectorType, LUDecompositionType >
    ::readArray( const T * src )
{
    for (int i = 0; i < size() * size(); i++)
        (*this)[i] = src[i];
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived,
                VectorType, LUDecompositionType >
    ::addition_helper
    ( const Derived & other, Derived * res ) const
{
    for( int i = 0; i < res->size() * res->size(); i++ )
        (*res)[i] = (*this)[i] + other[i];
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived,
                VectorType, LUDecompositionType >
    ::substraction_helper
    ( const Derived & other, Derived * res ) const
{
    for( int i = 0; i < res->size() * res->size(); i++ )
        (*res)[i] = (*this)[i] - other[i];
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived,
                VectorType, LUDecompositionType >
    ::scalar_multiplication_helper
    ( const T & factor, Derived * res ) const
{
    for( int i = 0; i < res->size() * res->size(); i++ )
        (*res)[i] = (*this)[i] * factor;
}

/**
  * Returns factor * v (multiplication of each entry of v by factor).
  *
  * \return_by_value \perf_use MatrixBase::operator*=( const T & )
  */
template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
Derived operator *
( const T & factor,
  const MatrixBase<T, Derived, VectorType, LUDecompositionType> &
        v )
{
    return v * factor;
}

/**
  * Multiplication of a vector by a matrix
  * on the right: returns v * m. Here, the vector v is regarded
  * as a row vector.
  *
  * \return_by_value \perf_use MatrixBase::leftmultiply()
  *
  * \sa MatrixBase::leftmultiply()
  */
template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
VectorType operator *
( const VectorType & v,
  const MatrixBase<T, Derived, VectorType, LUDecompositionType> & m )
{
    VectorType res(m.size());
    m.vector_leftmultiplication_helper(v, & res);
    return res;
}

/**
  * Multiplication of a vector by a matrix
  * on the right: does v = v * m. Here, the vector v is regarded
  * as a row vector.
  *
  * This only makes sense if v.size() == m.size().
  *
  * \sa MatrixBase::leftmultiply()
  */
template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
VectorType & operator *=
( VectorType & v,
  const MatrixBase<T, Derived, VectorType, LUDecompositionType> & m )
{
    assert( v.size() == m.size() );
    VectorType vtmp(v);
    m.vector_leftmultiplication_helper(vtmp, & v);
    return v;
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
Derived & MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::loadDiagonal( const T & coeff )
{
    if( hasDynamicSize() || size() > 4 )
        for( int i = 0; i < size(); i++ )
        {
            (*this)( i, i ) = coeff;
            for( int j = 0; j < i; j++ )
            {
                (*this)( j, i ) = static_cast<T>(0);
                (*this)( i, j ) = static_cast<T>(0);
            }
        }
    else
    {
        (*this)( 0, 0 ) = coeff;
        if( size() >= 2 ) (*this)( 1, 1 ) = coeff;
        if( size() >= 3 ) (*this)( 2, 2 ) = coeff;
        if( size() == 4 ) (*this)( 3, 3 ) = coeff;
        if( size() >= 2 )
        {
            (*this)( 1, 0 ) = static_cast<T>(0);
            (*this)( 0, 1 ) = static_cast<T>(0);
        }
        if( size() >= 3 )
        {
            (*this)( 2, 0 ) = static_cast<T>(0);
            (*this)( 2, 1 ) = static_cast<T>(0);
            (*this)( 0, 2 ) = static_cast<T>(0);
            (*this)( 1, 2 ) = static_cast<T>(0);
        }
        if( size() == 4 )
        {
            (*this)( 3, 0 ) = static_cast<T>(0);
            (*this)( 3, 1 ) = static_cast<T>(0);
            (*this)( 3, 2 ) = static_cast<T>(0);
            (*this)( 0, 3 ) = static_cast<T>(0);
            (*this)( 1, 3 ) = static_cast<T>(0);
            (*this)( 2, 3 ) = static_cast<T>(0);
        }
    }
    return *static_cast<Derived*>(this);
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
Derived & MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::loadDiagonal( const VectorType & coeffs )
{
    resize( coeffs.size() );
    if( hasDynamicSize() || size() > 4 )
        for( int i = 0; i < size(); i++ )
        {
            (*this)( i, i ) = coeffs(i);
            for( int j = 0; j < i; j++ )
            {
                (*this)( j, i ) = static_cast<T>(0);
                (*this)( i, j ) = static_cast<T>(0);
            }
        }
    else
    {
        (*this)( 0, 0 ) = coeffs(0);
        if( size() >= 2 ) (*this)( 1, 1 ) = coeffs(1);
        if( size() >= 3 ) (*this)( 2, 2 ) = coeffs(2);
        if( size() == 4 ) (*this)( 3, 3 ) = coeffs(3);

        if( size() >= 2 )
        {
            (*this)( 1, 0 ) = static_cast<T>(0);
            (*this)( 0, 1 ) = static_cast<T>(0);
        }
        if( size() >= 3 )
        {
            (*this)( 2, 0 ) = static_cast<T>(0);
            (*this)( 2, 1 ) = static_cast<T>(0);
            (*this)( 0, 2 ) = static_cast<T>(0);
            (*this)( 1, 2 ) = static_cast<T>(0);
        }
        if( size() == 4 )
        {
            (*this)( 3, 0 ) = static_cast<T>(0);
            (*this)( 3, 1 ) = static_cast<T>(0);
            (*this)( 3, 2 ) = static_cast<T>(0);
            (*this)( 0, 3 ) = static_cast<T>(0);
            (*this)( 1, 3 ) = static_cast<T>(0);
            (*this)( 2, 3 ) = static_cast<T>(0);
        }
    }
    return *static_cast<Derived*>(this);
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
Derived & MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::loadZero()
{
    for( int i = 0; i < size() * size(); i++ )
        (*this)[i] = static_cast<T>(0);
    return *static_cast<Derived*>(this);
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
Derived & MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::operator =
( const Derived & other )
{
    if( hasDynamicSize() )
        if( static_cast<Derived*>(this) == &other )
            return *static_cast<Derived*>(this);
    resize( other.size() );
    readArray( other.array() );
    return *static_cast<Derived*>(this);
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void
MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::getRow( int row, T * ret ) const
{
    assert( row >= 0 && row < size() );
    const T *rowptr = array() + row;
    for( int i = 0; i < size(); i++ )
    {
        ret[i] = *rowptr;
        rowptr += size();
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void
MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::setRow( int row, const T * src )
{
    assert( row >= 0 && row < size() );
    T *rowptr = array() + row;
    for( int i = 0; i < size(); i++ )
    {
        *rowptr = src[i];
        rowptr += size();
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void
MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::getColumn( int column, T * ret ) const
{
    assert( column >= 0 && column < size() );
    const T *colptr = columnPtr(column);
    for( int i = 0; i < size(); i++ )
    {
        ret[i] = *colptr;
        colptr++;
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void
MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::setColumn( int column, const T * src )
{
    assert( column >= 0 && column < size() );
    T *colptr = columnPtr(column);
    for( int i = 0; i < size(); i++ )
    {
        *colptr = src[i];
        colptr++;
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::readRows( const T * rows )
{
    if( hasDynamicSize() || size() > 4 )
    {
        const T * ptr = rows;
        for( int i = 0; i < size(); i++ )
        {
            setRow( i, ptr );
            ptr += size();
        }
    }
    else
    {
        setRow( 0, rows );
        if( size() >= 2 ) setRow( 1, rows + size() );
        if( size() >= 3 ) setRow( 2, rows + 2 * size() );
        if( size() == 4 ) setRow( 3, rows + 3 * size() );
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void
MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::matrix_multiplication_helper
( const Derived & other, Derived * res ) const
{
    if( hasDynamicSize() || size() > 4 )
    {
        T *destptr = res->array();
        const T *colptr = other.array();
        for (int col = 0; col < res->size(); col++) {
            const T *rowptr = array();
            for (int row = 0; row < res->size(); row++) {
                *destptr = static_cast<T>(0);
                const T *entry1ptr = colptr;
                const T *entry2ptr = rowptr;
                for (int entry = 0; entry < size(); entry++) {
                    *destptr += (*entry1ptr) * (*entry2ptr);
                    entry1ptr++;
                    entry2ptr += size();
                }
                destptr++;
                rowptr++;
            }
            colptr += other.size();
        }
    }
    else
    {
        if( size() == 1 ) (*res)[0] = (*this)[0] * other[0];
        else if( size() == 2 )
        {
            (*res)(0, 0) = (*this)(0, 0) * other(0, 0) + (*this)(0, 1) * other(1, 0);
            (*res)(1, 0) = (*this)(1, 0) * other(0, 0) + (*this)(1, 1) * other(1, 0);
            (*res)(0, 1) = (*this)(0, 0) * other(0, 1) + (*this)(0, 1) * other(1, 1);
            (*res)(1, 1) = (*this)(1, 0) * other(0, 1) + (*this)(1, 1) * other(1, 1);
        }
        else if( size() == 3 )
        {
            (*res)(0, 0) = (*this)(0, 0) * other(0, 0)
                         + (*this)(0, 1) * other(1, 0)
                         + (*this)(0, 2) * other(2, 0);
            (*res)(1, 0) = (*this)(1, 0) * other(0, 0)
                         + (*this)(1, 1) * other(1, 0)
                         + (*this)(1, 2) * other(2, 0);
            (*res)(2, 0) = (*this)(2, 0) * other(0, 0)
                         + (*this)(2, 1) * other(1, 0)
                         + (*this)(2, 2) * other(2, 0);
            (*res)(0, 1) = (*this)(0, 0) * other(0, 1)
                         + (*this)(0, 1) * other(1, 1)
                         + (*this)(0, 2) * other(2, 1);
            (*res)(1, 1) = (*this)(1, 0) * other(0, 1)
                         + (*this)(1, 1) * other(1, 1)
                         + (*this)(1, 2) * other(2, 1);
            (*res)(2, 1) = (*this)(2, 0) * other(0, 1)
                         + (*this)(2, 1) * other(1, 1)
                         + (*this)(2, 2) * other(2, 1);
            (*res)(0, 2) = (*this)(0, 0) * other(0, 2)
                         + (*this)(0, 1) * other(1, 2)
                         + (*this)(0, 2) * other(2, 2);
            (*res)(1, 2) = (*this)(1, 0) * other(0, 2)
                         + (*this)(1, 1) * other(1, 2)
                         + (*this)(1, 2) * other(2, 2);
            (*res)(2, 2) = (*this)(2, 0) * other(0, 2)
                         + (*this)(2, 1) * other(1, 2)
                         + (*this)(2, 2) * other(2, 2);
        }
        else // size() == 4
        {
            (*res)(0, 0) = (*this)(0, 0) * other(0, 0)
                         + (*this)(0, 1) * other(1, 0)
                         + (*this)(0, 2) * other(2, 0)
                         + (*this)(0, 3) * other(3, 0);
            (*res)(1, 0) = (*this)(1, 0) * other(0, 0)
                         + (*this)(1, 1) * other(1, 0)
                         + (*this)(1, 2) * other(2, 0)
                         + (*this)(1, 3) * other(3, 0);
            (*res)(2, 0) = (*this)(2, 0) * other(0, 0)
                         + (*this)(2, 1) * other(1, 0)
                         + (*this)(2, 2) * other(2, 0)
                         + (*this)(2, 3) * other(3, 0);
            (*res)(3, 0) = (*this)(3, 0) * other(0, 0)
                         + (*this)(3, 1) * other(1, 0)
                         + (*this)(3, 2) * other(2, 0)
                         + (*this)(3, 3) * other(3, 0);
            (*res)(0, 1) = (*this)(0, 0) * other(0, 1)
                         + (*this)(0, 1) * other(1, 1)
                         + (*this)(0, 2) * other(2, 1)
                         + (*this)(0, 3) * other(3, 1);
            (*res)(1, 1) = (*this)(1, 0) * other(0, 1)
                         + (*this)(1, 1) * other(1, 1)
                         + (*this)(1, 2) * other(2, 1)
                         + (*this)(1, 3) * other(3, 1);
            (*res)(2, 1) = (*this)(2, 0) * other(0, 1)
                         + (*this)(2, 1) * other(1, 1)
                         + (*this)(2, 2) * other(2, 1)
                         + (*this)(2, 3) * other(3, 1);
            (*res)(3, 1) = (*this)(3, 0) * other(0, 1)
                         + (*this)(3, 1) * other(1, 1)
                         + (*this)(3, 2) * other(2, 1)
                         + (*this)(3, 3) * other(3, 1);
            (*res)(0, 2) = (*this)(0, 0) * other(0, 2)
                         + (*this)(0, 1) * other(1, 2)
                         + (*this)(0, 2) * other(2, 2)
                         + (*this)(0, 3) * other(3, 2);
            (*res)(1, 2) = (*this)(1, 0) * other(0, 2)
                         + (*this)(1, 1) * other(1, 2)
                         + (*this)(1, 2) * other(2, 2)
                         + (*this)(1, 3) * other(3, 2);
            (*res)(2, 2) = (*this)(2, 0) * other(0, 2)
                         + (*this)(2, 1) * other(1, 2)
                         + (*this)(2, 2) * other(2, 2)
                         + (*this)(2, 3) * other(3, 2);
            (*res)(3, 2) = (*this)(3, 0) * other(0, 2)
                         + (*this)(3, 1) * other(1, 2)
                         + (*this)(3, 2) * other(2, 2)
                         + (*this)(3, 3) * other(3, 2);
            (*res)(0, 3) = (*this)(0, 0) * other(0, 3)
                         + (*this)(0, 1) * other(1, 3)
                         + (*this)(0, 2) * other(2, 3)
                         + (*this)(0, 3) * other(3, 3);
            (*res)(1, 3) = (*this)(1, 0) * other(0, 3)
                         + (*this)(1, 1) * other(1, 3)
                         + (*this)(1, 2) * other(2, 3)
                         + (*this)(1, 3) * other(3, 3);
            (*res)(2, 3) = (*this)(2, 0) * other(0, 3)
                         + (*this)(2, 1) * other(1, 3)
                         + (*this)(2, 2) * other(2, 3)
                         + (*this)(2, 3) * other(3, 3);
            (*res)(3, 3) = (*this)(3, 0) * other(0, 3)
                         + (*this)(3, 1) * other(1, 3)
                         + (*this)(3, 2) * other(2, 3)
                         + (*this)(3, 3) * other(3, 3);
        }
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void
MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::vector_multiplication_helper
( const VectorType & vector, VectorType * res ) const
{
    if( hasDynamicSize() || size() > 4 )
    {
        T *destptr = res->array();
        for( int i = 0; i < size(); i++)
        {
            const T *entryptr = array() + i;
            *destptr = static_cast<T>(0);
            for( int j = 0; j < size(); j++)
            {
                *destptr += (*entryptr) * vector[j];
                entryptr += size();
            }
            destptr++;
        }
    }
    else
    {
        if( size() == 1 ) (*res)[0] = (*this)[0] * vector[0];
        else if( size() == 2 )
        {
            (*res)[0] = (*this)(0, 0) * vector[0] + (*this)(0, 1) * vector[1];
            (*res)[1] = (*this)(1, 0) * vector[0] + (*this)(1, 1) * vector[1];
        }
        else if( size() == 3 )
        {
            (*res)[0] = (*this)(0, 0) * vector[0]
                   + (*this)(0, 1) * vector[1]
                   + (*this)(0, 2) * vector[2];
            (*res)[1] = (*this)(1, 0) * vector[0]
                   + (*this)(1, 1) * vector[1]
                   + (*this)(1, 2) * vector[2];
            (*res)[2] = (*this)(2, 0) * vector[0]
                   + (*this)(2, 1) * vector[1]
                   + (*this)(2, 2) * vector[2];
        }
        else // size() == 4
        {
            (*res)[0] = (*this)(0, 0) * vector[0]
                   + (*this)(0, 1) * vector[1]
                   + (*this)(0, 2) * vector[2]
                   + (*this)(0, 3) * vector[3];
            (*res)[1] = (*this)(1, 0) * vector[0]
                   + (*this)(1, 1) * vector[1]
                   + (*this)(1, 2) * vector[2]
                   + (*this)(1, 3) * vector[3];
            (*res)[2] = (*this)(2, 0) * vector[0]
                   + (*this)(2, 1) * vector[1]
                   + (*this)(2, 2) * vector[2]
                   + (*this)(2, 3) * vector[3];
            (*res)[3] = (*this)(3, 0) * vector[0]
                   + (*this)(3, 1) * vector[1]
                   + (*this)(3, 2) * vector[2]
                   + (*this)(3, 3) * vector[3];
        }
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void
MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::vector_leftmultiplication_helper
( const VectorType & vector, VectorType * res ) const
{
    if( hasDynamicSize() || size() > 4 )
    {
        T *destptr = res->array();
        for( int i = 0; i < size(); i++)
        {
            const T *entryptr = array() + i * size();
            *destptr = static_cast<T>(0);
            for( int j = 0; j < size(); j++)
            {
                *destptr += vector[j] * (*entryptr);
                entryptr++;
            }
            destptr++;
        }
    }
    else
    {
        if( size() == 1 ) (*res)[0] = vector[0] * (*this)[0];
        else if( size() == 2 )
        {
            (*res)[0] = vector[0] * (*this)(0, 0) + vector[1] * (*this)(1, 0);
            (*res)[1] = vector[0] * (*this)(0, 1) + vector[1] * (*this)(1, 1);
        }
        else if( size() == 3 )
        {
            (*res)[0] = vector[0] * (*this)(0, 0)
                   + vector[1] * (*this)(1, 0)
                   + vector[2] * (*this)(2, 0);
            (*res)[1] = vector[0] * (*this)(0, 1)
                   + vector[1] * (*this)(1, 1)
                   + vector[2] * (*this)(2, 1);
            (*res)[2] = vector[0] * (*this)(0, 2)
                   + vector[1] * (*this)(1, 2)
                   + vector[2] * (*this)(2, 2);
        }
        else // size() == 4
        {
            (*res)[0] = vector[0] * (*this)(0, 0)
                   + vector[1] * (*this)(1, 0)
                   + vector[2] * (*this)(2, 0)
                   + vector[3] * (*this)(3, 0);
            (*res)[1] = vector[0] * (*this)(0, 1)
                   + vector[1] * (*this)(1, 1)
                   + vector[2] * (*this)(2, 1)
                   + vector[3] * (*this)(3, 1);
            (*res)[2] = vector[0] * (*this)(0, 2)
                   + vector[1] * (*this)(1, 2)
                   + vector[2] * (*this)(2, 2)
                   + vector[3] * (*this)(3, 2);
            (*res)[3] = vector[0] * (*this)(0, 3)
                   + vector[1] * (*this)(1, 3)
                   + vector[2] * (*this)(2, 3)
                   + vector[3] * (*this)(3, 3);
        }
    }
}


template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
T MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::determinant() const
{
    if( size() == 1 )
        return (*this)[0];
    else if( size() == 2 )
        return (*this)[0] * (*this)[3] - (*this)[1] * (*this)[2];
    else if( size() == 3 )
        return (*this)[0] * ( (*this)[4] * (*this)[8] - (*this)[5] * (*this)[7] )
             - (*this)[1] * ( (*this)[3] * (*this)[8] - (*this)[5] * (*this)[6] )
             + (*this)[2] * ( (*this)[3] * (*this)[7] - (*this)[4] * (*this)[6] );
    else
        return LUDecompositionType(
                        *static_cast<const Derived*>(this)
                                  ).determinant();
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::computeInverse( Derived * result ) const
{
    result->resize( size() );
    if( size() == 1 )
    {
        (*result)[0] = static_cast<T>(1) / (*this)[0];
    }
    else if( size() == 2 )
    {
        T det = (*this)[0] * (*this)[3] - (*this)[1] * (*this)[2];
        (*result)(0, 0) = (*this)(1,1);
        (*result)(1, 0) = - (*this)(1,0);
        (*result)(0, 1) = - (*this)(0,1);
        (*result)(1, 1) = (*this)(0,0);
        (*result) /= det;
    }
    else if( size() == 3 )
    {
        T det = (*this)[0] * ( (*this)[4] * (*this)[8] - (*this)[5] * (*this)[7] )
              - (*this)[1] * ( (*this)[3] * (*this)[8] - (*this)[5] * (*this)[6] )
              + (*this)[2] * ( (*this)[3] * (*this)[7] - (*this)[4] * (*this)[6] );
        (*result)(0, 0) =
            (*this)(1,1) * (*this)(2,2) - (*this)(1,2) * (*this)(2,1);
        (*result)(0, 1) =
            - (*this)(0,1) * (*this)(2,2) + (*this)(0,2) * (*this)(2,1);
        (*result)(0, 2) =
            (*this)(0,1) * (*this)(1,2) - (*this)(1,1) * (*this)(0,2);
        (*result)(1, 0) =
            - (*this)(1,0) * (*this)(2,2) + (*this)(1,2) * (*this)(2,0);
        (*result)(1, 1) =
            (*this)(0,0) * (*this)(2,2) - (*this)(0,2) * (*this)(2,0);
        (*result)(1, 2) =
            - (*this)(0,0) * (*this)(1,2) + (*this)(1,0) * (*this)(0,2);
        (*result)(2, 0) =
            (*this)(1,0) * (*this)(2,1) - (*this)(1,1) * (*this)(2,0);
        (*result)(2, 1) =
            - (*this)(0,0) * (*this)(2,1) + (*this)(0,1) * (*this)(2,0);
        (*result)(2, 2) =
            (*this)(0,0) * (*this)(1,1) - (*this)(1,0) * (*this)(0,1);
        (*result) /= det;
    }
    else
    {
        LUDecompositionType( *static_cast<const Derived*>(this) )
            .computeInverse(result);
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::computeInverseSafely( Derived * result, bool * invertible ) const
{
    if( size() == 1 )
    {
        if( (*this)[0] == static_cast<T>(0) )
        {
            *invertible = false;
        }
        else
        {
            *invertible = true;
            result->resize(1);
            (*result)[0] = static_cast<T>(1) / (*this)[0];
        }
    }
    else if( size() == 2 )
    {
        T det = (*this)[0] * (*this)[3] - (*this)[1] * (*this)[2];
        if( det == static_cast<T>(0) )
        {
            *invertible = false;
        }
        else
        {
            *invertible = true;
            result->resize(2);
            (*result)(0, 0) = (*this)(1,1);
            (*result)(1, 0) = - (*this)(1,0);
            (*result)(0, 1) = - (*this)(0,1);
            (*result)(1, 1) = (*this)(0,0);
            (*result) /= det;
        }
    }
    else if( size() == 3 )
    {
        T det = (*this)[0] * ( (*this)[4] * (*this)[8] - (*this)[5] * (*this)[7] )
              - (*this)[1] * ( (*this)[3] * (*this)[8] - (*this)[5] * (*this)[6] )
              + (*this)[2] * ( (*this)[3] * (*this)[7] - (*this)[4] * (*this)[6] );
        if( det == static_cast<T>(0) )
        {
            *invertible = false;
        }
        else
        {
            *invertible = true;
            result->resize(3);
            (*result)(0, 0) =
                (*this)(1,1) * (*this)(2,2) - (*this)(1,2) * (*this)(2,1);
            (*result)(0, 1) =
              - (*this)(0,1) * (*this)(2,2) + (*this)(0,2) * (*this)(2,1);
            (*result)(0, 2) =
                (*this)(0,1) * (*this)(1,2) - (*this)(1,1) * (*this)(0,2);
            (*result)(1, 0) =
              - (*this)(1,0) * (*this)(2,2) + (*this)(1,2) * (*this)(2,0);
            (*result)(1, 1) =
                (*this)(0,0) * (*this)(2,2) - (*this)(0,2) * (*this)(2,0);
            (*result)(1, 2) =
              - (*this)(0,0) * (*this)(1,2) + (*this)(1,0) * (*this)(0,2);
            (*result)(2, 0) =
                (*this)(1,0) * (*this)(2,1) - (*this)(1,1) * (*this)(2,0);
            (*result)(2, 1) =
              - (*this)(0,0) * (*this)(2,1) + (*this)(0,1) * (*this)(2,0);
            (*result)(2, 2) =
                (*this)(0,0) * (*this)(1,1) - (*this)(1,0) * (*this)(0,1);
            (*result) /= det;
        }
    }
    else
    {
        LUDecompositionType luDecomp( *static_cast<const Derived*>(this) );
        *invertible = luDecomp.isInvertible();
        if(*invertible) luDecomp.computeInverse(result);
    }
}



template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
bool MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::isInvertible() const
{
    if( size() <= 3 )
    {
        int row_max_abs, col_max_abs;
        findBiggestEntry( &row_max_abs, &col_max_abs );
        return( ! Util::isNegligible( determinant(),
                                      (*this)( row_max_abs, col_max_abs ) ) );
    }
    else return LUDecompositionType(
                 *static_cast<const Derived*>(this)
                                   ).isInvertible();
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
        ::findBiggestEntry( int * res_row, int * res_col, int skip ) const
{
    int row_max_abs = skip, col_max_abs = skip;
    const T *entry_ptr = &( (*this)(skip, skip) );
    const T *entry_max_abs_ptr = entry_ptr;
    for( int col = skip; col < size(); col++)
    {
        for( int row = skip; row < size(); row++)
        {
            if( std::abs( *entry_ptr ) > std::abs( *entry_max_abs_ptr ) )
            {
                row_max_abs = row;
                col_max_abs = col;
                entry_max_abs_ptr = entry_ptr;
            }
            entry_ptr++;
        }
        entry_ptr += skip;
    }
    *res_row = row_max_abs;
    *res_col = col_max_abs;
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
    ::computeAdjoint( Derived * result ) const
{
    result->resize( size() );
    if( hasDynamicSize() || size() > 4 )
    {
        for( int row = 0; row < size(); row++ )
            for( int col = 0; col < size(); col++ )
                (*result)( col, row ) = Util::conj( (*this)( row, col ) );
    }
    else
    {
        (*result)( 0, 0 ) = Util::conj( (*this)( 0, 0 ) );
        if( size() >= 2 )
        {
            (*result)( 1, 0 ) = Util::conj( (*this)( 0, 1 ) );
            (*result)( 0, 1 ) = Util::conj( (*this)( 1, 0 ) );
            (*result)( 1, 1 ) = Util::conj( (*this)( 1, 1 ) );
        }
        if( size() >= 3 )
        {
            (*result)( 2, 0 ) = Util::conj( (*this)( 0, 2 ) );
            (*result)( 2, 1 ) = Util::conj( (*this)( 1, 2 ) );
            (*result)( 0, 2 ) = Util::conj( (*this)( 2, 0 ) );
            (*result)( 1, 2 ) = Util::conj( (*this)( 2, 1 ) );
            (*result)( 2, 2 ) = Util::conj( (*this)( 2, 2 ) );
        }
        if( size() == 4 )
        {
            (*result)( 3, 0 ) = Util::conj( (*this)( 0, 3 ) );
            (*result)( 3, 1 ) = Util::conj( (*this)( 1, 3 ) );
            (*result)( 3, 2 ) = Util::conj( (*this)( 2, 3 ) );
            (*result)( 0, 3 ) = Util::conj( (*this)( 3, 0 ) );
            (*result)( 1, 3 ) = Util::conj( (*this)( 3, 1 ) );
            (*result)( 2, 3 ) = Util::conj( (*this)( 3, 2 ) );
            (*result)( 3, 3 ) = Util::conj( (*this)( 3, 3 ) );
        }
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
    ::replaceWithAdjoint()
{
    if( hasDynamicSize() || size() > 4 )
    {
        for( int row = 0; row < size(); row++ )
        {
            (*this)( row, row ) = Util::conj( (*this)( row, row ) );
            for( int col = row+1; col < size(); col++ )
            {
                T tmp = (*this)( row, col );
                (*this)( row, col ) = Util::conj( (*this)( col, row ) );
                (*this)( col, row ) = Util::conj( tmp );
            }
        }
    }
    else
    {
        if( size() == 1 )
            (*this)( 0, 0 ) = Util::conj( (*this)( 0, 0 ) );
        T tmp;
        if( size() >= 2 )
        {
            tmp = Util::conj( (*this)( 1, 0 ) );
            (*this)( 1, 0 ) = Util::conj( (*this)( 0, 1 ) );
            (*this)( 0, 1 ) = tmp;
            (*this)( 1, 1 ) = Util::conj( (*this)( 1, 1 ) );
        }
        if( size() >= 3 )
        {
            tmp = Util::conj( (*this)( 2, 0 ) );
            (*this)( 2, 0 ) = Util::conj( (*this)( 0, 2 ) );
            (*this)( 0, 2 ) = tmp;
            tmp = Util::conj( (*this)( 2, 1 ) );
            (*this)( 2, 1 ) = Util::conj( (*this)( 1, 2 ) );
            (*this)( 1, 2 ) = tmp;
            (*this)( 2, 2 ) = Util::conj( (*this)( 2, 2 ) );
        }
        if( size() == 4 )
        {
            tmp = Util::conj( (*this)( 3, 0 ) );
            (*this)( 3, 0 ) = Util::conj( (*this)( 0, 3 ) );
            (*this)( 0, 3 ) = tmp;
            tmp = Util::conj( (*this)( 3, 1 ) );
            (*this)( 3, 1 ) = Util::conj( (*this)( 1, 3 ) );
            (*this)( 1, 3 ) = tmp;
            tmp = Util::conj( (*this)( 3, 2 ) );
            (*this)( 3, 2 ) = Util::conj( (*this)( 2, 3 ) );
            (*this)( 2, 3 ) = tmp;
            (*this)( 3, 3 ) = Util::conj( (*this)( 3, 3 ) );
        }
    }
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
Derived & MatrixBase<T, Derived, VectorType, LUDecompositionType>
    ::loadRotation2( const T & angle )
{
    resize(2);
    T _cos = cos( angle ), _sin = sin( angle );
    (*this)(0, 0) = _cos;
    (*this)(1, 1) = _cos;
    (*this)(1, 0) = _sin;
    (*this)(0, 1) = -_sin;
    return *static_cast<Derived*>(this);
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
Derived & MatrixBase<T, Derived, VectorType, LUDecompositionType>
    ::loadRotation3( const T & angle, const VectorType & axis )
{
    resize(3);
    T _cos = cos( angle ), _sin = sin( angle );
    T one_minus_cos = static_cast<T>(1) - _cos;
    T coord_times_one_minus_cos = axis.x() * one_minus_cos;
    (*this)( 0, 0 ) = axis.x() * coord_times_one_minus_cos + _cos;
    T coord_times_coord_times_one_minus_cos = axis.y()
                                            * coord_times_one_minus_cos;
    T other_coord_times_sin = axis.z() * _sin;
    (*this)( 1, 0 ) = coord_times_coord_times_one_minus_cos
                    + other_coord_times_sin;
    (*this)( 0, 1 ) = coord_times_coord_times_one_minus_cos
                    - other_coord_times_sin;
    coord_times_coord_times_one_minus_cos = axis.z()
                                          * coord_times_one_minus_cos;
    other_coord_times_sin = axis.y() * _sin;
    (*this)( 2, 0 ) = coord_times_coord_times_one_minus_cos
                    - other_coord_times_sin;
    (*this)( 0, 2 ) = coord_times_coord_times_one_minus_cos
                    + other_coord_times_sin;
    coord_times_one_minus_cos = axis.y() * one_minus_cos;
    (*this)( 1, 1 ) = axis.y() * coord_times_one_minus_cos + _cos;
    coord_times_coord_times_one_minus_cos = axis.z()
                                          * coord_times_one_minus_cos;
    other_coord_times_sin = axis.x() * _sin;
    (*this)( 2, 1 ) = coord_times_coord_times_one_minus_cos
                    + other_coord_times_sin;
    (*this)( 1, 2 ) = coord_times_coord_times_one_minus_cos
                    - other_coord_times_sin;
    (*this)( 2, 2 ) = axis.z() * axis.z() * one_minus_cos + _cos;
    return *static_cast<Derived*>(this);
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
void MatrixBase<T, Derived, VectorType, LUDecompositionType>
    ::recursiveGramSchmidt( int n )
{
    assert( n >= 1 && n < size() );
    VectorType v0( size() );
    VectorType v( size() );
    do
    {
        v0.loadRandomUnit();
        v = v0;
        for( int i = 0; i < n; i++ )
        {
            T d = static_cast<T>(0);
            const T * colptr = columnPtr( i );
            for( int j = 0; j < size(); j++ )
                d += Util::conj( colptr[j] ) * v0(j);
            for( int j = 0; j < size(); j++ )
                v(j) -= d * colptr[j];
        }
    } while( std::abs(v.norm2()) < 1e-3 );
    v.normalize();
    setColumn( n, v );
    if( n < size() - 1 ) recursiveGramSchmidt( n + 1 );
}

template< typename T, typename Derived,
          typename VectorType, typename LUDecompositionType >
Derived & MatrixBase<T, Derived, VectorType, LUDecompositionType>
    ::loadOrthoBasis( const VectorType & vector )
{
    resize( vector.size() );
    if( size() <= 3 )
    {
        setColumn( 0, vector );
        if( size() == 1 ) return *static_cast<Derived*>(this);
        T norm = vector.norm();
        if( size() == 2 )
        {
            (*this)(0,1) = -( vector(1) / norm );
            (*this)(1,1) = vector(0) / norm;
        }
        else
        {
            VectorType v( size() );
            vector.makeOrthoVector( & v );
            setColumn( 1, v );
            setColumn( 2, cross( vector, v ) / norm );
        }
    }
    else
    {
        VectorType v( vector );
        v.normalize();
        setColumn( 0, v );
        recursiveGramSchmidt( 1 );
        setColumn( 0, vector );
    }
    return *static_cast<Derived*>(this);
}

/**
  * Allows to print a matrix by simply doing
  * @code
    cout << mymatrix << endl;
  * @endcode
  */
template<typename T,
         typename Derived,
         typename VectorType,
         typename LUDecompositionType>
std::ostream & operator <<
( std::ostream & s,
  const MatrixBase<T, Derived, VectorType, LUDecompositionType> & m )
{
    for( int i = 0; i < m.size(); i++ )
    {
        s << m( i, 0 );
        for (int j = 1; j < m.size(); j++ )
            s << " " << m( i, j );
        if( i < m.size() - 1)
            s << std::endl;
    }
    return s;
}

} // namespace Eigen

#endif // EIGEN_MATRIXBASE_H
