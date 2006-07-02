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

using namespace KalziumGLHelpers;

Color::Color()
{
}

Color::Color( GLfloat red, GLfloat green, GLfloat blue,
                  GLfloat alpha )
{
	m_red = red;
	m_green = green;
	m_blue = blue;
	m_alpha = alpha;
}

Color& Color::operator=( const Color& other )
{
	m_red = other.m_red;
	m_green = other.m_green;
	m_blue = other.m_blue;
	m_alpha = other.m_alpha;

	return *this;
}

void Color::applyAsMaterials()
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

VertexArray::VertexArray()
{
	m_mode = GL_TRIANGLE_STRIP;
	m_vertexBuffer = 0;
	m_normalBuffer = 0;
	m_indexBuffer = 0;
	m_isInitialized = false;
}

VertexArray::~VertexArray()
{
	if( m_indexBuffer ) delete [] m_indexBuffer;
	if( m_vertexBuffer ) delete [] m_vertexBuffer;
	if( m_normalBuffer ) delete [] m_normalBuffer;
}

bool VertexArray::allocateBuffers()
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

	m_vertexBuffer = new Vector3<GLfloat>[m_vertexCount];
	if( ! m_vertexBuffer ) return false;
	m_normalBuffer = new Vector3<GLfloat>[m_vertexCount];
	if( ! m_normalBuffer ) return false;
	m_indexBuffer = new unsigned short[m_indexCount];
	if( ! m_indexBuffer ) return false;

	return true;
}

Sphere::Sphere()
	: VertexArray()
{
	m_detail = 0;
	m_radius = -1.0;
}

unsigned short Sphere::indexOfVertex( int strip, int column, int row)
{
	return ( row + ( 3 * m_detail + 1 ) * ( column + m_detail * strip ) );
}

void Sphere::computeVertex( int strip, int column, int row)
{
	strip %= 5;
	int next_strip = (strip + 1) % 5;

	Vector3<GLfloat> *vertex =
		&m_vertexBuffer[ indexOfVertex( strip, column, row ) ];

	Vector3<GLfloat> *normal =
		&m_normalBuffer[ indexOfVertex( strip, column, row ) ];

	const GLfloat phi = ( 1 + sqrt(5) ) / 2;

	const Vector3<GLfloat> northPole( 0, 1, phi );
	const Vector3<GLfloat> northVertex[5] = {
		Vector3<GLfloat>( 0, -1, phi ),
		Vector3<GLfloat>( phi, 0, 1 ),
		Vector3<GLfloat>( 1, phi, 0 ),
		Vector3<GLfloat>( -1, phi, 0 ),
		Vector3<GLfloat>( -phi, 0, 1 ) };
	const Vector3<GLfloat> southVertex[5] = {
		Vector3<GLfloat>( -1, -phi, 0 ),
		Vector3<GLfloat>( 1, -phi, 0 ),
		Vector3<GLfloat>( phi, 0, -1 ),
		Vector3<GLfloat>( 0, 1, -phi ),
		Vector3<GLfloat>( -phi, 0, -1 )
		 };
	const Vector3<GLfloat> southPole( 0, -1, -phi );

	const Vector3<GLfloat> *v0, *v1, *v2;
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


void Sphere::initialize()
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

void Sphere::setup( int detail, GLfloat radius )
{
	if( detail == m_detail && radius == m_radius ) return;
	m_detail = detail;
	m_radius = radius;
	initialize();
}

void Sphere::drawScaled( GLfloat radius )
{
	const GLfloat precision	= 0.001;

	if( approx_equal( radius, m_radius, precision ) )
	{
		draw();
		return;
	}

	GLfloat factor = radius / m_radius;
//	glEnable( GL_NORMALIZE );
	glPushMatrix();
	glScalef( factor, factor, factor );
	draw();
	glPopMatrix();
//	glDisable( GL_NORMALIZE );
}

Cylinder::Cylinder()
	: VertexArray()
{
	m_mode = GL_QUAD_STRIP;
	m_faces = 0;
	m_radius = -1.0;
}

void Cylinder::setup( int faces, GLfloat radius )
{
	if( faces == m_faces && radius == m_radius ) return;
	m_faces = faces;
	m_radius = radius;
	initialize();
}

void Cylinder::initialize()
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
bool CharRenderer::initialize( QChar c, const QFont &font )
{
	if( m_displayList ) return true;
	
	QFontMetrics fontMetrics ( font );
	m_width = fontMetrics.width( c );
	m_height = fontMetrics.height();
	if( m_width == 0 || m_height == 0 ) return false;
	QImage image( m_width, m_height, QImage::Format_RGB32 );
	
	QPainter painter;
	painter.begin( &image );
	painter.setFont( font );
	painter.setRenderHint( QPainter::TextAntialiasing );
	painter.setBackground(Qt::black);
	painter.eraseRect( image.rect() );
	painter.setPen(Qt::white);
	painter.drawText ( 0, 0, m_width, m_height, Qt::AlignBottom, c);
	painter.end();

	GLubyte *bitmap = new GLubyte [m_width * m_height];
	if( bitmap == 0 ) return false;

	for( int i = 0; i < m_width; i++)
	for( int j = 0; j < m_height; j++)
	{
		bitmap[ i + m_width * j ] =
			image.pixel( i, m_height - j - 1 ) & 0x000000ff;
	}

	glGenTextures( 1, &m_texture );
	if( m_texture == 0 ) return false;

	glBindTexture(GL_TEXTURE_2D,m_texture) ;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D (
		GL_TEXTURE_2D,
		0,
		GL_ALPHA,
		m_width,
		m_height,
		0,
		GL_ALPHA,
		GL_UNSIGNED_BYTE,
		bitmap );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	delete [] bitmap;

	m_displayList = glGenLists(1);
	if( m_displayList == 0 ) return false;

	glNewList( m_displayList, GL_COMPILE );
	glBindTexture( GL_TEXTURE_2D, m_texture );
	glBegin(GL_QUADS);
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
	m_wasEnabled_TEXTURE_2D = glIsEnabled( GL_TEXTURE_2D );
	m_wasEnabled_BLEND = glIsEnabled( GL_BLEND );
	m_wasEnabled_DEPTH_TEST = glIsEnabled( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_FOG );
	glEnable( GL_TEXTURE_2D );
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
	if( ! m_wasEnabled_TEXTURE_2D ) glDisable( GL_TEXTURE_2D);
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

void TextRenderer::print( int x, int y, const QString &string)
{
	if( ! m_glwidget ) return;
	if( string.isEmpty() ) return;

	if( ! m_isBetweenBeginAndEnd ) do_begin();
	
	glPushMatrix();
	glLoadIdentity();
	glTranslatef( x, y, 0 );
	for(int i = 0; i < string.size(); i++)
	{
		if( m_charTable.contains( string[i] ) )
			m_charTable.value(string[i])->draw();
		else
		{
			CharRenderer *c = new CharRenderer;
			if( c->initialize( string[i], m_font ) )
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
