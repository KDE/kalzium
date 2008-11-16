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

/** \file vectorbase.h
  * \brief Internal file
  */

#ifndef EIGEN_VECTORBASE_H
#define EIGEN_VECTORBASE_H

#include "util.h"

namespace Eigen
{

/** \ingroup internalbases
  *
  * \ingroup vectors
  *
  * \brief internal base class
  *
  * This class template is only internally used in Eigen.
  * It provides the base that the Vector and VectorX
  * class templates inherit.
  *
  * Note that one template parameter is Derived: this is a
  * C++ trick knows as Curiously Recursive Template Pattern.
  * Here, it allows us to implement in VectorBase the code
  * of both Vector and VectorX, which are very different
  * (Vector stores the size of the vector as template argument,
  * while VectorX stores it as member data, thus storing the
  * array itself on the heap).
  */
template<typename T, typename Derived>
class VectorBase
{
public:

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
    void multiplication_helper
    ( const T & factor, Derived * res ) const;

    /** \internal
      * Helper method, computes *this / factor and stores the result in res.
      * Doesn't attempt to resize res. Doesn't check that the sizes match.
      */
    void division_helper
    ( const T & factor, Derived * res ) const
    {
        multiplication_helper( static_cast<const T>(1) / factor, res );
    }

    /** \internal
      * Helper method, computes the cross product of *this and factor
      * and stores the result in res_array (an array, not a vector).
      * Doesn't check the sizes (must be 3).
      */
    void crossproduct_helper
    ( const Derived & other, T * res_array ) const
    {
        res_array[0] =  (*this)[1] * other[2] - (*this)[2] * other[1];
        res_array[1] =  (*this)[2] * other[0] - (*this)[0] * other[2];
        res_array[2] =  (*this)[0] * other[1] - (*this)[1] * other[0];
    }

public:

    /**
      * @returns true if the vector has dynamic size (i.e. is an
      * object of class VectorX), false if the vector has fixed size
      * (i.e. is an object of class Vector).
      *
      * \sa size(), resize()
      */
    bool hasDynamicSize() const
    {
        return static_cast<const Derived*>(this)->_hasDynamicSize();
    }

    /**
      * Returns the size (dimension) of the vector.
      *
      * \sa hasDynamicSize(), resize()
      */
    int size() const
    {
        return static_cast<const Derived*>(this)->_size();
    }

    /**
      * Tries to resize the vector. That is only possible if the vector
      * has dynamic size, i.e. is an object of class VectorX. Otherwise,
      * nothing is done.
      *
      * The vector coords are not kept, they
      * are left with undefined values after resizing.
      *
      * \sa size(), hasDynamicSize()
      */
    void resize( int newsize )
    {
        static_cast<Derived*>(this)->_resize(newsize);
    }

    /**
      * Returns the array of the vector, as constant.
      *
      * \sa array(), operator()(int) const, operator[](int) const
      */
    const T * array() const
    {
        return static_cast<const Derived*>(this)->m_array;
    }

    /**
      * @returns the array of the vector, as non-constant.
      *
      * \sa array() const, operator()(int), operator[](int)
      */
    T * array()
    {
        return static_cast<Derived*>(this)->m_array;
    }

    /**
      * @returns a constant reference to the i-th coord of the vector.
      *
      * Same as operator[].
      *
      * \sa operator()(int), operator[](int) const
      */
    const T & operator () ( int i ) const
    {
        assert( i >= 0 && i < size() );
        return array() [i];
    }

    /**
      * @returns a non-constant reference to the i-th coord of the vector.
      *
      * Same as operator[].
      *
      * \sa operator()(int) const, operator[](int)
      */
    T & operator () ( int i )
    {
        assert( i >= 0 && i < size() );
        return array() [i];
    }

    /**
      * @returns a constant reference to the i-th coord of the vector.
      *
      * Same as operator().
      *
      * \sa operator[](int), operator()(int) const
      */
    const T & operator [] ( int i ) const
    {
        assert( i >= 0 && i < size() );
        return array() [i];
    }

    /**
      * @returns a non-constant reference to the i-th coord of the vector.
      *
      * Same as operator().
      *
      * \sa operator[](int) const, operator()(int)
      */
    T & operator [] ( int i )
    {
        assert( i >= 0 && i < size() );
        return array() [i];
    }

    /**
      * Returns the dot product of *this by other.
      *
      * *this and other must have the same size (the compiler will check that
      * for fixed-size vectors, but not for dynamic-size vectors).
      *
      * If T is std::complex, the dot product is hermitian, i.e.
      * the coords of *this get complex-conjugated in the formula.
      *
      * \sa norm(), norm2()
      */
    T dot( const Derived & other ) const;

    /**
      * Returns the cross product of *this by other.
      * *this and other must have size exactly 3.
      *
      * \return_by_value \perf_use cross(const Derived &, Derived *) const
      *
      * \sa cross(const Derived &, Derived *) const
      */
    Derived cross( const Derived & other ) const
    {
        assert( other.size() == 3 && size() == 3 );
        Derived res(3);
        crossproduct_helper( other, res.array() );
        return res;
    }

    /**
      * Sets *res to be the cross product of *this by other.
      * *this and other must have size exactly 3.
      *
      * In fixed-size, *res must also have size 3. In dynamic-size,
      * *res gets resized to size 3 if necessary.
      *
      * \sa cross(const Derived &) const
      */
    void cross( const Derived & other, Derived * res ) const
    {
        assert( other.size() == 3 && size() == 3 );
        res->resize(3);
        crossproduct_helper( other, res->array() );
    }

    /**
      * Returns the squared norm of *this, that is, the dot product
      * of *this with itself.
      *
      * \sa norm(), dot()
      */
    T norm2() const
    {
        return dot( *static_cast<const Derived*>(this) );
    }

    /**
      * Returns the norm of *this, obtained as the square root of norm2().
      *
      * \sa norm2(), dot(), normalize()
      */
    T norm() const
    {
        return sqrt( norm2() );
    }

    /**
      * Normalizes *this, that is, divides *this by norm().
      *
      * \sa norm(), normalized()
      */
    Derived & normalize()
    {
        (*this) /= norm();
        return *static_cast<Derived *>(this);
    }

    /**
      * Returns a normalized copy of *this. In other words,
      * returns (*this) / norm().
      *
      * \return_by_value
      *
      * \sa normalize()
      */
    Derived normalized()
    {
        Derived ret( *static_cast<Derived *>(this) );
        return(ret.normalize());
    }

    /**
      * Loads into *this a unit vector that is orthogonal to \a other.
      *
      * The size of \a other must be at least 2.
      * *this gets resized to have the same size, if it has dynamic size.
      *
      * If the size is exactly 2, then other points toward the left, \ie
      * other.x() = -y() and other.y() = x(). For dimensions at least 3, it
      * is of course impossible to speak of "pointing toward the left".
      *
      * @returns a reference to *this.
      *
      * \sa ortho(), MatrixBase::loadOrthoBasis()
      */
    Derived & loadOrtho( const Derived & other );

    /**
      * Constructs a unit vector that is orthogonal to *this,
      * and stores it into *res.
      *
      * *res and *this must have the same size, and that size must
      * be at least 2.
      *
      * @returns a reference to *res.
      *
      * \deprecated use loadOrtho() or ortho() instead.
      *
      * \sa loadOrtho(), ortho(), MatrixBase::loadOrthoBasis()
      */
    Derived & makeOrthoVector( Derived * res ) const
    {
        return res->loadOrtho( *static_cast<const Derived*>(this) );
    }

     /**
      * Returns a unit vector that is orthogonal to *this.
      *
      * \return_by_value \perf_use loadOrtho() instead.
      *
      * \sa loadOrtho(), makeOrthoVectorMatrixBase::loadOrthoBasis()
      */
    Derived ortho() const
    {
        Derived res( size() );
        return res.loadOrtho( *static_cast<const Derived*>(this) );
    }

    /**
      * Sets all coords of *this to zero.
      *
      * \sa loadRandom()
      */
    Derived & loadZero();

    /**
      * Sets all coords to random values between -1.0 and 1.0. For complex
      * numbers, both the real and imaginary parts can range from -1.0 to 1.0.
      * The resulting vector can be zero (though that's not going to
      * happen often!)
      *
      * \sa loadRandomUnit(), loadZero()
      */
    Derived & loadRandom();

    /**
      * Loads into *this a random unit vector.
      *
      * \sa loadRandom(), normalize()
      */
    Derived & loadRandomUnit()
    {
        T x;
        do x = loadRandom().norm();
        while( x == static_cast<T>(0) );
        (*this) /= x;
        return *static_cast<Derived *>(this);
    }

    /**
      * Reads the coords of *this from an array. The number of entries
      * read from the array is equal to size().
      *
      * \sa operator=()
      */
    void readArray( const T * src );

    /**
      * Copies other into *this.
      *
      * *this gets resized if it didn't already have the same size as other.
      *
      * \sa readArray()
      */
    Derived & operator = ( const Derived & other )
    {
        if( hasDynamicSize() )
            if( &other == static_cast<Derived*>(this) )
                return *static_cast<Derived*>(this);
        resize( other.size() );
        readArray( other.array() );
        return *static_cast<Derived*>(this);
    }

    /**
      * Stores *this + other into *this (coordinate-wise addition).
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
      * Stores *this - other into *this (coordinate-wise substraction).
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
      * Stores *this * factor into *this (multiplication of each coord).
      *
      * \sa operator*(const T&) const
      */
    Derived & operator *= ( const T & factor )
    {
        multiplication_helper( factor, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Stores *this / factor into *this (division of each coord).
      *
      * \sa operator*=(const T&), operator/(const T&) const
      */
    Derived & operator /= ( const T & factor )
    {
        division_helper( factor, static_cast<Derived*>(this) );
        return *static_cast<Derived*>(this);
    }

    /**
      * Returns *this + other (coordinate-wise addition).
      * The vectors *this and other must have
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
        addition_helper( other, &res );
        return res;
    }

    /**
      * Returns *this - other (coordinate-wise substraction).
      * The vectors *this and other must have
      * the same size.
      *
      * \return_by_value
      *
      * \sa operator-=(const Derived &), operator-(void) const
      */
    Derived operator - ( const Derived & other ) const
    {
        assert( other.size() == size() );
        Derived res( size() );
        substraction_helper( other, &res );
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
        for( int i = 0; i < size(); i++ )
            res[i] = -( (*this)[i] );
        return res;
    }

    /** Replaces *this with (-(*this)).
      *
      * \sa operator-(void) const
      */
    void replaceWithOpposite()
    {
        for( int i = 0; i < size(); i++ )
            (*this)[i] = -( (*this)[i] );
    }

    /**
      * Returns true if *this and other are approximately equal.
      *
      * The optional parameter precision allows to control the number
      * of significant digits of precision. For instance, setting precision
      * to 1e-5 results in a precision of 5 decimal digits.
      *
      * This test is for nonzero vectors. If either of the two vectors
      * being compared is zero, then it returns true if, and only if the other
      * one is also zero -- which is not what one typically wants.
      *
      * To compare a vector with the zero vector, i.e. to check whether a
      * vector is approximately zero, use isZero() instead.
      *
      * \sa operator==(), operator!=(), isZero()
      */
    bool isApprox( const Derived & other, const T & precision = Util::epsilon<T>() ) const
    {
        return( std::abs( (*this - other).norm2() )
             <= std::min( std::abs(norm2()), std::abs(other.norm2()) )
              * Util::abs2(precision) );
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
      * Returns true if all coeffs of *this are smaller (in absolute value)
      * than other*precision. In other words, returns true if all coeffs are
      * much smaller than \a other. For the meaning of \a precision, see isApprox().
      *
      * \sa isNegligible( const Derived &, const T & ) const, isApprox(), isZero()
      */
    bool isNegligible( const T & other, const T & precision = Util::epsilon<T>() ) const
    {
        bool ret = true;
        for( int i = 0; i < size(); i++ )
            if( std::abs((*this)(i)) > std::abs(other*precision) )
                ret = false;
        return ret;
    }

    /**
      * Checks whether the vector *this is much smaller than \a other.
      *
      * Equivalent to isNegligible( other.norm(), precision ).
      *
      * \sa isNegligible( const T &, const T & ) const
      */
    bool isNegligible( const Derived & other, const T & precision = Util::epsilon<T>() ) const
    {
        return isNegligible( other.norm(), precision );
    }

    /** Tests whether *this is approximately equal to the zero matrix.
      * 
      * Equivalent to isNegligible(1). In other words, returns true if
      * all entries of *this are approximately zero, in the sense that
      * they have absolute value smaller than epsilon.
      *
      * \sa isNegligible( const T &, const T & ) const, isApprox()
      */
    bool isZero( const T & precision = Util::epsilon<T>() ) const
    {
        return isNegligible( static_cast<T>(1) );
    }

    /**
      * Returns *this * factor (multiplication of each coord).
      *
      * \return_by_value
      *
      * \sa operator*=(const T&)
      */
    Derived operator * ( const T & factor ) const
    {
        Derived res( size() );
        multiplication_helper( factor, &res );
        return res;
    }

    /**
      * Returns *this / factor (division of each coord).
      *
      * \return_by_value
      *
      * \sa operator/=(const T&), operator*(const T&) const
      */
    Derived operator / ( const T & factor ) const
    {
        Derived res( size() );
        division_helper( factor, &res );
        return res;
    }

    /**
      * Returns a reference to the first coord of *this.
      *
      * \sa x() const
      */
    T & x() { return (*this)[0]; }

    /**
      * Returns a constant reference to the first coord of *this.
      *
      * \sa x()
      */
    const T & x() const { return (*this)[0]; }

    /**
      * Returns a reference to the second coord of *this.
      *
      * The size of *this must be at least 2.
      *
      * \sa y() const
      */
    T & y()
    {
        assert( size() >= 2 );
        return (*this)[1];
    }

    /**
      * Returns a constant reference to the second coord of *this.
      *
      * The size of *this must be at least 2.
      *
      * \sa y()
      */
    const T & y() const
    {
        assert( size() >= 2 );
        return (*this)[1];
    }

    /**
      * Returns a reference to the third coord of *this.
      *
      * The size of *this must be at least 3.
      *
      * \sa z() const
      */
    T & z()
    {
        assert( size() >= 3 );
        return (*this)[2];
    }

    /**
      * Returns a constant reference to the third coord of *this.
      *
      * The size of *this must be at least 3.
      *
      * \sa z()
      */
    const T & z() const
    {
        assert( size() >= 3 );
        return (*this)[2];
    }

    /**
      * Returns a reference to the fourth coord of *this.
      *
      * The size of *this must be at least 4.
      *
      * \sa w() const
      */
    T & w()
    {
        assert( size() >= 4 );
        return (*this)[3];
    }

    /**
      * Returns a constant reference to the fourth coord of *this.
      *
      * The size of *this must be at least 4.
      *
      * \sa w()
      */
    const T & w() const
    {
        assert( size() >= 4 );
        return (*this)[3];
    }
};

template<typename T, typename Derived>
void VectorBase<T, Derived>::readArray( const T * src )
{
    for( int i = 0; i < size(); i++ )
        (*this)[i] = src[i];
}

template<typename T, typename Derived>
Derived & VectorBase<T, Derived>::loadZero()
{
    for( int i = 0; i < size(); i++ )
        (*this)[i] = static_cast<T>(0);
    return *static_cast<Derived *>(this);
}

template<typename T, typename Derived>
Derived & VectorBase<T, Derived>::loadRandom()
{
    for( int i = 0; i < size(); i++ )
        Util::pickRandom( (*this)[i] );
    return *static_cast<Derived *>(this);
}

template<typename T, typename Derived>
void VectorBase<T, Derived>::addition_helper
( const Derived & other, Derived * res ) const
{
    for( int i = 0; i < res->size(); i++ )
        (*res)[i] = (*this)[i] + other[i];
}

template<typename T, typename Derived>
void VectorBase<T, Derived>::substraction_helper
( const Derived & other, Derived * res ) const
{
    for( int i = 0; i < res->size(); i++ )
        (*res)[i] = (*this)[i] - other[i];
}

template<typename T, typename Derived>
void VectorBase<T, Derived>::multiplication_helper
( const T & factor, Derived * res ) const
{
    for( int i = 0; i < res->size(); i++ )
        (*res)[i] = (*this)[i] * factor;
}

template<typename T, typename Derived>
T VectorBase<T, Derived>::dot( const Derived & other ) const
{
    assert( size() == other.size() );
    T ret = Util::conj((*this)[0]) * other[0];
    for( int i = 1; i < size(); i++ )
        ret += Util::conj((*this)[i]) * other[i];
    return ret;
}

template<typename T, typename Derived>
Derived & VectorBase<T, Derived>::loadOrtho( const Derived & other )
{
    assert( other.size() >= 2 );
    resize( other.size() );

    if( size() == 2 )
    {
        x() = - Util::conj( other.y() );
        y() = Util::conj( other.x() );
        normalize();
    }
    else
    {
        /* Let us compute the crossed product of *this with a vector
        that is not too close to being colinear to *this.
        */

        /* unless the x and y coords are both close to zero, we can
        * simply take ( -y, x, 0 ) and normalize it.
        */
        if( ! Util::isNegligible( other.x(), other.z() )
         || ! Util::isNegligible( other.y(), other.z() ) )
        {
            T nm = sqrt( Util::abs2( other.x() ) + Util::abs2( other.y() ) );
            x() = - Util::conj(other.y()) / nm;
            y() = Util::conj(other.x()) / nm;
            z() = static_cast<T>(0);
        }
        /* if both x and y are close to zero, then the vector is close
        * to the z-axis, so it's far from colinear to the x-axis for instance.
        * So we take the crossed product with (1,0,0) and normalize it.
        */
        else
        {
            T nm = sqrt( Util::abs2( other.y() ) + Util::abs2( other.z() ) );
            x() = static_cast<T>(0);
            y() = - Util::conj(other.z()) / nm;
            z() = Util::conj(other.y()) / nm;
        }
        for( int i = 3; i < size(); i++ ) (*this)(i) = static_cast<T>(0);
    }
    return *static_cast<Derived*>(this);
}

/**
  * Returns factor * v (multiplication of each coord of v by factor).
  */
template<typename T, typename Derived>
Derived operator *
( const T & factor, const VectorBase<T, Derived> & v )
{
    return v * factor;
}

/**
  * Dot product
  *
  * If T is std::complex, the dot product is hermitian, i.e.
  * the coords of \a v1 get complex-conjugated in the formula.
  */
template<typename T, typename Derived>
T dot
( const VectorBase<T, Derived> & v1, const Derived & v2 )
{
    return v1.dot(v2);
}

/**
  * Cross product.
  *
  * Of course, \a v1 and \a v2 must have size exactly 3.
  */
template<typename T, typename Derived>
Derived cross
( const VectorBase<T, Derived> & v1, const Derived & v2 )
{
    return v1.cross(v2);
}

/**
  * Allows to print a vector by simply doing
  * @code
    cout << myvector << endl;
  * @endcode
  */
template<typename T, typename Derived>
std::ostream & operator <<
( std::ostream & s, const VectorBase<T, Derived> & v )
{
    s << "(" << v(0);
    for( int i = 1; i < v.size(); i++ )
        s << ", " << v( i );
    s << ")";
    return s;
}

} // namespace Eigen

#endif // EIGEN_VECTORBASE_H
