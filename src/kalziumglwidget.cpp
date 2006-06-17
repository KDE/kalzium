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
	m_sphereDisplayList = 0;
	m_bondDisplayList = 0;
	m_isDragging = false;
	m_molecule = 0;
	m_detail = 0;
	m_useFog = false;

	slotChooseStylePreset( PRESET_SPHERES_AND_BICOLOR_BONDS );
	
	setMinimumSize( 100,100 );
}

KalziumGLWidget::~KalziumGLWidget()
{
	if( m_sphereDisplayList )
		glDeleteLists( m_sphereDisplayList, 1 );
}

void KalziumGLWidget::initializeGL()
{
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glShadeModel( GL_SMOOTH );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glDisable( GL_BLEND );

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

	glEnable( GL_COLOR_SUM_EXT );
	glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL_EXT,
		GL_SEPARATE_SPECULAR_COLOR_EXT );
}

void KalziumGLWidget::getColor( OBAtom &a, GLfloat &r, GLfloat &g, GLfloat &b )
{
	if ( a.IsOxygen() )
	{//red
		r = 1.0;
		g = 0.0;
		b = 0.0;
	}
	else if ( a.IsSulfur() )
	{//yellow
		r = 1.0;
		g = 1.0;
		b = 0.0;
	}
	else if ( a.IsCarbon() )
	{//almost black
		r = 0.25;
		g = 0.25;
		b = 0.25;
	}
	else if ( a.IsNitrogen() )
	{
		r = 1.0;
		g = 0.9;
		b = 0.5;
	}
	else if ( a.IsHydrogen() )
	{//white
		r = 1.0;
		g = 1.0;
		b = 1.0;
	}
	else
	{
		r = 0.5;
		g = 0.5;
		b = 0.5;
	}
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
		glEnable( GL_NORMALIZE );
		glEnable( GL_LIGHTING );

		FOR_ATOMS_OF_MOL( a, m_molecule )
		{
			FLOAT x = (FLOAT) a->GetX();
			FLOAT y = (FLOAT) a->GetY();
			FLOAT z = (FLOAT) a->GetZ();
	
			GLfloat r, g, b;
	
			getColor( *a, r, g, b );
		
			drawSphere(
				x, y, z,
				atomRadius (),
				r, g, b);
		}

		glDisable( GL_NORMALIZE );
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
		FLOAT x1 = (FLOAT) static_cast<OBAtom*>(bond->GetBgn())->GetX();
		FLOAT y1 = (FLOAT) static_cast<OBAtom*>(bond->GetBgn())->GetY();
		FLOAT z1 = (FLOAT) static_cast<OBAtom*>(bond->GetBgn())->GetZ();
		FLOAT x2 = (FLOAT) static_cast<OBAtom*>(bond->GetEnd())->GetX();
		FLOAT y2 = (FLOAT) static_cast<OBAtom*>(bond->GetEnd())->GetY();
		FLOAT z2 = (FLOAT) static_cast<OBAtom*>(bond->GetEnd())->GetZ();
		
		FLOAT x3 = (x1 + x2) / 2;
		FLOAT y3 = (y1 + y2) / 2;
		FLOAT z3 = (z1 + z2) / 2;
		
		GLfloat r1, g1, b1, r2, g2, b2;
		getColor( *static_cast<OBAtom*>(bond->GetBgn()), r1, g1, b1 );
		getColor( *static_cast<OBAtom*>(bond->GetEnd()), r2, g2, b2 );
		
		switch( m_bondStyle )
		{
			case BOND_LINE:
				glBegin( GL_LINES );
				glColor3f( r1, g1, b1 );
				glVertex3f( x1, y1, z1 );
				glVertex3f( x3, y3, z3 );
				glColor3f( r2, g2, b2 );
				glVertex3f( x3, y3, z3 );
				glVertex3f( x2, y2, z2 );
				glEnd();
				break;
			
			case BOND_CYLINDER_GRAY:
				drawBond( x1, y1, z1, x2, y2, z2, 0.5, 0.5, 0.5 );
				break;

			case BOND_CYLINDER_BICOLOR:
				drawBond( x1, y1, z1, x3, y3, z3, r1, g1, b1 );
				drawBond( x2, y2, z2, x3, y3, z3, r2, g2, b2 );
				break;

			case BOND_DISABLED: break;
		}
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

void KalziumGLWidget::drawGenericSphere()
{
	static int lastDetail = -1;
	if( 0 == m_sphereDisplayList || lastDetail != m_detail)
	{
		if( m_sphereDisplayList )
			glDeleteLists( m_sphereDisplayList, 1 );
		m_sphereDisplayList = glGenLists( 1 );
		if( 0 == m_sphereDisplayList ) return;
		GLUquadricObj *q = gluNewQuadric();
		if( 0 == q) return;
		glNewList( m_sphereDisplayList, GL_COMPILE );
		switch( m_detail)
		{
			case 0:
				gluSphere( q, 1.0, 10, 10 );
				break;
			case 1:
				gluSphere( q, 1.0, 20, 20 );
				break;
			case 2:
				gluSphere( q, 1.0, 40, 40 );
				break;
		}	
		glEndList();
		gluDeleteQuadric( q );
		lastDetail = m_detail;
	}
	
	glCallList( m_sphereDisplayList );
}

void KalziumGLWidget::drawGenericBond()
{
	int slices;
	static int lastDetail = -1;
	static float lastBondRadiusCoeff = -1.0;
	if( 0 == m_bondDisplayList  || lastDetail != m_detail
	 || lastBondRadiusCoeff != m_bondRadiusCoeff)
	{
		m_bondDisplayList = glGenLists( 1 );
		if( 0 == m_bondDisplayList ) return;
		switch( m_detail)
		{
			case 0:
				slices = 10;
				break;
			case 1:
				slices = 10;
				break;
			case 2:
				slices = 20;
				break;
		}	
		glNewList( m_bondDisplayList, GL_COMPILE );
		glBegin( GL_QUADS );
		for (double i = 0.0; i < slices; i++)
		{	
			glNormal3f( cos(2*M_PI * i/slices), sin(2*M_PI * i/slices), 0.0 );
			glVertex3f( cos(2*M_PI * i/slices) * bondRadius(), sin(2*M_PI * i/slices) * bondRadius(), 1.0 );
			glNormal3f( cos(2*M_PI * i/slices), sin(2*M_PI * i/slices), 0.0 );
			glVertex3f( cos(2*M_PI * i/slices) * bondRadius(), sin(2*M_PI * i/slices) * bondRadius(), 0.0 );
			glNormal3f( cos(2*M_PI * (i+1)/slices), sin(2*M_PI * (i+1)/slices), 0.0 );
			glVertex3f( cos(2*M_PI * (i+1)/slices) * bondRadius(), sin(2*M_PI * (i+1)/slices) * bondRadius(), 0.0 );
			glNormal3f( cos(2*M_PI * (i+1)/slices), sin(2*M_PI * (i+1)/slices), 0.0 );
			glVertex3f( cos(2*M_PI * (i+1)/slices) * bondRadius(), sin(2*M_PI * (i+1)/slices) * bondRadius(), 1.0 );
		}
		glEnd();
		glEndList();
		lastDetail = m_detail;
		lastBondRadiusCoeff = m_bondRadiusCoeff;
	}
	
	glCallList( m_bondDisplayList );

}

void KalziumGLWidget::drawSphere( GLdouble x, GLdouble y, GLdouble z,
	GLdouble radius, GLfloat red, GLfloat green, GLfloat blue )
{
	GLfloat ambientColor [] = { red / 2, green / 2, blue / 2, 1.0 };
	GLfloat diffuseColor [] = { red, green, blue, 1.0 };
	GLfloat specularColor [] = { (2.0 + red) / 3, (2.0 + green) / 3,
		(2.0 + blue) / 3, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	
	glPushMatrix();
	glTranslated( x, y, z );
	glScaled( radius, radius, radius);
	drawGenericSphere();
	glPopMatrix();
}

void KalziumGLWidget::drawBond( FLOAT x1, FLOAT y1, FLOAT z1,
	FLOAT x2, FLOAT y2, FLOAT z2,
	GLfloat red, GLfloat green, GLfloat blue )
{
	GLfloat ambientColor [] = { red / 2, green / 2, blue / 2, 1.0 };
	GLfloat diffuseColor [] = { red, green, blue, 1.0 };
	GLfloat specularColor [] = { (2.0 + red) / 3, (2.0 + green) / 3,
		(2.0 + blue) / 3, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

	// the "axis vector" of the cylinder
	FLOAT axis[3] = { x2 - x1, y2 - y1, z2 - z1 };
	
	// find two vectors v, w such that (axis,v,w) is an orthogonal basis.
	FLOAT v[3], w[3];
	construct_ortho_3D_basis_given_first_vector3( axis, v, w );

	// normalize v and w. We DON'T want to normalize axis
	normalize3( v );
	normalize3( w );

	// construct the 4D transformation matrix
	FLOAT matrix[16];

	// column 1
	matrix[0] = v[0];
	matrix[1] = v[1];
	matrix[2] = v[2];
	matrix[3] = 0.0;

	// column 2
	matrix[4] = w[0];
	matrix[5] = w[1];
	matrix[6] = w[2];
	matrix[7] = 0.0;

	// column 3
	matrix[8] = axis[0];
	matrix[9] = axis[1];
	matrix[10] = axis[2];
	matrix[11] = 0.0;

	// column 4
	matrix[12] = x1;
	matrix[13] = y1;
	matrix[14] = z1;
	matrix[15] = 1.0;

	//now we can do the actual drawing !
	glPushMatrix();
	GLMULTMATRIX( matrix );
	drawGenericBond();
	glPopMatrix();
}


inline float KalziumGLWidget::bondRadius()
{
	return m_bondRadiusCoeff * m_molMinBondLength;
	
}
inline float KalziumGLWidget::atomRadius()
{
	return m_atomRadiusCoeff * m_molMinBondLength;
}

void KalziumGLWidget::slotSetMolecule( OpenBabel::OBMol* molecule )
{
	if ( !molecule ) return;
	m_molecule = molecule;
	prepareMoleculeData();
	updateGL();
}

void KalziumGLWidget::slotChooseStylePreset( StylePreset stylePreset )
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
			m_atomRadiusCoeff = 2.4;
			m_bondRadiusCoeff = 0.0;
			break;
	}

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
		FLOAT rad = SQRT(x*x + y*y + z*z);
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
		FLOAT len = SQRT ( (x1 - x2) * (x1 - x2)
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
	updateGL();
}

bool KalziumGLWidget::approx_equal( FLOAT a, FLOAT b, FLOAT precision )
{
	FLOAT abs_a = FABS( a );
	FLOAT abs_b = FABS( b );

	FLOAT max_abs;
	if( abs_a < abs_b )
		max_abs = abs_b;
	else
		max_abs = abs_a;
	return( FABS( a - b ) <= precision * max_abs );
}

FLOAT KalziumGLWidget::norm3( FLOAT *u )
{
	return SQRT( u[0] * u[0] + u[1] * u[1] + u[2] * u[2] );
}

void KalziumGLWidget::normalize3( FLOAT *u )
{
	FLOAT n = norm3( u );
	if( 0 == n ) return;
	u[0] /= n;
	u[1] /= n;
	u[2] /= n;
}

void KalziumGLWidget::construct_ortho_3D_basis_given_first_vector3(
	const FLOAT *U, FLOAT *v, FLOAT *w)
{
	// let us first make a normalized copy of U
	FLOAT u[3];
	u[0] = U[0]; u[1] = U[1]; u[2] = U[2];
	if( 0 == norm3( u ) ) return;
	normalize3( u );

	// initially we set v = u
	v[0] = u[0]; v[1] = u[1]; v[2] = u[2];

	// next we want to change v so that it becomes non-colinear to u
	if( ! approx_equal( v[0], v[1], 0.01 ) )
	{
		FLOAT tmp = v[0];
		v[0] = v[1];
		v[1] = tmp;
	}
	else if( ! approx_equal( v[1], v[2], 0.01 ) )
	{
		FLOAT tmp = v[2];
		v[2] = v[1];
		v[1] = tmp;
	}
	else // the 3 coords of v are approximately equal
	{    // which implies that v is not colinear to (0,0,1)
		v[0] = 0.0; v[1] = 0.0; v[2] = 1.0;
	}

	// now, v is not colinear to u. We compute its dot product with u
	FLOAT u_dot_v = u[0] * v[0] + u[1] * v[1] + u[2] * v[2];

	// now we change v so that it becomes orthogonal to u
	v[0] -= u[0] * u_dot_v;
	v[1] -= u[1] * u_dot_v;
	v[2] -= u[2] * u_dot_v;

	// now that u and v are orthogonal, w can be constructed as
	// their crossed product
	w[0] = u[1] * v[2] - u[2] * v[1];
	w[1] = u[2] * v[0] - u[0] * v[2];
	w[2] = u[0] * v[1] - u[1] * v[0];
}

inline void GLColor::apply()
{
	glColor3fv( reinterpret_cast<GLfloat *>( this ) );
}

void GLColor::applyAsMaterials()
{
	GLfloat ambientColor [] = { red / 2, green / 2, blue / 2, 1.0 };
	GLfloat diffuseColor [] = { red, green, blue, 1.0 };
	GLfloat specularColor [] = { (2.0 + red) / 3, (2.0 + green) / 3,
		(2.0 + blue) / 3, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
}

GLVertexArray::GLVertexArray()
{
	m_vertices = 0;
	m_indices = 0;
}

GLVertexArray::~GLVertexArray()
{
	if( m_vertices ) delete []m_vertices;
	if( m_indices ) delete []m_indices;
}

void GLVertexArray::draw()
{
	if( ! m_vertices ) return;
	if( ! m_indices ) return;
	glInterleavedArrays( GL_N3F_V3F, 0,  m_vertices );
	glDrawElements( GL_TRIANGLE_STRIP, m_nbIndices, GL_UNSIGNED_SHORT, m_indices );
}

SphereVertexArray::SphereVertexArray( unsigned int strips,
	unsigned int lozangesPerStrip ) : GLVertexArray()
{
	regenerate( strips, lozangesPerStrip );
}

SphereVertexArray::~SphereVertexArray()
{
}

void SphereVertexArray::regenerate( unsigned int strips,
	unsigned int lozangesPerStrip )
{
	m_strips = strips;
	m_lozangesPerStrip = lozangesPerStrip;
	if( m_vertices ) delete []m_vertices;
	if( m_indices ) delete []m_indices;
	generate();
}

void SphereVertexArray::generate()
{

}

#include "kalziumglwidget.moc"
