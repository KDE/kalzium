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
#include <QChar>
#include <QHash>
#include <kdebug.h>

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
		virtual inline void select()
		{
			glVertexPointer( 3, GL_FLOAT, 0, m_vertexBuffer );
			glNormalPointer( GL_FLOAT, 0, m_normalBuffer );
		}
		virtual inline void draw()
		{
			select();
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
			select();
			glDrawArrays( m_mode, 0, m_vertexCount );
		}
};

/** This is a helper class for TextRenderer, and should probably never be
* used directly. See TextRenderer.
*
* The CharRenderer class represents a character stored as OpenGL rendering
* data : a texture object and a display list mapping it on a quad and then
* translating to the right of it.
*
* See the m_charTable member of TextRenderer for an example of use of
* this class.
*/
class CharRenderer
{
	protected:
		/**
		 * The OpenGL texture object
		 */
		GLuint m_texture;

		/**
		 * The OpenGL display list
		 */
		GLuint m_displayList;

		/**
		 * Width and height in pixels of the rendered character
		 */
		int m_width, m_height;

	public:
		CharRenderer();
		~CharRenderer();
		bool initialize( QChar c, const QFont &font );
		inline void draw()
		{
			glCallList( m_displayList );
		}
};


/** This class renders text inside a QGLWidget. It replaces the functionality
* of QGLWidget::renderText(). The advantages over renderText() include:
*  - supports any font, any character encoding supported by Qt
*    (renderText is 8-bit-only and can only use "OpenGL-compatible" fonts)
*  - does not use any library outside Qt (renderText uses FreeType on X11)
*  - renders characters as textured quads instead of calling glDrawPixels,
*    which does not make much of a difference on MesaGL, but can be a lot
*    faster and safer with other (buggy) OpenGL implementations. It will also
*    allow to add more graphical effects in the future, like rotation,
*    if we ever need that.
*  - the characters are stored as 8bpp Alpha, which takes 4 times less
*    memory than the 32bpp RGBA used by renderText.
*  - the characters are rendered on-the-fly on the first time they appear
*    in a QString being printed. This is achieved using a QHash to test whether
*    a character has already been rendered.
*
* Recommended usage:
* The TextRender class is meant to be used from inside a child class of
* QGLWidget, say MyGLWidget.
*
* In the declaration of MyGLWidget, please declare a TextRenderer member:
* 
* @code
class MyGLWidget : public QGLWidget
{
	...
	TextRenderer m_textRenderer;
	...
};
* @endcode
* 
* Now, in the constructor of MyGLWidget, you please call setup()
* along these lines:
*
* @code
	QFont f;
	f.setStyleHint( QFont::SansSerif, QFont::PreferAntialias );
	m_textRenderer.setup( this, f );
* @endcode 
*
* The setup() method should be called only once, which means you have to choose
* a font once and for all, in the lifetime of your TextRenderer. Any QFont can
* be used, the above is just an example. Now, to actually render text, in
* the MyGLWidget::paintGL() method, you can call

* @code
	m_textRenderer.print( x, y, string );
* @endcode

* where x,y are ints and string is any QString. If you want to choose a color,
* please call glColor3f or glColor4f before calling print(). Of course you can
* also call qglColor or Color::apply. You can achieve semitransparent text at
* no additional cost by choosing a semitransparent color.
*
* If you wish to do several calls to print(), it will improve performance
* to enclose them between a call to begin() and a call to end(), like that:
* 
* @code
	m_textRenderer.begin();
	m_textRenderer.print( x1, y1, string1 );
	m_textRenderer.print( x2, y2, string2 );
	m_textRenderer.print( x3, y2, string3 );
	m_textRenderer.end();
* @endcode
* 
* Please make sure, though, that no OpenGL state change occurs between begin()
* and end(), except the state changes performed by the TextRenderer itself.
* In other words, please avoid calling glSomething() between begin() and end(),
* except if you are sure that this call won't perform a relevant state change.
*
* The print() method when called alone, or the begin()-print()-end() group,
* do restore the OpenGL state as they found it, including the matrices.
*
* If you experience rendering problems, you can try the following:
* - disable some OpenGL state bits. For instance, TextRenderer automatically
*   disables fog and lighting during rendering, because it doesn't work
*   correctly with them enabled. There probably are other OpenGL state bits
*   that have to be disabled, so if your program enables some of them, you
*   might have to disable them before rendering text.
* - if you experience poor font quality, please consider using an antialiased
*   font.
*
* @author Benoit Jacob
*/
class TextRenderer
{
	protected:
		/**
		 * The font used for rendering the chars. This is set
		 * once and for all by setup(). Note that it is stored
		 * by value, so the caller doesn't have to keep it alive.
		 */
		QFont m_font;
		
		/**
		 * This hash gives the correspondence table between QChars
		 * (the keys) and the corresponding CharRenderers (the values).
		 * Every time a QChar is being met, either it is found in this
		 * table, in which case it can be directly rendered, or it is not
		 * found, in which case a new CharRenderer is created for it,
		 * and added to this table.
		 */
		QHash<QChar, CharRenderer*> m_charTable;

		/**
		 * The QGLWidget in which to render. This is set
		 * once and for all by setup().
		 */
		const QGLWidget *m_glwidget;

		/**
		 * This equals true if begin() has been called, but end() hasn't
		 * since.
		 */
		GLboolean m_isBetweenBeginAndEnd;

		/**
		 * These members are used to remember the OpenGL state in order
		 * to be able to restore it after rendering. See do_end().
		 */
		GLboolean m_wasEnabled_LIGHTING;
		GLboolean m_wasEnabled_TEXTURE_2D;
		GLboolean m_wasEnabled_FOG;
		GLboolean m_wasEnabled_BLEND;
		GLboolean m_wasEnabled_DEPTH_TEST;

		/**
		 * Stores the relevant part of the OpenGL state, and prepares
		 * for rendering
		 */
		void do_begin();

		/**
		 * Restores the OpenGL state
		 */
		void do_end();

	public:
		TextRenderer();
		~TextRenderer();
		
		/**
		 * This should be called only once, before any printing occurs.
		 * @param glwidget The QGLWidget in which to render.
		 * See m_glwidget member.
		 * @param font The QFont to use. See m_font member.
		 */
		void setup( const QGLWidget *glwidget, const QFont &font );

		/**
		 * Prints text at the position (x,y) in window coordinates
		 * (0,0) is the bottom left corner
		 * @param x the x-coordinate
		 * @param y the y-coordinate
		 * @param string the QString to print
		 */
		void print( int x, int y, const QString &string);

		/**
		 * Call this before doing multiple calls to print(). This is
		 * not necessary, but will improve performance. Don't forget,
		 * then, to call end() after.
		 */
		void begin();

		/**
		 * Call this after having called begin() and print().
		 */
		void end();
};

} // namespace KalziumGL

#endif // KALZIUMGLHELPERCLASSES_H

