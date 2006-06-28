#ifndef KALZIUMGLHELPERCLASSES_H
#define KALZIUMGLHELPERCLASSES_H
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
#include <math.h>
#include <QGLWidget>
#include <QPainter>
#include <QImage>

/** USE_DOUBLE_PRECISION: if defined, use doubles instead of floats for
 * handling the model's geometric data. This does not seem to impact
 * significantly the performance. The vertex arrays are unaffected: they
 * always use floats.
 */
#define USE_DOUBLE_PRECISION

/** USE_FPS_COUNTER: if defined, the GL Widgets will show a frames-per-second
 * counter. *Use only for testing*: this makes the GL Widget constantly
 * redraw, which under normal circumstances is a waste of CPU power.
 */
#define USE_FPS_COUNTER 

namespace KalziumGLHelpers
{

#ifdef USE_DOUBLE_PRECISION
typedef double FLOAT;
typedef GLdouble GLFLOAT;
#else
typedef float FLOAT;
typedef GLfloat GLFLOAT;
#endif

inline float SQRT( float x ) { return sqrtf( x ); }
inline double SQRT( double x ) { return sqrt( x ); }
inline float SIN( float x ) { return sinf( x ); }
inline double SIN( double x ) { return sin( x ); }
inline float COS( float x ) { return cosf( x ); }
inline double COS( double x ) { return cos( x ); }
inline float FABS( float x ) { return fabsf( x ); }
inline double FABS( double x ) { return fabs( x ); }
inline void GLMULTMATRIX( const GLfloat *m ) { glMultMatrixf(m); }
inline void GLMULTMATRIX( const GLdouble *m ) { glMultMatrixd(m); }
inline void GLTRANSLATE( GLfloat x, GLfloat y, GLfloat z ) \
	{ glTranslatef( x, y, z ); }
inline void GLTRANSLATE( GLdouble x, GLdouble y, GLdouble z ) \
	{ glTranslated( x, y, z ); }

/**
* This class represents a color in OpenGL float red-green-blue format.
*
* @author Benoit Jacob
*/
struct Color
{
	GLfloat m_red, m_green, m_blue, m_alpha;

	Color();
	Color( GLfloat red, GLfloat green, GLfloat blue,
		GLfloat alpha = 1.0 );

	Color& operator=( const Color& other );

	/**
	* Sets this color to be the one used by OpenGL for rendering
	* when lighting is disabled.
	*/
	inline void apply()
	{
		glColor4fv( reinterpret_cast<GLfloat *>( this ) );
	}

	/**
	* Applies nice OpenGL materials using this color as the
	* diffuse color while using different shades for the ambient and
	* specular colors. This is only useful if lighting is enabled.
	*/
	void applyAsMaterials();
};

/**
* Tests whether two Ts are approximately equal. Here T is assumed to be
* a floating-point type. Recall that operator== between floating-point
* types is broken.
* returns true if abs( a - b ) <= c * precision
* where c = max( abs( a ), abs( b ) )
*/
template<class T> static bool approx_equal( T a, T b, T precision )
{
	T abs_a = FABS( a );
	T abs_b = FABS( b );

	T max_abs;
	if( abs_a <= abs_b )
		max_abs = abs_b;
	else
		max_abs = abs_a;
	return( FABS( a - b ) <= precision * max_abs );
}

/**
* This template class represents a vector in 3-space. It is meant to be
* used with T = a floating-point type.
*
* @author Benoit Jacob
*/

template<class T> class Vector3
{
	public:
		T x, y, z;
		Vector3() {}
		Vector3( T _x, T _y, T _z)
		{ x = _x; y = _y; z = _z; }

		Vector3<T>& operator= ( const Vector3<T>& other )
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		/**
		* returns the norm of the vector, that is, its length
		*/
		inline T norm() const { return SQRT( x * x + y * y + z * z ); }

		/**
		* normalizes the vector, that is, scales it so that its norm
		* becomes 1.
		*/
		void normalize()
		{
			T n = norm();
			if( n == 0.0 ) return;
			x /= n;
			y /= n;
			z /= n;
		}
};

/**
* Given a vector U, constructs two vectors v and w
* such that (U, v, w) is a direct orthogonal basis.
* U is not supposed to be normalized.
* v and w are not getting normalized.
*/
template<class T> void construct_ortho_basis_given_first_vector(
	const Vector3<T> &U, Vector3<T> & v, Vector3<T> & w )
{
	if( U.norm() == 0 ) return;

	// let us first make a normalized copy of U
	Vector3<T> u = U;
	u.normalize();

	// first we want to set v to be non-colinear to u
	v = u;

	if( ! approx_equal( v.x, v.y, 0.1 ) )
	{
		T tmp = v.x;
		v.x = v.y;
		v.y = tmp;
	}
	else if( ! approx_equal( v.y, v.z, 0.1 ) )
	{
		T tmp = v.z;
		v.z = v.y;
		v.y = tmp;
	}
	else // the 3 coords of v are approximately equal
	{    // which implies that v is not colinear to (0,0,1)
		v = Vector3<T>( 0, 0, 1 );
	}

	// now, v is not colinear to u. We compute its dot product with u
	T u_dot_v = u.x * v.x + u.y * v.y + u.z * v.z;

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

/**
* This is an abstract base class for an OpenGL vertex array.
*
* @author Benoit Jacob
*/
class VertexArray
{

	protected:
		GLenum m_mode;
		Vector3<GLfloat> *m_vertexBuffer;
		Vector3<GLfloat> *m_normalBuffer;
		unsigned int m_vertexCount;
		unsigned short *m_indexBuffer;
		unsigned int m_indexCount;
		
		bool m_isInitialized;
		
		virtual void initialize() = 0;
		virtual bool allocateBuffers();

	public:
		VertexArray();
		virtual ~VertexArray();
		virtual void select();
		virtual inline void draw()
		{
			glDrawElements( m_mode, m_indexCount,
				GL_UNSIGNED_SHORT, m_indexBuffer );
		}
};

/**
* This class represents and draws a sphere
*
* @author Benoit Jacob
*/
class Sphere : public VertexArray
{
	private:
		inline unsigned short indexOfVertex(
			int strip, int column, int row);
		void computeVertex( int strip, int column, int row );

	protected:
		int m_detail;
		GLfloat m_radius;

		virtual void initialize();

	public:
		Sphere();
		virtual ~Sphere() {}
		virtual void setup( int detail, GLfloat radius );
		virtual void drawScaled( GLfloat radius );
};

/**
* This class represents and draws a cylinder
*
* @author Benoit Jacob
*/
class Cylinder : public VertexArray
{
	protected:
		int m_faces;
		GLfloat m_radius;

		virtual void initialize();

	public:
		Cylinder();
		virtual ~Cylinder() {}
		virtual void setup( int detail, GLfloat radius );
		virtual inline void draw()
		{
			glDrawArrays( m_mode, 0, m_vertexCount );
		}
};

class TextPainter
{
	protected:
		int m_width, m_height;
		QImage *m_image;
		QPainter *m_painter;
		QFontMetrics *m_fontMetrics;

	public:
		TextPainter()
		{
			m_width = 0;
			m_height = 0;
			m_image = 0;
			m_painter = 0;
			m_fontMetrics = 0;
		}
		~TextPainter()
		{
			if( m_image ) delete m_image;
			if( m_painter ) delete m_painter;
			if( m_fontMetrics ) delete m_fontMetrics;
		}
		bool print( QGLWidget *glwidget, int x, int y, const QString &string)
		{
			glDisable( GL_LIGHTING );
			glEnable(GL_TEXTURE_2D);
			
			/*if( ! m_font )
			{
				m_font = new QFont();
				if( ! m_font ) return false;
			}*/

			const QFont &m_font = (glwidget->font());

			if( ! m_painter )
			{
				m_painter = new QPainter();
				if( ! m_painter ) return false;
			}

			m_painter->setFont(m_font);

			if( ! m_fontMetrics )
			{
				
				m_fontMetrics = new QFontMetrics(m_font);
				if( ! m_fontMetrics ) return false;
			}
		
			int new_width = m_fontMetrics->width( string );
			int new_height = m_fontMetrics->height();
			
			if(new_width == 0 || new_height == 0)
			{
				return false;
			}

			if( new_width > m_width || new_height > m_height )
			{
				if( m_image ) delete m_image;
				m_width = ( new_width > m_width ) ? new_width : m_width;
				m_height = ( new_height > m_height ) ? new_height : m_height;
				m_image = new QImage( m_width, m_height, QImage::Format_ARGB32 );
			}
		
			m_painter->begin( m_image );
			m_painter->setRenderHint(QPainter::TextAntialiasing);
			//painter.setBackground(Qt::black);
			m_painter->setBrush(Qt::white);
			m_painter->eraseRect( 0, 0, m_width, m_height );
		
			//painter.drawText ( 0, 0, s );
			m_painter->drawText ( 0, m_height, string );
			m_painter->end();
		
			glwidget->bindTexture( *m_image );
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			glOrtho( 0, glwidget->width(), 0, glwidget->height(), -1, 1 );
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();
			glBegin(GL_QUADS);
			glTexCoord2f( 0, 0);
			glVertex2f( x , y );
			glTexCoord2f( 1, 0);
			glVertex2f( x+m_width , y );
			glTexCoord2f( 1, 1);
			glVertex2f( x+m_width , y+m_height );
			glTexCoord2f( 0, 1);
			glVertex2f( x , y+m_height );
			glEnd();
			glDisable( GL_TEXTURE_2D);
			glPopMatrix();
			glMatrixMode( GL_PROJECTION );
			glPopMatrix();
			glMatrixMode( GL_MODELVIEW );
			return true;
		}
};

} // namespace KalziumGL

#endif // KALZIUMGLHELPERCLASSES_H

