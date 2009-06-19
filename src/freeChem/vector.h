// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * vector.h 
 *
 * Copyright (C) 2007-2009 Jean Bréfort <jean.brefort@normalesup.org>
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */
 
#ifndef GCU_VECTOR_H
#define GCU_VECTOR_H

#include "macros.h"

/*!\file*/
namespace gcu
{

/*!\class Vector gcu/vector.h
3d vectors class.
*/

class Vector
{
public:
/*!
Constructs a vector with all three coordinates equal to 0.
*/
	Vector ();
	Vector (double x, double y, double z);
	Vector (double d[3]);
	Vector (Vector const &v);

	Vector operator+ (Vector const &v) const;
	Vector operator- (Vector const &v) const;
	Vector operator* (double m) const;
	Vector operator/ (double d) const;
	Vector operator*= (double m);
	Vector operator/= (double d);
	double operator[] (unsigned i) const;

	double GetLength () const;
	Vector CreateOrthogonal () const;
	Vector Cross (Vector const &v) const;

GCU_PROP (double, X)
GCU_PROP (double, Y)
GCU_PROP (double, Z)
};

/*!
@param f a multiplication factor
@param v the vector to multiply

@return the product of f and v. 
*/
Vector operator* (double f, Vector const& v);

/*!\class Vector3f gcu/vector.h
Utility 3d vectors class used by the gcu::Sphere class.
*/

class Vector3f
{
public:
/*!
Constructs a vector with all three coordinates equal to 0.
*/
	Vector3f ();
/*!
@param x the x coordinate.
@param y the y coordinate.
@param z the z coordinate.

Constructs a vector given its three coordinates.
*/
	Vector3f (float x, float y, float z);
/*!
The destructor.
*/
	~Vector3f ();

/*!
*/
	void normalize ();
/*!
@param other a Vector3f to copy.

The assignment operator.
@return a new vector identical to \a other and \a this.
*/
	Vector3f &operator= (Vector3f const& other);
/*!
@param other a Vector3f to substract.

@return
*/
	Vector3f operator- (Vector3f const& other) const;
/*!
@param other a Vector3f to add.

@return the some of the two vectors.
*/
	Vector3f operator+ (Vector3f const& other) const;
/*!
@param f a real parameter.

@return the vector multiplied by \a f.
*/
	Vector3f operator* (float f) const;

private:
/*!\fn Setx (float x)
@param x the new value for the first coordinate.

Sets the first coordinate of the vector.
*/
/*!\fn Getx()
@return the first coordinate of the vector.
*/
/*!\fn GetRefx()
@return the first coordinate of the vector as a reference.
*/
	GCU_PROP (float, x);
/*!\fn Sety (float y)
@param y the new value for the second coordinate.

Sets the second coordinate of the vector.
*/
/*!\fn Gety()
@return the second coordinate of the vector.
*/
/*!\fn GetRefy()
@return the second coordinate of the vector as a reference.
*/
	GCU_PROP (float, y);
/*!\fn Setz (float z)
@param z the new value for the third coordinate.

Sets the third coordinate of the vector.
*/
/*!\fn Getz()
@return the third coordinate of the vector.
*/
/*!\fn GetRefz()
@return the third coordinate of the vector as a reference.
*/
	GCU_PROP (float, z);
};

/*!
@param f a multiplication factor
@param v the vector to multiply

@return the product of f and v. 
*/
Vector3f operator* (float f, Vector3f const& v);

}	//namespace gcu

#endif //GCU_VECTOR_H
