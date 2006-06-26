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
#include "kalziumglhelperclasses.h"

#include <math.h>

#include <kdebug.h>

#include <QMouseEvent>
#include <QListWidget>

#ifdef USE_FPS_COUNTER
#include <QTime>
#endif

#include <openbabel/mol.h>
#include <openbabel/obiter.h>

using namespace OpenBabel;

KalziumGLWidget::KalziumGLWidget( QWidget * parent )
	: QGLWidget( parent )
{
	m_displayList = 0;
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

void KalziumGLWidget::deleteDisplayList()
{
#ifdef USE_DISPLAY_LIST
	if( m_displayList) glDeleteLists( m_displayList, 1 );
	m_displayList = 0;
#endif
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

#ifdef USE_DISPLAY_LIST
	if ( ! m_displayList )
	{
	m_displayList = glGenLists( 1 );
	glNewList( m_displayList, GL_COMPILE );
#endif

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

#ifdef USE_DISPLAY_LIST
	glEndList();
	}

	glCallList( m_displayList );
#endif

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
#ifdef USE_FPS_COUNTER
	QTime t;

	static bool firstTime = true;
	static int old_time, new_time;
	static int frames;
	static QString s;

	if( firstTime )
	{
		t.start();
		firstTime = false;
		old_time = t.elapsed();
		frames = 0;
	}

	new_time = t.elapsed();
	
	frames++;

	if( new_time - old_time > 200 )
	{
		s = QString::number( 1000 * frames /
			double( new_time - old_time ),
			'f', 1 );
		s += " frames per second";
		frames = 0;
		old_time = new_time;
	}

	renderText ( 20, 20, s );

	update();
#endif
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

	deleteDisplayList();
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
//	axis.construct_ortho_basis_given_first_vector( v, w );

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
	//m_molecule->Center();
	std::map<std::string, std::string> m;
	m["c"] = "c";
	m_molecule->DoTransformations(&m);


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

void KalziumGLWidget::slotAtomsSelected( QList<OpenBabel::OBAtom*> atoms )
{

}

void KalziumGLWidget::slotAtomSelected( OpenBabel::OBAtom* atom )
{
	if ( !atom ) return;
	m_selectedAtom = atom;
	updateGL();
}

#include "kalziumglwidget.moc"
