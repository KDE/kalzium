// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * matrix2d.cc 
 *
 * Copyright (C) 2004-2007 Jean Bréfort <jean.brefort@normalesup.org>
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

#include "matrix2d.h"
#include <cmath>

namespace gcu
{

Matrix2D::Matrix2D()
{
}

Matrix2D::~Matrix2D()
{
}

Matrix2D::Matrix2D(double Angle, bool Deg)
{
	if (Deg) Angle *= (M_PI / 180.);
	x[0][0] = x[1][1] = cos(Angle);
	x[0][1] = sin(Angle);
	x[1][0] = - x[0][1];
}

Matrix2D::Matrix2D(double x11, double x12, double x21, double x22)
{
	x[0][0] = x11;
	x[0][1] = x12;
	x[1][0] = x21;
	x[1][1] = x22;
}

Matrix2D& Matrix2D::operator*(Matrix2D& cMat)
{
	static Matrix2D cMat0;
	Matrix2D m(
			x[0][0] * cMat.x[0][0] + x[0][1] * cMat.x[1][0],
			x[0][0] * cMat.x[0][1] + x[0][1] * cMat.x[1][1],
			x[1][0] * cMat.x[0][0] + x[1][1] * cMat.x[1][0],
			x[1][0] * cMat.x[0][1] + x[1][1] * cMat.x[1][1]
		);
	return cMat0 = m;
}

Matrix2D& Matrix2D::operator=(Matrix2D& cMat)
{
	x[0][0] = cMat.x[0][0];
	x[0][1] = cMat.x[0][1];
	x[1][0] = cMat.x[1][0];
	x[1][1] = cMat.x[1][1];
	return *this;
}

void Matrix2D::Transform(double &dx, double &dy)
{
	double x1 = dx * x[0][0] + dy * x[0][1];
	dy = dx * x[1][0] + dy * x[1][1];
	dx = x1;
}

}	//	namespace gcu
