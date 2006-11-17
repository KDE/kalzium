/***************************************************************************
    copyright            : (C) 2006 by Benoit Jacob <jacob@math.jussieu.fr>
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

using namespace OpenBabel;
using namespace Eigen;

#include<iostream>
using namespace std;

namespace KalziumGLHelpers
{

MolStyle::MolStyle( BondStyle bondStyle, AtomStyle atomStyle,
	double singleBondRadius,
	bool renderMultipleBonds,
	double multipleBondRadius,
	double multipleBondShift,
	double atomRadiusFactor )
{
	m_bondStyle = bondStyle;
	m_atomStyle = atomStyle;
	m_singleBondRadius = singleBondRadius;
	m_renderMultipleBonds = renderMultipleBonds;
	m_multipleBondRadius = multipleBondRadius;
	m_multipleBondShift = multipleBondShift;
	m_atomRadiusFactor = atomRadiusFactor;
}

double MolStyle::getAtomRadius( int atomicNumber )
{
	switch( m_atomStyle )
	{
		case ATOMS_USE_FIXED_RADIUS:
			return m_atomRadiusFactor;
		case ATOMS_USE_VAN_DER_WAALS_RADIUS:
			return m_atomRadiusFactor
				* etab.GetVdwRad( atomicNumber );
		default: return 0;
	}
}

Color::Color( GLfloat red, GLfloat green, GLfloat blue,
                  GLfloat alpha )
{
	m_red = red;
	m_green = green;
	m_blue = blue;
	m_alpha = alpha;
}

Color::Color( const OBAtom* atom )
{
	std::vector<double> rgb = etab.GetRGB( atom->GetAtomicNum() );
	m_red = rgb[0];
	m_green = rgb[1];
	m_blue = rgb[2];
	m_alpha = 1.0;
}

void Color::applyAsMaterials()
{
	GLfloat ambientColor [] = { m_red / 2, m_green / 2, m_blue / 2,
	                            m_alpha };
	GLfloat diffuseColor [] = { m_red, m_green, m_blue, m_alpha };

	float s = ( 0.5 + fabsf( m_red - m_green )
		+ fabsf( m_blue - m_green ) + fabsf( m_blue - m_red ) ) / 4.0;

	float t = 1.0 - s;

	GLfloat specularColor [] = { s + t * m_red,
		s + t * m_green,
		s + t * m_blue,
		m_alpha };

	glMaterialfv( GL_FRONT, GL_AMBIENT, ambientColor );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuseColor );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specularColor );
	glMaterialf( GL_FRONT, GL_SHININESS, 50.0 );
}

VertexArray::VertexArray( GLenum mode,
	bool hasIndexBuffer,
	bool hasSeparateNormalBuffer )
{
	m_mode = mode;
	m_vertexBuffer = 0;
	m_normalBuffer = 0;
	m_indexBuffer = 0;
	m_displayList = 0;
	m_hasIndexBuffer = hasIndexBuffer;
	m_hasSeparateNormalBuffer = hasSeparateNormalBuffer;
	m_isValid = false;
}

VertexArray::~VertexArray()
{
	freeBuffers();
	if( m_displayList )
		glDeleteLists( m_displayList, 1 );
}

void VertexArray::freeBuffers()
{
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
	if( m_normalBuffer && m_hasSeparateNormalBuffer ) 
	{
		delete [] m_normalBuffer;
		m_normalBuffer = 0;
	}
}

bool VertexArray::allocateBuffers()
{
	if( m_vertexCount > 65536 ) return false;

	freeBuffers();

	m_vertexBuffer = new Vector3f[m_vertexCount];
	if( ! m_vertexBuffer ) return false;
	
	if( m_hasSeparateNormalBuffer )
	{
		m_normalBuffer = new Vector3f[m_vertexCount];
		if( ! m_normalBuffer ) return false;
	}
	else m_normalBuffer = m_vertexBuffer;

	if( m_hasIndexBuffer )
	{
		m_indexBuffer = new unsigned short[m_indexCount];
		if( ! m_indexBuffer ) return false;
	}

	return true;
}

void VertexArray::do_draw()
{
	glVertexPointer( 3, GL_FLOAT, 0, m_vertexBuffer );
	glNormalPointer( GL_FLOAT, 0, m_normalBuffer );
	if( m_hasIndexBuffer )
		glDrawElements( m_mode, m_indexCount,
			GL_UNSIGNED_SHORT, m_indexBuffer );
	else
		glDrawArrays( m_mode, 0, m_vertexCount );
}

void VertexArray::compileDisplayList()
{
#ifdef USE_DISPLAY_LISTS
	if( ! m_displayList )
		m_displayList = glGenLists( 1 );
	if( ! m_displayList ) return;
	
	glNewList( m_displayList, GL_COMPILE );
	do_draw();
	glEndList();

	freeBuffers();
#endif
}

void VertexArray::initialize()
{
	m_isValid = false;
	m_vertexCount = getVertexCount();
	m_indexCount = getIndexCount();
	if( m_indexCount < 0 || m_vertexCount < 0 ) return;
	if( ! allocateBuffers() ) return;
	buildBuffers();
	compileDisplayList();
	m_isValid = true;
}

Sphere::Sphere()
	: VertexArray( GL_TRIANGLE_STRIP, true, false )
{
	m_detail = 0;
}

unsigned short Sphere::indexOfVertex( int strip, int column, int row)
{
	return ( row + ( 3 * m_detail + 1 ) * ( column + m_detail * strip ) );
}

void Sphere::computeVertex( int strip, int column, int row)
{
	strip %= 5;
	int next_strip = (strip + 1) % 5;

	// the index of the vertex we want to store the result in
	unsigned short index = indexOfVertex( strip, column, row );

	// reference to the vertex we want to store the result in
	Vector3f & vertex = m_vertexBuffer[ index ];

	// the "golden ratio", useful to construct an icosahedron
	const float phi = ( 1 + sqrt(5) ) / 2;

	// the 12 vertices of the icosahedron
	const Vector3f northPole( 0, 1, phi );
	const Vector3f northVertices[5] = {
		Vector3f( 0, -1, phi ),
		Vector3f( phi, 0, 1 ),
		Vector3f( 1, phi, 0 ),
		Vector3f( -1, phi, 0 ),
		Vector3f( -phi, 0, 1 ) };
	const Vector3f southVertices[5] = {
		Vector3f( -1, -phi, 0 ),
		Vector3f( 1, -phi, 0 ),
		Vector3f( phi, 0, -1 ),
		Vector3f( 0, 1, -phi ),
		Vector3f( -phi, 0, -1 )
		 };
	const Vector3f southPole( 0, -1, -phi );

	// pointers to the 3 vertices of the face of the icosahedron
	// in which we are
	const Vector3f *v0, *v1, *v2;

	// coordinates of our position inside this face.
	// range from 0 to m_detail.
	int  c1, c2;

	// first, normalize the global coords row, column
	if( row >= 2 * m_detail && column == 0 )
	{
		strip--;
		if( strip < 0 ) strip += 5;
		next_strip--;
		if( next_strip < 0 ) next_strip += 5;
		column = m_detail;
	}

	// next, determine in which face we are, and determine the coords
	// of our position inside this face
	if( row  <= m_detail )
	{
		v0 = &northVertices[strip];
		v1 = &northPole;
		v2 = &northVertices[next_strip];
		c1 = m_detail - row;
		c2 = column;
	}
	else if( row >= 2 * m_detail )
	{
		v0 = &southVertices[next_strip];
		v1 = &southPole;
		v2 = &southVertices[strip];
		c1 = row - 2 * m_detail;
		c2 = m_detail - column;
	}
	else if( row <= m_detail + column )
	{
		v0 = &northVertices[next_strip];
		v1 = &southVertices[next_strip];
		v2 = &northVertices[strip];
		c1 = row - m_detail;
		c2 = m_detail - column;
	}
	else
	{
		v0 = &southVertices[strip];
		v1 = &southVertices[next_strip];
		v2 = &northVertices[strip];
		c1 = column;
		c2 = 2 * m_detail - row;
	}

	// now, compute the actual coords of the vertex
	float u1 = static_cast<float>(c1) / m_detail;
	float u2 = static_cast<float>(c2) / m_detail;
	vertex = *v0 + u1 * ( *v1 - *v0 ) + u2 * ( *v2 - *v0 );

	// project the vertex onto the sphere
	vertex.normalize();
}

int Sphere::getVertexCount()
{
	if( m_detail < 1 ) return -1;
	return ( 3 * m_detail + 1 ) * ( 5 * m_detail + 1 );
}

int Sphere::getIndexCount()
{
	if( m_detail < 1 ) return -1;
	return (2 * ( 2 * m_detail + 1 ) + 2 ) * 5 * m_detail;
}

void Sphere::buildBuffers()
{
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
}

void Sphere::setup( int detail )
{
	if( detail == m_detail ) return;
	m_detail = detail;
	initialize();
}

void Sphere::draw( const Vector3d &center, double radius )
{
	glPushMatrix();
	glTranslated( center.x(), center.y(), center.z() );
	glScaled( radius, radius, radius );
	VertexArray::draw();
	glPopMatrix();
}

Cylinder::Cylinder()
	: VertexArray( GL_QUAD_STRIP, false, true )
{
	m_faces = 0;
}

void Cylinder::setup( int faces )
{
	if( faces == m_faces ) return;
	m_faces = faces;
	initialize();
}

int Cylinder::getVertexCount()
{
	if( m_faces < 3 ) return -1;
	return 2 * m_faces + 2;
}

void Cylinder::buildBuffers()
{
	for( int i = 0; i <= m_faces; i++ )
	{
		float angle = 2 * M_PI * i / m_faces;
		float x = cosf( angle );
		float y = sinf( angle );

		m_normalBuffer[ 2 * i ].x() = x;
		m_normalBuffer[ 2 * i ].y() = y;
		m_normalBuffer[ 2 * i ].z() = 0.0;

		m_normalBuffer[ 2 * i + 1 ] = m_normalBuffer[ 2 * i ];

		m_vertexBuffer[ 2 * i ].x() = x;
		m_vertexBuffer[ 2 * i ].y() = y;
		m_vertexBuffer[ 2 * i ].z() = 1.0;

		m_vertexBuffer[ 2 * i + 1 ].x() = x;
		m_vertexBuffer[ 2 * i + 1 ].y() = y;
		m_vertexBuffer[ 2 * i + 1 ].z() = 0.0;
	}
}

void Cylinder::draw( const Vector3d &end1, const Vector3d &end2,
	double radius, int order, double shift )
{
	// the "axis vector" of the cylinder
	Vector3d axis = end2 - end1;
	double axisNorm = axis.norm();
	if( axisNorm == 0.0 ) return;
	Vector3d axisNormalized = axis / axisNorm;
	
	Vector3d ortho1( axisNormalized.y(), -axisNormalized.x(), 0.0 );
	double ortho1Norm = ortho1.norm();
	if( ortho1Norm > 0.001 ) ortho1 /= ortho1Norm;
	else {
		ortho1 = Vector3d( 0.0,
		                   axisNormalized.z(),
		                   -axisNormalized.y() );
		ortho1.normalize();
	}
	ortho1 *= radius;

	Vector3d ortho2 = cross( axisNormalized, ortho1 );

	// construct the 4D transformation matrix
	Matrix4d matrix;

	matrix(0, 0) = ortho1(0);
	matrix(1, 0) = ortho1(1);
	matrix(2, 0) = ortho1(2);
	matrix(3, 0) = 0.0;

	matrix(0, 1) = ortho2(0);
	matrix(1, 1) = ortho2(1);
	matrix(2, 1) = ortho2(2);
	matrix(3, 1) = 0.0;

	matrix(0, 2) = axis(0);
	matrix(1, 2) = axis(1);
	matrix(2, 2) = axis(2);
	matrix(3, 2) = 0.0;

	matrix(0, 3) = end1(0);
	matrix(1, 3) = end1(1);
	matrix(2, 3) = end1(2);
	matrix(3, 3) = 1.0;

	//now we can do the actual drawing !
	glPushMatrix();
	glMultMatrixd( matrix.array() );
	if( order == 1 ) VertexArray::draw();
	else
	{
		double angleOffset = 0.0;
		if( order >= 3 )
		{
			if( order == 3 ) angleOffset = 90.0;
			else angleOffset = 22.5;
		}
		
		double displacementFactor = shift / radius;
		for( int i = 0; i < order; i++)
		{
			glPushMatrix();
			glRotated( angleOffset + 360.0 * i / order,
			           0.0, 0.0, 1.0 );
			glTranslated( displacementFactor, 0.0, 0.0 );
			VertexArray::draw();
			glPopMatrix();
		}
	}
	glPopMatrix();
}

CharRenderer::CharRenderer()
{
	m_texture = 0;
	m_displayList = 0;
}

CharRenderer::~CharRenderer()
{
	if( m_texture ) glDeleteTextures( 1, &m_texture );
	if( m_displayList ) glDeleteLists( m_displayList, 1 );
}

bool CharRenderer::initialize( QChar c, const QFont &font, GLenum textureTarget )
{
	if( m_displayList ) return true;
	
	QFontMetrics fontMetrics ( font );
	m_width = fontMetrics.width( c );
	m_height = fontMetrics.height();
	if( m_width <= 0 || m_height <= 0 ) return false;
	QImage image( m_width, m_height, QImage::Format_RGB32 );
	
	QPainter painter;
	painter.begin( &image );
	painter.setFont( font );
	painter.setRenderHint( QPainter::TextAntialiasing );
	painter.setBackground( Qt::black );
	painter.eraseRect( image.rect() );
	painter.setPen( Qt::blue );
	painter.drawText ( 0, 0, m_width, m_height, Qt::AlignBottom, c );
	painter.end();

	GLubyte *bitmap = new GLubyte[ m_width * m_height ];
	if( ! bitmap ) return false;

	for( int j = m_height - 1, n = 0; j >= 0; j-- )
	for( int i = 0; i < m_width; i++, n++ )
	{
		bitmap[n] = qBlue( image.pixel( i, j ) );
	}

	glGenTextures( 1, &m_texture );
	if( ! m_texture ) return false;

	glBindTexture( textureTarget, m_texture );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexImage2D(
		textureTarget,
		0,
		GL_ALPHA,
		m_width,
		m_height,
		0,
		GL_ALPHA,
		GL_UNSIGNED_BYTE,
		bitmap );
	glTexParameteri( textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	delete [] bitmap;

	m_displayList = glGenLists(1);
	if( ! m_displayList ) return false;

	glNewList( m_displayList, GL_COMPILE );
	glBindTexture( textureTarget, m_texture );
	glBegin( GL_QUADS );
	glTexCoord2f( 0, 0);
	glVertex2f( 0 , 0 );
	glTexCoord2f( 1, 0);
	glVertex2f( m_width , 0 );
	glTexCoord2f( 1, 1);
	glVertex2f( m_width, m_height );
	glTexCoord2f( 0, 1);
	glVertex2f( 0 , m_height );
	glEnd();
	glTranslatef( m_width, 0, 0 );
	glEndList();
	return true;
}

TextRenderer::TextRenderer()
{
	m_glwidget = 0;
	m_isBetweenBeginAndEnd = false;
	m_textureTarget = GL_TEXTURE_2D;
}

TextRenderer::~TextRenderer()
{
	QHash<QChar, CharRenderer *>::iterator i = m_charTable.begin();
	while( i != m_charTable.end() )
	{
		delete i.value();
		i = m_charTable.erase(i);
        }
}

void TextRenderer::setup( const QGLWidget *glwidget, const QFont &font )
{
	if( m_glwidget ) return;
	m_glwidget = glwidget;
	m_font = font;
}

void TextRenderer::do_begin()
{
	m_wasEnabled_LIGHTING = glIsEnabled( GL_LIGHTING );
	m_wasEnabled_FOG = glIsEnabled( GL_FOG );
	m_wasEnabled_textureTarget
		= glIsEnabled( m_textureTarget );
	m_wasEnabled_BLEND = glIsEnabled( GL_BLEND );
	m_wasEnabled_DEPTH_TEST = glIsEnabled( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_FOG );
	glEnable( m_textureTarget );
	glEnable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0, m_glwidget->width(), 0, m_glwidget->height(), -1, 1 );
	glMatrixMode( GL_MODELVIEW );
}

void TextRenderer::begin()
{
	if( ! m_glwidget ) return;
	if( m_isBetweenBeginAndEnd ) return;
	m_isBetweenBeginAndEnd = true;
	do_begin();
}

void TextRenderer::do_end()
{
	if( ! m_wasEnabled_textureTarget )
		glDisable( m_textureTarget );
	if( ! m_wasEnabled_BLEND ) glDisable( GL_BLEND );
	if( m_wasEnabled_DEPTH_TEST ) glEnable( GL_DEPTH_TEST );
	if( m_wasEnabled_LIGHTING ) glEnable( GL_LIGHTING );
	if( m_wasEnabled_FOG ) glEnable( GL_FOG );
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}

void TextRenderer::end()
{
	if( m_isBetweenBeginAndEnd ) do_end();
	m_isBetweenBeginAndEnd = false;
}

void TextRenderer::print( int x, int y, const QString &string )
{
	if( ! m_glwidget ) return;
	if( string.isEmpty() ) return;

	if( ! m_isBetweenBeginAndEnd ) do_begin();
	
	glPushMatrix();
	glLoadIdentity();
	glTranslatef( x, y, 0 );
	for( int i = 0; i < string.size(); i++ )
	{
		if( m_charTable.contains( string[i] ) )
			m_charTable.value( string[i] )->draw();
		else
		{
			CharRenderer *c = new CharRenderer;
			if( c->initialize( string[i], m_font, m_textureTarget ) )
			{
				m_charTable.insert( string[i], c);
				c->draw();
			}
			else delete c;
		}
	}
	glPopMatrix();

	if( ! m_isBetweenBeginAndEnd ) do_end();
}

void createOrthoBasisGivenFirstVector
	( const Vector3d &U, Vector3d * v, Vector3d * w )
{
	U.makeOrthoVector(v);
	*w = cross( U, *v );
	w->normalize();
}

} // namespace KalziumGLHelpers
