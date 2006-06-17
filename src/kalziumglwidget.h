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

#define USE_DOUBLE_PRECISION

#ifdef USE_DOUBLE_PRECISION
#define FLOAT		double
#define GLFLOAT		GLdouble
#define GLTRANSLATE	glTranslated
#define GLMULTMATRIX	glMultMatrixd
#define SQRT		sqrt
#define COS		cos
#define SIN		sin
#define FABS		fabs
#else
#define FLOAT		float
#define GLFLOAT		GLfloat
#define GLTRANSLATE	glTranslatef
#define GLMULTMATRIX	glMultMatrixf
#define SQRT		sqrtf
#define COS		cosf
#define SIN		sinf
#define FABS		fabsf
#endif

struct GLColor;

/**
 * This class displays the 3D-view of a molecule
 * 
 * @author Benoit Jacob
 */
class KalziumGLWidget : public QGLWidget
{
	Q_OBJECT

	protected:
		GLuint m_sphereDisplayList;
		GLuint m_bondDisplayList;
		
		/**
		 * equals true if the user is currently dragging (rotating)
		 * the view
		 */
		bool m_isDragging;

		QPoint m_lastDraggingPosition;

		GLFLOAT m_RotationMatrix[16];

		/**
		 * The molecule which is displayed
		 */
		OpenBabel::OBMol* m_molecule;
		FLOAT m_molRadius;
		FLOAT m_molMinBondLength;
		FLOAT m_molMaxBondLength;

		/**
		 * The coefficient set by the user, determining the
		 * radius of atoms.
		 * WARNING: its meaning has just changed! (june 17)
		 * Now the actual radius is proportional to
		 * m_atomRadiusCoeff.
		 */
		float m_atomRadiusCoeff;

		/**
		 * The coefficient set by the user, determining the
		 * radius (that is, half the thickness) of bonds.
		 */
		float m_bondRadiusCoeff;

		/**
		 * The detail-grade from 0 to 2.
		 */
		int m_detail;

		/**
		 * Set this to true to enable the fog effect
		 */
		bool m_useFog;


		/**
		 * The selected atom
		 */
		OpenBabel::OBAtom* m_selectedAtom;

		/**
		 * The style in which the atoms are rendered.
		 */
		enum AtomStyle
		{
			ATOM_DISABLED,
			ATOM_SPHERE
		} m_atomStyle;

		/**
		 * The style in which the bonds are rendered.
		 */
		enum BondStyle
		{
			BOND_DISABLED,
			BOND_LINE,
			BOND_CYLINDER_GRAY,
			BOND_CYLINDER_BICOLOR
		} m_bondStyle;

		/**
		 * Some style presets
		 */
		enum StylePreset
		{
			PRESET_LINES,
			PRESET_STICKS,
			PRESET_SPHERES_AND_GRAY_BONDS,
			PRESET_SPHERES_AND_BICOLOR_BONDS,
			PRESET_BIG_SPHERES
		};

	private: // some standard 3D math stuff here

		/**
		 * Tests whether two FLOATs are approximately equal.
		 * Recall that operator == between floating-point types
		 * is broken.
		 * returns true if abs( a - b ) <= c * precision
		 * where c = max( abs( a ), abs( b ) )
		 */
		bool approx_equal( FLOAT a, FLOAT b, FLOAT precision );

		/**
		 * Returns the norm of a 3D vector
		 */
		FLOAT norm3( FLOAT *u );

		/**
		 * Normalizes a 3D vector
		 */
		void normalize3( FLOAT *u );

		/**
		 * Given a 3D vector U, constructs two vectors v and w
		 * such that (U, v, w) is a direct orthogonal basis.
		 * U is not supposed to be normalized, and v and w
		 * are not getting normalized.
		 */
		void construct_ortho_3D_basis_given_first_vector3(
			const FLOAT *U, FLOAT *v, FLOAT *w);

	public:
		/**
		 * Constructor
		 */
		KalziumGLWidget( QWidget *parent = 0 );

		/**
		 * Destructor
		 */
		virtual ~KalziumGLWidget();

		inline OpenBabel::OBMol* molecule () const
			{ return m_molecule; }

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

		/**
		 * Chooses the style of rendering among some presets
		 * @param stylePreset the wanted style preset
		 */
		/**
		 * Chooses the style of rendering among some presets
		 * @param stylePreset the wanted style preset
		 */
		void slotChooseStylePreset( int stylePreset ){
			ChooseStylePreset( (StylePreset) stylePreset );
		}

		/**
		 * The atom @p atom was selected by the user
		 */
		void slotAtomSelected( OpenBabel::OBAtom* atom );

	protected:
		/**
		 * This method initializes OpenGL. Automatically called by Qt
		 */
		virtual void initializeGL();

		/**
		 * This method does the painting. Automatically called by Qt
		 */
		virtual void paintGL();

		/**
		 * This method is called by Qt whenever the widget is resized.
		 */
		virtual void resizeGL( int width, int height );

		virtual void mousePressEvent( QMouseEvent * event );
		virtual void mouseReleaseEvent( QMouseEvent * event );
		virtual void mouseMoveEvent( QMouseEvent * event );

		/**
		 * This method is called by slotSetMolecule. It prepares the
		 * molecule for rendering, and computes some useful data about
		 * it.
		 */
		void KalziumGLWidget::prepareMoleculeData();

		/**
		 * This method will shortly be removed, hence no doc.
		 */
		virtual void drawGenericSphere();

		/**
		 * This method will shortly be removed, hence no doc.
		 */
		virtual void drawGenericBond();
		
		/**
		 * This method draws a sphere
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
				GLColor &color );

		/**
		 * This method draws a bond
		 * @param x
		 * @param y
		 * @param z
		 * @param radius
		 * @param red
		 * @param green
		 * @param blue
		 */

		virtual void drawBond( GLFLOAT x1, GLFLOAT y1, GLFLOAT z1,
			GLFLOAT x2, GLFLOAT y2, GLFLOAT z2,
			GLColor &color );

		inline float bondRadius();
		inline float atomRadius();

		/**
		 * Chooses the style of rendering among some presets
		 * @param stylePreset the wanted style preset
		 */
		void ChooseStylePreset( StylePreset stylePreset );

		/**
		 * returns the color which an atom should be painted
		 */

		GLColor& getAtomColor( OpenBabel::OBAtom* atom );

};

/**
 * This class represents a color in OpenGL float red-green-blue format.
 *
 * @author Benoit Jacob
 */
struct GLColor
{
	GLfloat m_red, m_green, m_blue;

	GLColor();
	GLColor(GLfloat red, GLfloat green, GLfloat blue);

	GLColor& operator=( const GLColor& other );

	void clamp();

	/**
	 * Sets this color to be the one used by OpenGL for rendering
	 * when lighting is disabled. It just calls glColor3fv.
	 */
	inline void apply();

	/**
	 * Applies nice OpenGL materials using this color as the
	 * diffuse color while using different shades for the ambient and
         * specular colors. This is only useful if GL lighting is enabled.
	 */
	void applyAsMaterials();
};

/**
 * This is an abstract base class for a GL vertex array
 *
 * @author Benoit Jacob
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
 * This class generates and stores a GL vertex array representing a sphere
 *
 * @author Benoit Jacob
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

#endif // KALZIUMGLWIDGET_H
