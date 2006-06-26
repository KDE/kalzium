/***************************************************************************
    copyright            : (C) 2006 by Benoit Jacob
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "kalziumglhelperclasses.h"
#include <math.h>

#ifdef USE_FPS_COUNTER
#include <QTime>
#endif

template<class T> bool GLVector3<T>::approx_equal(
	FLOAT a, FLOAT b, FLOAT precision )
{
	FLOAT abs_a = GLFABS( a );
	FLOAT abs_b = GLFABS( b );

	FLOAT max_abs;
	if( abs_a < abs_b )
		max_abs = abs_b;
	else
		max_abs = abs_a;
	return( GLFABS( a - b ) <= precision * max_abs );
}

template<class T> void GLVector3<T>::construct_ortho_basis_given_first_vector(
	GLVector3<T> &v, GLVector3<T> &w )
{
	if( norm() == 0 ) return;
	
	// let us first make a normalized copy of *this
	GLVector3<T> u = *this;
	u.normalize();

	// first we want to set v to be non-colinear to u

	v = u;

	if( ! approx_equal( v.x, v.y, 0.1 ) )
	{
		FLOAT tmp = v.x;
		v.x = v.y;
		v.y = tmp;
	}
	else if( ! approx_equal( v.y, v.z, 0.1 ) )
	{
		FLOAT tmp = v.z;
		v.z = v.y;
		v.y = tmp;
	}
	else // the 3 coords of v are approximately equal
	{    // which implies that v is not colinear to (0,0,1)
		v = GLVector3<T>( 0, 0, 1 );
	}

	// now, v is not colinear to u. We compute its dot product with u
	FLOAT u_dot_v = u.x * v.x + u.y * v.y + u.z * v.z;

	// now we change v so that it becomes orthogonal to u
	v.x -= u.x * u_dot_v;
	v.y -= u.y * u_dot_v;
	v.z -= u.z * u_dot_v;

	// now that u and v are orthogonal, w can be constructed as
	// their crossed product
	w.x = u.y * v.z - u.z * v.y;
	w.y = u.z * v.x - u.x * v.z;
	w.z = u.x * v.y - u.y * v.x;
}

GLColor::GLColor()
{
}

GLColor::GLColor( GLfloat red, GLfloat green, GLfloat blue,
                  GLfloat alpha )
{
	m_red = red;
	m_green = green;
	m_blue = blue;
	m_alpha = alpha;
}

GLColor& GLColor::operator=( const GLColor& other )
{
	m_red = other.m_red;
	m_green = other.m_green;
	m_blue = other.m_blue;
	m_alpha = other.m_alpha;

	return *this;
}

void GLColor::apply()
{
	glColor4fv( reinterpret_cast<GLfloat *>( this ) );
}

void GLColor::applyAsMaterials()
{
	GLfloat ambientColor [] = { m_red / 2, m_green / 2, m_blue / 2,
	                            m_alpha };
	GLfloat diffuseColor [] = { m_red, m_green, m_blue, m_alpha };
	GLfloat specularColor [] = { (2.0 + m_red) / 3, (2.0 + m_green) / 3,
		(2.0 + m_blue) / 3, m_alpha };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
}

GLVertexArray::GLVertexArray()
{
	allocateId();
	m_mode = GL_TRIANGLE_STRIP;
	m_vertexBuffer = 0;
	m_normalBuffer = 0;
	m_indexBuffer = 0;
	m_isInitialized = false;
}

GLVertexArray::~GLVertexArray()
{
	if( m_indexBuffer ) delete [] m_indexBuffer;
	if( m_vertexBuffer ) delete [] m_vertexBuffer;
	if( m_normalBuffer ) delete [] m_normalBuffer;
}

void GLVertexArray::allocateId()
{
	static int counter = 0;
	m_id = counter++;
}


void GLVertexArray::select()
{
	static int selected_id = -1;

	if( selected_id == m_id ) return;

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, m_vertexBuffer );
	glNormalPointer( GL_FLOAT, 0, m_normalBuffer );
	selected_id = m_id;
}

void GLVertexArray::draw()
{
	if( ! m_isInitialized ) return;
	select();
	glDrawElements( m_mode, m_indexCount,
		GL_UNSIGNED_SHORT, m_indexBuffer );
}

bool GLVertexArray::allocateBuffers()
{
	if( m_vertexCount > 65536 ) return false;

	m_isInitialized = false;

	if( m_indexBuffer )
	{
		delete [] m_indexBuffer;
		m_indexBuffer = 0;
	}
	if( m_vertexBuffer )
	{
		delete [] m_vertexBuffer;
		m_vertexBuffer = 0;
	}
	if( m_normalBuffer ) 
	{
		delete [] m_normalBuffer;
		m_normalBuffer = 0;
	}

	m_vertexBuffer = new GLVector3<GLfloat>[m_vertexCount];
	if( ! m_vertexBuffer ) return false;
	m_normalBuffer = new GLVector3<GLfloat>[m_vertexCount];
	if( ! m_normalBuffer ) return false;
	m_indexBuffer = new unsigned short[m_indexCount];
	if( ! m_indexBuffer ) return false;

	return true;
}

GLSphere::GLSphere()
	: GLVertexArray()
{
	m_detail = 0;
	m_radius = -1.0;
}

unsigned short GLSphere::indexOfVertex( int strip, int column, int row)
{
	return ( row + ( 3 * m_detail + 1 ) * ( column + m_detail * strip ) );
}

void GLSphere::computeVertex( int strip, int column, int row)
{
	strip %= 5;
	int next_strip = (strip + 1) % 5;

	GLVector3<GLfloat> *vertex =
		&m_vertexBuffer[ indexOfVertex( strip, column, row ) ];

	GLVector3<GLfloat> *normal =
		&m_normalBuffer[ indexOfVertex( strip, column, row ) ];

	const GLfloat phi = ( 1 + sqrt(5) ) / 2;

	const GLVector3<GLfloat> northPole( 0, 1, phi );
	const GLVector3<GLfloat> northVertex[5] = {
		GLVector3<GLfloat>( 0, -1, phi ),
		GLVector3<GLfloat>( phi, 0, 1 ),
		GLVector3<GLfloat>( 1, phi, 0 ),
		GLVector3<GLfloat>( -1, phi, 0 ),
		GLVector3<GLfloat>( -phi, 0, 1 ) };
	const GLVector3<GLfloat> southVertex[5] = {
		GLVector3<GLfloat>( -1, -phi, 0 ),
		GLVector3<GLfloat>( 1, -phi, 0 ),
		GLVector3<GLfloat>( phi, 0, -1 ),
		GLVector3<GLfloat>( 0, 1, -phi ),
		GLVector3<GLfloat>( -phi, 0, -1 )
		 };
	const GLVector3<GLfloat> southPole( 0, -1, -phi );

	const GLVector3<GLfloat> *v0, *v1, *v2;
	int  c1, c2;

	if( row >= 2 * m_detail && column == 0 )
	{
		strip--;
		if( strip < 0 ) strip += 5;
		next_strip--;
		if( next_strip < 0 ) next_strip += 5;
		column = m_detail;
	}

	if( row  <= m_detail )
	{
		v0 = &northVertex[strip];
		v1 = &northPole;
		v2 = &northVertex[next_strip];
		c1 = m_detail - row;
		c2 = column;
	}
	else if( row >= 2 * m_detail )
	{
		v0 = &southVertex[next_strip];
		v1 = &southPole;
		v2 = &southVertex[strip];
		c1 = row - 2 * m_detail;
		c2 = m_detail - column;
	}
	else if( row <= m_detail + column )
	{
		v0 = &northVertex[next_strip];
		v1 = &southVertex[next_strip];
		v2 = &northVertex[strip];
		c1 = row - m_detail;
		c2 = m_detail - column;
	}
	else
	{
		v0 = &southVertex[strip];
		v1 = &southVertex[next_strip];
		v2 = &northVertex[strip];
		c1 = column;
		c2 = 2 * m_detail - row;
	}

	GLfloat u1 = GLfloat(c1) / m_detail;
	GLfloat u2 = GLfloat(c2) / m_detail;

	vertex->x = v0->x + u1 * (v1->x - v0->x) + u2 * (v2->x - v0->x);
	vertex->y = v0->y + u1 * (v1->y - v0->y) + u2 * (v2->y - v0->y);
	vertex->z = v0->z + u1 * (v1->z - v0->z) + u2 * (v2->z - v0->z);

	vertex->normalize();

	*normal = *vertex;

	vertex->x *= m_radius;
	vertex->y *= m_radius;
	vertex->z *= m_radius;
}


void GLSphere::initialize()
{
	if( m_detail < 1 ) return;
	m_vertexCount = ( 3 * m_detail + 1 ) * ( 5 * m_detail + 1 );
	m_indexCount = (2 * ( 2 * m_detail + 1 ) + 2 ) * 5 * m_detail;

	if( ! allocateBuffers() ) return;

	for( int strip = 0; strip < 5; strip++ )
	for( int column = 1; column < m_detail; column++ )
	for( int row = column; row <= 2 * m_detail + column; row++ )
		computeVertex( strip, column, row );

	for( int strip = 1; strip < 5; strip++ )
	for( int row = 0; row <= 3 * m_detail; row++ )
		computeVertex( strip, 0, row );

	for( int row = 0; row <= 2 * m_detail; row++ )
		computeVertex( 0, 0, row );

	for( int row = m_detail; row <= 3 * m_detail; row++ )
		computeVertex( 4, m_detail, row );

	unsigned int i = 0;
	for( int strip = 0; strip < 5; strip++ )
	for( int column = 0; column < m_detail; column++ )
	{
		int row = column;
		m_indexBuffer[i++] = indexOfVertex( strip, column, row );
		for( ; row <= 2 * m_detail + column; row++ )
		{
			m_indexBuffer[i++] =
				indexOfVertex( strip, column, row );
			m_indexBuffer[i++] =
				indexOfVertex( strip, column + 1, row + 1 );
		}
		m_indexBuffer[i++] = indexOfVertex( strip, column + 1,
			2 * m_detail + column + 1);
	}

	m_isInitialized = true;
}

void GLSphere::setup( int detail, GLfloat radius )
{
	if( detail == m_detail && radius == m_radius ) return;
	m_detail = detail;
	m_radius = radius;
	allocateId();
	initialize();
}

void GLSphere::drawScaled( GLfloat radius )
{
	const GLfloat precision	= 0.001;

	if( GLVector3<GLfloat>::approx_equal( radius, m_radius, precision ) )
	{
		draw();
		return;
	}

	GLfloat factor = radius / m_radius;
	glEnable( GL_NORMALIZE );
	glPushMatrix();
	glScalef( factor, factor, factor );
	draw();
	glPopMatrix();
	glDisable( GL_NORMALIZE );
}

GLCylinder::GLCylinder()
	: GLVertexArray()
{
	m_mode = GL_QUAD_STRIP;
	m_faces = 0;
	m_radius = -1.0;
}

void GLCylinder::setup( int faces, GLfloat radius )
{
	if( faces == m_faces && radius == m_radius ) return;
	m_faces = faces;
	m_radius = radius;
	allocateId();
	initialize();
}

void GLCylinder::initialize()
{
	if( m_faces < 3 ) return;

	m_vertexCount = 2 * m_faces + 2; // we will use a redundant vertex array
	m_indexCount = 0; // we won't use it.

	if( ! allocateBuffers() ) return;

	for( int i = 0; i <= m_faces; i++)
	{
		float angle = 2 * M_PI * i / m_faces;
		float x = cosf( angle );
		float y = sinf( angle );

		m_normalBuffer[ 2 * i ].x = x;
		m_normalBuffer[ 2 * i ].y = y;
		m_normalBuffer[ 2 * i ].z = 0.0;

		m_vertexBuffer[ 2 * i ].x = x * m_radius ;
		m_vertexBuffer[ 2 * i ].y = y * m_radius;
		m_vertexBuffer[ 2 * i ].z = 1.0;

		m_normalBuffer[ 2 * i + 1].x = x;
		m_normalBuffer[ 2 * i + 1].y = y;
		m_normalBuffer[ 2 * i + 1].z = 0.0;

		m_vertexBuffer[ 2 * i + 1].x = x * m_radius;
		m_vertexBuffer[ 2 * i + 1].y = y * m_radius ;
		m_vertexBuffer[ 2 * i + 1].z = 0.0;
	}

	m_isInitialized = true;
}

void GLCylinder::draw()
{
	if ( ! m_isInitialized ) return;
	select();
	glDrawArrays( m_mode, 0, m_vertexCount );
}
