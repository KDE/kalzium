#ifndef KALZIUMGLWIDGET_H
#define KALZIUMGLWIDGET_H
/***************************************************************************
    copyright            : (C) 2006 by Benoit Jacob
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QGLWidget>
#include <openbabel/mol.h>

#define FLOAT	double

#if(FLOAT==double)
#define GLFLOAT		GLdouble
#define GLSUFFIX	d
#define SQRT		sqrt
#define COS		cos
#define SIN		sin
#define FABS		fabs
#elif(FLOAT==float)
#define GLFLOAT		GLfloat
#define GLSUFFIX	f
#define SQRT		sqrtf
#define COS		cosf
#define SIN		sinf
#define FABS		fabsf
#endif

/**
 * @autor Benoit Jacob
 */
class GLVertexArray
{
	protected:
		struct Vertex {
			float nx, ny, nz;
			float vx, vy, vz;
		};

		Vertex *m_vertices;
		unsigned short *m_indices;
		unsigned int m_nbVertices;
		unsigned int  m_nbIndices;
		
		virtual void generate() = 0;
	public:
		GLVertexArray();
		virtual ~GLVertexArray();
		void draw();
};

/**
 * @autor Benoit Jacob
 */
class SphereVertexArray : public GLVertexArray
{
	protected:
		unsigned int m_strips, m_lozangesPerStrip;
		virtual void generate();

	public:
		SphereVertexArray(unsigned int strips, unsigned int lozangesPerStrip);
		virtual ~SphereVertexArray();

		virtual void regenerate(unsigned int strips, unsigned int lozangesPerStrip);
};

/**
 * This class displays the 3D-view of a molecule
 * 
 * @autor Benoit Jacob
 */
class KalziumGLWidget : public QGLWidget
{
	Q_OBJECT

	protected:
		GLuint m_sphereDisplayList;
		GLuint m_bondDisplayList;
		bool m_isDragging;
		QPoint m_lastDraggingPosition;
		GLFLOAT m_RotationMatrix[16];

	public:
		/**
		 * Constructor
		 */
		KalziumGLWidget( QWidget *parent = 0 );

		/**
		 * Destructor
		 */
		virtual ~KalziumGLWidget();

		void getColor( const OpenBabel::OBAtom &a, GLfloat &r, GLfloat &g, GLfloat &b );

	public slots:
		/**
		 * sets the molecule which will be displayed
		 * @param molecule the molecule to render
		 */
		void slotSetMolecule( OpenBabel::OBMol* molecule );
		
		/**
		 * Sets the detail-grade in a range from 0 to 2
		 *  @param detail the detail-grade of the rendering. 0 is low, 2 is high
		 */
		void slotSetDetail( int detail );

	protected:
		/**
		 * This method initilized OpenGL
		 */
		virtual void initializeGL();
		virtual void paintGL();
		virtual void resizeGL( int width, int height );
		virtual void mousePressEvent( QMouseEvent * event );
		virtual void mouseReleaseEvent( QMouseEvent * event );
		virtual void mouseMoveEvent( QMouseEvent * event );

		/**
		 * This method...
		 */
		virtual void drawGenericSphere();
		virtual void drawGenericBond();
		
		/**
		 * This method...
		 * @param x
		 * @param y
		 * @param z
		 * @param radius
		 * @param red
		 * @param green
		 * @param blue
		 */
		virtual void drawSphere( 
				GLFLOAT x, 
				GLFLOAT y, 
				GLFLOAT z, 
				GLFLOAT radius,
				GLfloat red, 
				GLfloat green, 
				GLfloat blue );

		virtual void drawBond( GLFLOAT x1, GLFLOAT y1, GLFLOAT z1,
			GLFLOAT x2, GLFLOAT y2, GLFLOAT z2,
			GLfloat red, GLfloat green, GLfloat blue );

		/**
		 * The molecule which is displayed
		 */
		OpenBabel::OBMol* m_molecule;
		FLOAT m_molRadius;
		FLOAT m_molMinBondLength;
		FLOAT m_molMaxBondLength;
		FLOAT m_bondsRadius;

		/**
		 * The detail-grade from 0 to 2
		 */
		int m_detail;

		/**
		 * The coefficient set by the user, determining the
		 * radius of atoms.
		 * 0.0 -> minimum radius -> "sticks-style" rendering
		 * 1.0 -> maximum radius for which bonds are visible
		 * values larger than 1.0 result in atoms so large
		 * that they completely hide the bonds.
		 */
		float m_atomsRadiusCoeff;
};

// tests whether two FLOATs are equal
bool approx_equal( FLOAT a, FLOAT b );

// compute the norm of a vector (dimension 3)
FLOAT norm3 ( const FLOAT *u );

// normalize a vector (dimension 3)
void normalize3( FLOAT *u );

// given a first vector U, construct two new vectors v and w such that
// (U, v, w) is a direct orthogonal basis (dimension 3).
// U is not supposed to be normalized, and v and w aren't normalized.
void construct_ortho_3D_basis_given_first_vector3(
	const FLOAT *U, FLOAT *v, FLOAT *w);

#endif // KALZIUMGLWIDGET_H
