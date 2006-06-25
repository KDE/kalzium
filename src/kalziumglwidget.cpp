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
#include "kalziumglwidget.h"

#include <math.h>

#include <kdebug.h>

#include <QMouseEvent>
#include <QListWidget>

#include <openbabel/mol.h>
#include <openbabel/obiter.h>

using namespace OpenBabel;

KalziumGLWidget::KalziumGLWidget( QWidget * parent )
	: QGLWidget( parent )
{
	m_isDragging = false;
	m_molecule = 0;
	m_detail = 0;
	m_useFog = false;
	m_selectedAtom = 0;

	ChooseStylePreset( PRESET_SPHERES_AND_BICOLOR_BONDS );
	
	setMinimumSize( 100,100 );
}

KalziumGLWidget::~KalziumGLWidget()
{
}

void KalziumGLWidget::initializeGL()
{
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glShadeModel( GL_SMOOTH );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_CULL_FACE );
	glDisable( GL_BLEND );
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glGetDoublev( GL_MODELVIEW_MATRIX, m_RotationMatrix );
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambientLight[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[] = { 0.8, 0.7, 1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_FOG);
	GLfloat fogColor[] = { 0.0, 0.0, 0.0, 1.0 };
	glFogfv( GL_FOG_COLOR, fogColor );
	glFogi( GL_FOG_MODE, GL_LINEAR );
	glFogf( GL_FOG_DENSITY, 0.45 );
	glFogf( GL_FOG_START, 2.7 * m_molRadius );
	glFogf( GL_FOG_END, 5.0 * m_molRadius );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_COLOR_SUM_EXT );
	glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL_EXT,
		GL_SEPARATE_SPECULAR_COLOR_EXT );

	setupObjects();
}

void KalziumGLWidget::paintGL()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 40.0, float( width() ) / height(), m_molRadius, 5.0 * (m_molRadius + atomRadius ()));
	glMatrixMode( GL_MODELVIEW );

	if( !m_molecule )
		return;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// set up the camera
	glLoadIdentity();
	GLTRANSLATE ( 0.0, 0.0, -3.0 * (m_molRadius + atomRadius () ) );
	GLMULTMATRIX ( m_RotationMatrix );

	// set up fog
	if( m_useFog == true )
	{
		glEnable( GL_FOG );
		GLfloat fogColor[] = { 0.0, 0.0, 0.0, 1.0 };
		glFogfv( GL_FOG_COLOR, fogColor );
		glFogi( GL_FOG_MODE, GL_LINEAR );
		glFogf( GL_FOG_DENSITY, 0.45 );
		glFogf( GL_FOG_START, 2.7 * ( m_molRadius + atomRadius() ) );
		glFogf( GL_FOG_END, 5.0 * ( m_molRadius + atomRadius() ) );
	}
	else glDisable( GL_FOG );

	// render the atoms
	if( m_atomStyle == ATOM_SPHERE )
	{
		glEnable( GL_LIGHTING );

		FOR_ATOMS_OF_MOL( a, m_molecule )
		{
			GLFLOAT x = (GLFLOAT) a->GetX();
			GLFLOAT y = (GLFLOAT) a->GetY();
			GLFLOAT z = (GLFLOAT) a->GetZ();
		
			GLColor c = getAtomColor( &*a );
		
			drawSphere(
				x, y, z,
				atomRadius(),
				c);
		}
	}

	// prepare for rendering the bonds
	switch( m_bondStyle )
	{
		case BOND_LINE:
			glDisable( GL_LIGHTING );
			break;
		
		case BOND_CYLINDER_GRAY:
		case BOND_CYLINDER_BICOLOR:
			glEnable( GL_LIGHTING );
			break;
		case BOND_DISABLED: break;
	}

	// render the bonds
	if( BOND_DISABLED != m_bondStyle) FOR_BONDS_OF_MOL( bond, m_molecule )
	{
		GLFLOAT x1 = (GLFLOAT)
			static_cast<OBAtom*>(bond->GetBgn())->GetX();
		GLFLOAT y1 = (GLFLOAT)
			static_cast<OBAtom*>(bond->GetBgn())->GetY();
		GLFLOAT z1 = (GLFLOAT)
			static_cast<OBAtom*>(bond->GetBgn())->GetZ();
		GLFLOAT x2 = (GLFLOAT)
			static_cast<OBAtom*>(bond->GetEnd())->GetX();
		GLFLOAT y2 = (GLFLOAT)
			static_cast<OBAtom*>(bond->GetEnd())->GetY();
		GLFLOAT z2 = (GLFLOAT)
			static_cast<OBAtom*>(bond->GetEnd())->GetZ();
		
		GLFLOAT x3 = (x1 + x2) / 2;
		GLFLOAT y3 = (y1 + y2) / 2;
		GLFLOAT z3 = (z1 + z2) / 2;
		
		GLColor c1, c2;
		c1 = getAtomColor( static_cast<OBAtom*>(bond->GetBgn()) );
		c2 = getAtomColor( static_cast<OBAtom*>(bond->GetEnd()) );
		GLColor gray( 0.5, 0.5, 0.5 );
		
		switch( m_bondStyle )
		{
			case BOND_LINE:
				glBegin( GL_LINES );
				c1.apply();
				glVertex3f( x1, y1, z1 );
				glVertex3f( x3, y3, z3 );
				c2.apply();
				glVertex3f( x3, y3, z3 );
				glVertex3f( x2, y2, z2 );
				glEnd();
				break;
			
			case BOND_CYLINDER_GRAY:
				drawBond( x1, y1, z1, x2, y2, z2, gray );
				break;

			case BOND_CYLINDER_BICOLOR:
				drawBond( x1, y1, z1, x3, y3, z3, c1 );
				drawBond( x2, y2, z2, x3, y3, z3, c2 );
				break;

			case BOND_DISABLED: break;
		}
	}

	// now, paint a semitransparent sphere around the selected atom

	if( m_selectedAtom )
	{
		GLFLOAT x = (GLFLOAT) m_selectedAtom->GetX();
		GLFLOAT y = (GLFLOAT) m_selectedAtom->GetY();
		GLFLOAT z = (GLFLOAT) m_selectedAtom->GetZ();

		GLColor c( 0.4, 0.4, 1.0, 0.7 );

		GLFLOAT radius = m_molMinBondLength * 0.35;
		GLFLOAT min_radius = (GLFLOAT) atomRadius () * 1.25;
		if( radius < min_radius ) radius = min_radius;

		glEnable( GL_BLEND );

		glEnable( GL_LIGHTING );

		drawSphere(
			x, y, z,
			radius,
			c);

		glDisable( GL_BLEND );

	}
}

void KalziumGLWidget::resizeGL( int width, int height )
{
	glViewport( 0, 0, width , height );
}

void KalziumGLWidget::mousePressEvent( QMouseEvent * event )
{
	if( event->buttons() & Qt::LeftButton )
	{	
		m_isDragging = true;
		m_lastDraggingPosition = event->pos ();
	}
}

void KalziumGLWidget::mouseReleaseEvent( QMouseEvent * event )
{
	if( !( event->buttons() & Qt::LeftButton ) )
	{
		m_isDragging = false;
		updateGL();
	}
}

void KalziumGLWidget::mouseMoveEvent( QMouseEvent * event )
{
	if( m_isDragging )
	{
		QPoint deltaDragging = event->pos() - m_lastDraggingPosition;
		m_lastDraggingPosition = event->pos();

		glPushMatrix();
		glLoadIdentity();
		glRotated( deltaDragging.x(), 0.0, 1.0, 0.0 );
		glRotated( deltaDragging.y(), 1.0, 0.0, 0.0 );
		glMultMatrixd( m_RotationMatrix );
		glGetDoublev( GL_MODELVIEW_MATRIX, m_RotationMatrix );
		glPopMatrix();
		updateGL();
	}
}

void KalziumGLWidget::setupObjects()
{
	int sphere_detail, cylinder_faces;

	if( m_atomRadiusCoeff < 0.05) sphere_detail = 1;
	else if( m_atomRadiusCoeff < 0.30) sphere_detail = 2;
	else sphere_detail = 3;

	sphere_detail *= ( m_detail + 1 );

	if( m_bondRadiusCoeff < 0.02) cylinder_faces = 4;
	else if( m_bondRadiusCoeff < 0.10) cylinder_faces = 6;
	else cylinder_faces = 8;

	cylinder_faces *= ( m_detail + 1 );

	m_sphere.setup( sphere_detail, atomRadius() );
	m_cylinder.setup( cylinder_faces, bondRadius() );
}

void KalziumGLWidget::drawSphere( GLdouble x, GLdouble y, GLdouble z,
	GLfloat radius, GLColor &color )
{
	color.applyAsMaterials();
	
	glPushMatrix();
	glTranslated( x, y, z );
	m_sphere.drawScaled( radius );
	glPopMatrix();
}

void KalziumGLWidget::drawBond( FLOAT x1, FLOAT y1, FLOAT z1,
	FLOAT x2, FLOAT y2, FLOAT z2, GLColor &color )
{
	color.applyAsMaterials();

	// the "axis vector" of the cylinder
	GLVector3<FLOAT> axis( x2 - x1, y2 - y1, z2 - z1 );
	
	// find two vectors v, w such that (axis,v,w) is an orthogonal basis.
	GLVector3<FLOAT> v, w;
	axis.construct_ortho_basis_given_first_vector( v, w );

	// normalize v and w. We DON'T want to normalize axis
	v.normalize();
	w.normalize();

	// construct the 4D transformation matrix
	FLOAT matrix[16];

	// column 1
	matrix[0] = v.x;
	matrix[1] = v.y;
	matrix[2] = v.z;
	matrix[3] = 0.0;

	// column 2
	matrix[4] = w.x;
	matrix[5] = w.y;
	matrix[6] = w.z;
	matrix[7] = 0.0;

	// column 3
	matrix[8] = axis.x;
	matrix[9] = axis.y;
	matrix[10] = axis.z;
	matrix[11] = 0.0;

	// column 4
	matrix[12] = x1;
	matrix[13] = y1;
	matrix[14] = z1;
	matrix[15] = 1.0;

	//now we can do the actual drawing !
	glPushMatrix();
	GLMULTMATRIX( matrix );
	m_cylinder.draw();
	glPopMatrix();
}


inline GLFLOAT KalziumGLWidget::bondRadius()
{
	return m_bondRadiusCoeff * m_molMinBondLength;
	
}
inline GLFLOAT KalziumGLWidget::atomRadius()
{
	return m_atomRadiusCoeff * m_molMinBondLength;
}

void KalziumGLWidget::slotSetMolecule( OpenBabel::OBMol* molecule )
{
	if ( !molecule ) return;
	m_molecule = molecule;
	m_selectedAtom = 0;
	prepareMoleculeData();
	setupObjects();
	updateGL();
}

void KalziumGLWidget::ChooseStylePreset( StylePreset stylePreset )
{
	switch( stylePreset )
	{
		case PRESET_LINES:
			m_atomStyle = ATOM_DISABLED;
			m_bondStyle = BOND_LINE;
			m_atomRadiusCoeff = 0.0;
			m_bondRadiusCoeff = 0.0;
			break;
		case PRESET_STICKS:
			m_atomStyle = ATOM_SPHERE;
			m_bondStyle = BOND_CYLINDER_BICOLOR;
			m_atomRadiusCoeff = 0.13;
			m_bondRadiusCoeff = 0.13;
			break;
		case PRESET_SPHERES_AND_GRAY_BONDS:
			m_atomStyle = ATOM_SPHERE;
			m_bondStyle = BOND_CYLINDER_GRAY;
			m_atomRadiusCoeff = 0.20;
			m_bondRadiusCoeff = 0.05;
			break;
		case PRESET_SPHERES_AND_BICOLOR_BONDS:
			m_atomStyle = ATOM_SPHERE;
			m_bondStyle = BOND_CYLINDER_BICOLOR;
			m_atomRadiusCoeff = 0.20;
			m_bondRadiusCoeff = 0.05;
			break;
		case PRESET_BIG_SPHERES:
			m_atomStyle = ATOM_SPHERE;
			m_bondStyle = BOND_DISABLED;
			m_atomRadiusCoeff = 1.0;
			m_bondRadiusCoeff = 0.0;
			break;
	}
	setupObjects();
	updateGL();
}

void KalziumGLWidget::prepareMoleculeData()
{
	// translate the molecule so that center has coords 0,0,0
	m_molecule->Center();

	// calculate the radius of the molecule
	// that is, the maximal distance between an atom of the molecule
	// and the center of the molecule
	m_molRadius = 0.0;
	FOR_ATOMS_OF_MOL( a, m_molecule )
	{
		FLOAT x = (FLOAT) a->GetX();
		FLOAT y = (FLOAT) a->GetY();
		FLOAT z = (FLOAT) a->GetZ();
		FLOAT rad = GLSQRT(x*x + y*y + z*z);
		if( rad > m_molRadius )
			m_molRadius = rad;
	}

	// calculate the length of the shortest bond, of the longest bond
	m_molMinBondLength = 2 * m_molRadius;
	m_molMaxBondLength = 0.0;
	FOR_BONDS_OF_MOL( b, m_molecule )
	{
		FLOAT x1 = (FLOAT) static_cast<OBAtom*>(b->GetBgn())->GetX();
		FLOAT y1 = (FLOAT) static_cast<OBAtom*>(b->GetBgn())->GetY();
		FLOAT z1 = (FLOAT) static_cast<OBAtom*>(b->GetBgn())->GetZ();
		FLOAT x2 = (FLOAT) static_cast<OBAtom*>(b->GetEnd())->GetX();
		FLOAT y2 = (FLOAT) static_cast<OBAtom*>(b->GetEnd())->GetY();
		FLOAT z2 = (FLOAT) static_cast<OBAtom*>(b->GetEnd())->GetZ();
		FLOAT len = GLSQRT ( (x1 - x2) * (x1 - x2)
		                   + (y1 - y2) * (y1 - y2)
		                   + (z1 - z2) * (z1 - z2) );
		if( len > m_molMaxBondLength )
			m_molMaxBondLength = len;
		if( len < m_molMinBondLength )
			m_molMinBondLength = len;
	}
}

void KalziumGLWidget::slotSetDetail( int detail )
{
	m_detail = detail;
	if( m_detail >= 2 ) m_useFog = true;
	else m_useFog = false;
	setupObjects();
	updateGL();
}

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

GLColor& KalziumGLWidget::getAtomColor( OpenBabel::OBAtom* atom )
{
	static GLColor c;

	if ( atom->IsHydrogen() )
	{//white
		c.m_red = 1.0;
		c.m_green = 1.0;
		c.m_blue = 1.0;
	}
	else if ( atom->IsCarbon() )
	{//almost black
		c.m_red = 0.25;
		c.m_green = 0.25;
		c.m_blue = 0.25;
	}
	else if ( atom->IsOxygen() )
	{//red
		c.m_red = 1.0;
		c.m_green = 0.0;
		c.m_blue = 0.0;
	}
	else if ( atom->IsNitrogen() )
	{
		c.m_red = 1.0;
		c.m_green = 0.9;
		c.m_blue = 0.5;
	}
	else if ( atom->IsSulfur() )
	{//yellow
		c.m_red = 1.0;
		c.m_green = 1.0;
		c.m_blue = 0.0;
	}
	else
	{
		c.m_red = 0.5;
		c.m_green = 0.5;
		c.m_blue = 0.5;
	}

	return c;
}


void KalziumGLWidget::slotAtomSelected( OpenBabel::OBAtom* atom )
{
	if ( !atom ) return;
	m_selectedAtom = atom;
	updateGL();
}

inline void GLColor::apply()
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

#include "kalziumglwidget.moc"
