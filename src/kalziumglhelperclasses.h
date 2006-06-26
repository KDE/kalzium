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

#define USE_DOUBLE_PRECISION
#define USE_DISPLAY_LIST
#define USE_FPS_COUNTER

#ifdef USE_DOUBLE_PRECISION
#define FLOAT		double
#define GLFLOAT		GLdouble
#else
#define FLOAT		float
#define GLFLOAT		GLfloat
#endif

inline float GLSQRT( float x ) { return sqrtf( x ); }
inline double GLSQRT( double x ) { return sqrt( x ); }
inline float GLSIN( float x ) { return sinf( x ); }
inline double GLSIN( double x ) { return sin( x ); }
inline float GLCOS( float x ) { return cosf( x ); }
inline double GLCOS( double x ) { return cos( x ); }
inline float GLFABS( float x ) { return fabsf( x ); }
inline double GLFABS( double x ) { return fabs( x ); }
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
struct GLColor
{
	GLfloat m_red, m_green, m_blue, m_alpha;

	GLColor();
	GLColor( GLfloat red, GLfloat green, GLfloat blue,
	         GLfloat alpha = 1.0 );

	GLColor& operator=( const GLColor& other );

	/**
	 * Sets this color to be the one used by OpenGL for rendering
	 * when lighting is disabled. It just calls glColor4fv.
	 */
	//TODO inline?
	void apply();

	/**
	 * Applies nice OpenGL materials using this color as the
	 * diffuse color while using different shades for the ambient and
         * specular colors. This is only useful if GL lighting is enabled.
	 */
	void applyAsMaterials();
};

/**
 * This template class represents a vector in 3-space. It is meant to be
 * used with T = a floating-point type.
 *
 * @author Benoit Jacob
 */

template<class T> class GLVector3
{
	public:
		T x, y, z;
		GLVector3() {}
		GLVector3( T _x, T _y, T _z)
		{ x = _x; y = _y; z = _z; }

		/**
		 * Tests whether two FLOATs are approximately equal.
		 * Recall that operator == between floating-point types
		 * is broken.
		 * returns true if abs( a - b ) <= c * precision
		 * where c = max( abs( a ), abs( b ) )
		 */
		static bool approx_equal( FLOAT a, FLOAT b, FLOAT precision );

		GLVector3<T>& operator= ( const GLVector3<T>& other )
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		/**
		 * returns the norm of the vector, that is, its length
		 */
		inline T norm() { return GLSQRT( x * x + y * y + z * z ); }

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
		
		/**
		 * Constructs two vectors v and w
		 * such that (*this, v, w) is a direct orthogonal basis.
		 * v and w are not getting normalized.
		 */
		void construct_ortho_basis_given_first_vector(
			GLVector3<T> & v, GLVector3<T> & w );
};

/**
 * This is an abstract base class for an OpenGL vertex array.
 *
 * @author Benoit Jacob
 */
class GLVertexArray
{
	private:
		int m_id;

	protected:
		GLenum m_mode;
		GLVector3<GLfloat> *m_vertexBuffer;
		GLVector3<GLfloat> *m_normalBuffer;
		unsigned int m_vertexCount;
		unsigned short *m_indexBuffer;
		unsigned int m_indexCount;
		
		bool m_isInitialized;
		
		virtual void initialize() = 0;
		virtual bool allocateBuffers();
		virtual void allocateId();
		virtual void select();

	public:
		GLVertexArray();
		virtual ~GLVertexArray();
		//inline?
		virtual void draw();
};

/**
 * This class represents and draws a sphere
 *
 * @author Benoit Jacob
 */
class GLSphere : public GLVertexArray
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
		GLSphere();
		virtual ~GLSphere() {}
		virtual void setup( int detail, GLfloat radius );
		virtual void drawScaled( GLfloat radius );
};

/**
 * This class represents and draws a cylinder
 *
 * @author Benoit Jacob
 */
class GLCylinder : public GLVertexArray
{
	protected:
		int m_faces;
		GLfloat m_radius;

		virtual void initialize();

	public:
		GLCylinder();
		virtual ~GLCylinder() {}
		virtual void setup( int detail, GLfloat radius );
		virtual inline void draw();
};

#endif // KALZIUMGLHELPERCLASSES_H

