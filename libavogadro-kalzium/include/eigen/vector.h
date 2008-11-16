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

/** \file vector.h
  * \brief Vector and VectorX class templates
  */

#ifndef EIGEN_VECTOR_H
#define EIGEN_VECTOR_H

#include "vectorbase.h"

namespace Eigen
{

/** \ingroup fixedsize
  *
  * \ingroup vectors
  *
  * \brief Fixed-size vector.
  *
  * A class for fixed-size vectors (for linear algebra).
  * Thus, a Vector<T,Size> is the same
  * as a T[Size] array, except that it has convenient operators and methods
  * for basic vector math.
  *
  * The template parameter T is the type of the coords of the vector.
  * It can be any type representing either real or complex numbers.
  * The template parameter Size is the size of the vector (number of coords).
  * The following typedefs are provided to cover the usual cases:
  * @code
    typedef Vector<double, 2>               Vector2d;
    typedef Vector<double, 3>               Vector3d;
    typedef Vector<double, 4>               Vector4d;
    typedef Vector<float,  2>               Vector2f;
    typedef Vector<float,  3>               Vector3f;
    typedef Vector<float,  4>               Vector4f;
    typedef Vector<std::complex<double>, 2> Vector2cd;
    typedef Vector<std::complex<double>, 3> Vector3cd;
    typedef Vector<std::complex<double>, 4> Vector4cd;
    typedef Vector<std::complex<float>,  2> Vector2cf;
    typedef Vector<std::complex<float>,  3> Vector3cf;
    typedef Vector<std::complex<float>,  4> Vector4cf;
  * @endcode
  *
  * If you prefer dynamic-size vectors (they are slower), see the VectorX
  * class template, which provides exactly the same functionality and API
  * in dynamic-size version.
  *
  * The Vector class template provides all the usual operators and methods
  * to manipulate vectors.
  *
  * Here are some examples of usage of Vector:
  * @code
    using namespace Eigen;
    using namespace std; // we'll use cout for outputting vectors

    Vector3d vec1( -1.1, 2.9, 4.3 ); // construct vector vec1 with given coords
    double array[3] = { 2.4, 3.1, -0.7 };
    Vector3d vec2( array ); // reads the coords of vec2 from array2

    vec1 += vec2; // computes the coord-wise sum vec1 + vec2, stores it in vec1
    vec1 = vec1 - vec2; // there are also non-assignment operators
    vec1 = 0.9 * vec1 + vec2 / 2.6; // you can also multiply/divide by numbers

    vec1.x() = vec2.y() // read-write access to the x,y,z,w coords
    vec1(2) = -1.4; // Stores the value -1.4 in coord 2 of vec1.
    vec1.z() = -1.4; // equivalent to the previous line

    cout << vec1 << endl; // outputs vec1
    cout << "norm of vec1: " << vec1.norm() << endl;
    cout << cross( vec1, vec2 ) << endl; // cross-product
  * @endcode
  */
template< typename T, int Size >
class Vector : public VectorBase< T, Vector<T, Size> >
{
    friend class VectorBase< T, Vector<T, Size> >;
    typedef class VectorBase< T, Vector<T, Size> > Base;

private:

    /** \internal
      * Returns false. A Vector<T,Size> doesn't have dynamic size.
      */
    bool _hasDynamicSize() const
    { return false; }

    /** \internal
      * Returns the size of the vector.
      */
    int _size() const
    { return Size; }

    /** \internal
      * Does nothing. A Vector<T,Size> can't be resized.
      *
      * if newsize != size(), a debug message is generated.
      */
    void _resize( int size ) const
    { assert( size == this->size() ); }

public:

    /**
      * Default constructor. Constructs a vector with uninitialized coords.
      */
    Vector() {}

    /**
      * Convenience constructor provided for API homogeneity with VectorX.
      * The unused_size argument is not used.
      */
    explicit Vector( int unused_size )
    { assert( unused_size == this->size() ); }

    /**
      * Copy constructor.
      */
    Vector( const Vector &v )
    {
        readArray( v.array() );
    }

    /**
      * Constructor reading the coords from an array.
      */
    Vector( const T *array )
    {
        readArray( array );
    }

    /**
      * Convenience constructor provided for API homogeneity with VectorX.
      * Constructor reading the coords from an array.
      * The unused_size argument is not used.
      */
    Vector( int unused_size, const T *array )
    {
        assert( unused_size == this->size() );
        readArray( array );
    }

    /**
      * Convenience constructor for vectors of size 2.
      */
    Vector( T x, T y )
    {
        assert( this->size() == 2 );
        this->x() = x;
        this->y() = y;
    }

    /**
      * Convenience constructor for vectors of size 3.
      */
    Vector( T x, T y, T z )
    {
        assert( this->size() == 3 );
        this->x() = x;
        this->y() = y;
        this->z() = z;
    }

    /**
      * Convenience constructor for vectors of size 4.
      */
    Vector( T x, T y, T z, T w )
    {
        assert( this->size() == 4 );
        this->x() = x;
        this->y() = y;
        this->z() = z;
        this->w() = w;
    }

    Vector & operator = ( const Vector & other )
    { return Base::operator = ( other ); }

    Vector & operator += ( const Vector & other )
    { return Base::operator += ( other ); }

    Vector & operator -= ( const Vector & other )
    { return Base::operator -= ( other ); }

    Vector & operator *=( const T & factor )
    { return Base::operator *= ( factor ); }

    Vector & operator /=( const T & factor )
    { return Base::operator /= ( factor ); }

protected:

    /**
      * The vector's array of coordinates.
      */
    T m_array[Size];
};

/** \ingroup dynamicsize
  *
  * \ingroup vectors
  *
  * \brief Dynamic-size vector
  *
  * A class for dynamic-size vectors (for linear algebra).
  *
  * The template parameter T is the type of the coords of the vector.
  * It can be any type representing either real or complex numbers.
  * The following typedefs are provided to cover the usual cases:
  * @code
    typedef VectorX<double>                 VectorXd;
    typedef VectorX<float>                  VectorXf;
    typedef VectorX< std::complex<double> > VectorXcd;
    typedef VectorX< std::complex<float> >  VectorXcf;
  * @endcode
  *
  * If you prefer fixed-size vectors (they are faster), see the Vector
  * class template, which provides exactly the same functionality and API
  * in fixed-size version.
  *
  * The VectorX class template provides all the usual operators and methods
  * to manipulate vectors.
  *
  * Here are some examples of usage of VectorX:
  * @code
    using namespace Eigen;
    using namespace std; // we'll use cout for outputting vectors

    double array1[3] = { -1.1, 2.9, 4.3 };
    VectorXd vec1( 3, array1 ); // construct vector vec1 from array array1

    VectorXd vec2( 3 ); // construct a new uninitialized vector of size 3
    double array2[3] = { 2.4, 3.1, -0.7 };
    vec2.readArray( array2); // reads the coords of vec2 from array2

    vec1 += vec2; // computes the coord-wise sum vec1 + vec2, stores it in vec1
    vec1 = vec1 - vec2; // there are also non-assignment operators
    vec1 = 0.9 * vec1 + vec2 / 2.6; // you can also multiply/divide by numbers

    VectorXd vec3(5); // construct a new uninitialized vector of size 5
    vec3 = vec1; // Resizes vec3 to size 3, copies vec1 into vec3

    vec1(2) = -1.4; // Stores the value -1.4 in coord 2 of vec1.
    cout << vec1 << endl;
    cout << "norm of vec1: " << vec1.norm() << endl;
  * @endcode
  */
template <typename T>
class VectorX : public VectorBase< T, VectorX<T> >
{
    friend class VectorBase< T, VectorX<T> >;
    typedef class VectorBase< T, VectorX<T> > Base;


private:

    /** \internal
      * Small helper function for the constructors and the _resize method
      */
    void init( int size )
    {
       assert( size >= 1 );
       m_size = size;
       m_array  = new T[_size()];
    }

    /** \internal
      * Returns true. A VectorX has dynamic size.
      */
    bool _hasDynamicSize() const
    { return true; }

    /** \internal
      * Returns the size of the vector.
      */
    int _size() const
    { return m_size; }

    /** \internal
      * Resizes the vector.
      */
    void _resize( int size );

public:

    /**
      * Copy constructor
      */
    VectorX( const VectorX & other )
    {
        init( other._size() );
        readArray( other.array() );
    }

    /**
      * Constructs a vector with given size and uninitialized coords.
      * The default value sor size is 1.
      */
    explicit VectorX( int size = 1 )
    { init( size ); }

    /**
      * Constructs a vector with given size and reads its coord from the array.
      */
    VectorX( int size, const T * array )
    {
        init( size );
        readArray( array );
    }

    ~VectorX()
    { delete[] m_array; }

    VectorX & operator = ( const VectorX & other )
    { return Base::operator = ( other ); }

    VectorX & operator += ( const VectorX & other )
    { return Base::operator += ( other ); }

    VectorX & operator -= ( const VectorX & other )
    { return Base::operator -= ( other ); }

    VectorX & operator *=( const T & factor )
    { return Base::operator *= ( factor ); }

    VectorX & operator /=( const T & factor )
    { return Base::operator /= ( factor ); }

protected:

    /**
      * The size (dimension) of the vector
      */
    int m_size;

    /**
      * The vector's array of coordinates.
      */
    T *m_array;

};

template<typename T>
void VectorX<T>::_resize( int size )
{
    assert( size >= 1 );
    if( size == _size() ) return;
    if( size > _size() )
    {
        delete[] m_array;
        m_array  = new T[size];
    }
    m_size = size;
}

EIGEN_MAKE_FIXEDSIZE_TYPEDEFS(Vector)
EIGEN_MAKE_DYNAMICSIZE_TYPEDEFS(VectorX)

}

#endif // EIGEN_VECTOR_H
