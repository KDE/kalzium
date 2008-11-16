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

/** \file util.h
  * \brief Internal file
  */

#ifndef EIGEN_UTIL_H
#define EIGEN_UTIL_H

#include <cstdlib>
#include <cmath>
#include <complex>
#include <iostream>
#include <cassert>

#ifdef __GNUC__
# if __GNUC__>=4
#  define EIGEN_WITH_GCC_4_OR_LATER
# endif
#endif

namespace Eigen
{

namespace Util
{

/** Stores in x a random float between -1.0 and 1.0 */
inline void pickRandom( float & x )
{
    x = 2.0f * rand() / RAND_MAX - 1.0f;
}

/** Stores in x a random double between -1.0 and 1.0 */
inline void pickRandom( double & x )
{
    x = 2.0 * rand() / RAND_MAX - 1.0;
}

/** Stores in the real and imaginary parts of x
  * random values between -1.0 and 1.0 */
template<typename T> void pickRandom( std::complex<T> & x )
{
#ifdef EIGEN_WITH_GCC_4_OR_LATER
    pickRandom( x.real() );
    pickRandom( x.imag() );
#else // workaround by David Faure for MacOS 10.3 and GCC 3.3, commit 630812
    T r = x.real();
    T i = x.imag();
    pickRandom( r );
    pickRandom( i );
    x = std::complex<T>(r,i);
#endif
}

template<typename T> inline T epsilon() { return static_cast<T>(0); }
template<> inline float epsilon<float>() { return 1e-5f; }
template<> inline double epsilon<double>() { return 1e-11; }
template<> inline std::complex<float> epsilon<std::complex<float> >()
{ return static_cast<std::complex<float> >(1e-5f); }
template<> inline std::complex<double> epsilon<std::complex<double> >()
{ return static_cast<std::complex<double> >(1e-11); }

inline float abs2( const float& x ) { return x * x; }
inline double abs2( const double& x ) { return x * x; }
inline float abs2( const std::complex<float>& x ) { return norm(x); }
inline double abs2( const std::complex<double>& x ) { return norm(x); }

/**
  * overloaded function that returns the complex conjugate of a float x.
  * Of course, as floats are reals, this is just the trivial function
  * returning x. But that'll become useful to handle matrices of complex
  * numbers.
  */
inline float conj( const float& x )
{
    return x;
}

/**
  * overloaded function that returns the complex conjugate of a double x.
  * Of course, as doubles are reals, this is just the trivial function
  * returning x. But that'll become useful to handle matrices of complex
  * numbers.
  */
inline double conj( const double& x )
{
    return x;
}

/**
  * overloaded function that returns the complex conjugate of a complex x.
  */
template<typename T>
std::complex<T> conj( const std::complex<T> & x )
{
    return std::conj(x);
}

/**
  * Short version: returns true if the absolute value of \a a is much smaller
  * than that of \a b, false otherwise.
  *
  * Long version: returns ( abs(a) <= abs(b) * epsilon(b) ).
  *
  * This function uses the epsilon overloaded function
  * to determine what's "small".
  */
template<typename T> bool isNegligible( const T& a, const T& b )
{
    return( std::abs(a) <= std::abs(b) * std::abs(epsilon<T>()) );
}

/**
  * Returns true if a is very close to b, false otherwise.
  *
  * In other words: returns abs( a - b ) <= min( abs(a), abs(b) ) * epsilon(b).
  *
  * @param a,b can be real or complex numbers (std::complex).
  */
template<typename T> bool isApprox( const T& a, const T& b )
{
    return( std::abs( a - b )
         <= std::min( std::abs(a), std::abs(b) ) * epsilon<T>() );
}

#define EIGEN_MAKE_FIXEDSIZE_TYPEDEFS(Class) \
    typedef Class<double, 2>               Class##2d;\
    typedef Class<double, 3>               Class##3d;\
    typedef Class<double, 4>               Class##4d;\
    typedef Class<float,  2>               Class##2f;\
    typedef Class<float,  3>               Class##3f;\
    typedef Class<float,  4>               Class##4f;\
    typedef Class<std::complex<double>, 2> Class##2cd;\
    typedef Class<std::complex<double>, 3> Class##3cd;\
    typedef Class<std::complex<double>, 4> Class##4cd;\
    typedef Class<std::complex<float>,  2> Class##2cf;\
    typedef Class<std::complex<float>,  3> Class##3cf;\
    typedef Class<std::complex<float>,  4> Class##4cf;

#define EIGEN_MAKE_DYNAMICSIZE_TYPEDEFS(Class) \
    typedef Class<double>                 Class##d;\
    typedef Class<float>                  Class##f;\
    typedef Class< std::complex<double> > Class##cd;\
    typedef Class< std::complex<float> >  Class##cf;

} // namespace Util

} // namespace Eigen

#endif // EIGEN_UTIL_H

/** @defgroup vectors Vectors*/
/** @defgroup matrices Matrices*/
/** @defgroup fixedsize Fixed-size classes*/
/** @defgroup dynamicsize Dynamic-size classes*/
/** @defgroup solving Solving systems of equations*/
/** @defgroup regression Linear regression analysis*/
/** @defgroup ludecomp LU Decomposition*/
/** @defgroup projective Projective geometry*/
/** @defgroup internalbases Internal stuff*/

