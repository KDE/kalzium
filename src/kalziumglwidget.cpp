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
#include <klocale.h>

#include <QMouseEvent>
#include <QListWidget>

#ifdef USE_FPS_COUNTER
#include <QTime>
#endif

#include <openbabel/mol.h>

using namespace KalziumGLHelpers;
using namespace OpenBabel;

KalziumGLWidget::KalziumGLWidget( QWidget * parent )
	: QGLWidget( parent )
{
	m_isDragging = false;
	m_molecule = 0;
	m_detail = 0;
	m_displayList = 0;
	m_haveToRecompileDisplayList = true;
	m_useFog = false;
	m_inZoom = false;
	m_inMeasure = false;

	QFont f;
	f.setStyleHint( QFont::SansSerif, QFont::PreferAntialias );
	m_textRenderer.setup( this, f );
	
	setMinimumSize( 100,100 );
	setMolStyle( 0 );
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

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glGetDoublev( GL_MODELVIEW_MATRIX, m_RotationMatrix );
	glPopMatrix();

	glEnable( GL_NORMALIZE );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	GLfloat ambientLight[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[] = { 0.8, 0.7, 1.0, 0.0 };

	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularLight );
	glLightfv( GL_LIGHT0, GL_POSITION, position );

	GLfloat fogColor[] = { 0.0, 0.0, 0.0, 1.0 };
	glFogfv( GL_FOG_COLOR, fogColor );
	glFogi( GL_FOG_MODE, GL_LINEAR );
	glFogf( GL_FOG_DENSITY, 0.45 );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_COLOR_SUM_EXT );
	glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL_EXT,
		GL_SEPARATE_SPECULAR_COLOR_EXT );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );

	setupObjects();
}

void KalziumGLWidget::paintGL()
{
	if( ! m_molecule )
	{
		glClear( GL_COLOR_BUFFER_BIT );
		glColor3f( 0.0, 1.0, 0.6 );
		m_textRenderer.print( 20, height() - 40,
			i18n("Please load a molecule") );
		return;
	}

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 40.0, float( width() ) / height(),
		getMolRadius(), 5.0 * getMolRadius() );
	glMatrixMode( GL_MODELVIEW );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// set up the camera
	glLoadIdentity();
	glTranslated ( 0.0, 0.0, -3.0 * getMolRadius() );
	glMultMatrixd ( m_RotationMatrix );

	// set up fog
	if( m_useFog )
	{
		glEnable( GL_FOG );
		GLfloat fogColor[] = { 0.0, 0.0, 0.0, 1.0 };
		glFogfv( GL_FOG_COLOR, fogColor );
		glFogi( GL_FOG_MODE, GL_LINEAR );
		glFogf( GL_FOG_DENSITY, 0.45 );
		glFogf( GL_FOG_START, 2.7 * getMolRadius() );
		glFogf( GL_FOG_END, 5.0 * getMolRadius()  );
	}
	else glDisable( GL_FOG );

#ifdef USE_DISPLAY_LISTS
	if( m_haveToRecompileDisplayList )
	{
	if( ! m_displayList ) m_displayList = glGenLists( 1 );
	if( ! m_displayList ) return;
	glNewList( m_displayList, GL_COMPILE );
#endif

	renderAtoms();
	renderBonds();

#ifdef USE_DISPLAY_LISTS
	glEndList();
	m_haveToRecompileDisplayList = false;
	}
	glCallList( m_displayList );
#endif
	renderSelection();

#ifdef USE_FPS_COUNTER
	FPSCounter();
	update();
#endif
}

void KalziumGLWidget::renderAtoms()
{
	if( m_molStyle.m_atomStyle != MolStyle::ATOMS_DISABLED )
	{
		FOR_ATOMS_OF_MOL( atom, m_molecule )
		{
			drawAtom( &*atom );
		}
	}
}

void KalziumGLWidget::renderBonds()
{
	if( m_molStyle.m_bondStyle != MolStyle::BONDS_DISABLED )
	{
		FOR_BONDS_OF_MOL( bond, m_molecule )
		{
			drawBond( &*bond );
		}
	}
}

void KalziumGLWidget::renderSelection()
{
	if( ! m_selectedAtoms.count() ) return;

	Color c( 1.0, 1.0, 1.0, 0.5 );
	glEnable( GL_BLEND );
	foreach(OpenBabel::OBAtom* atom, m_selectedAtoms)
	{
		c.applyAsMaterials();
		m_sphere.draw( atom->GetVector(),
			0.18 + m_molStyle.getAtomRadius( atom ) );
	}
	glDisable( GL_BLEND );
}

void KalziumGLWidget::FPSCounter()
{
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
		s += QString(" FPS");
		frames = 0;
		old_time = new_time;
	}

	glColor3f( 1.0, 1.0, 0.0 );
	m_textRenderer.print( 20, 20, s );
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

void KalziumGLWidget::rotate( )
{
// OK, let's momentarily disable that until I get it working (Benoit)

/*
	kDebug() << "KalziumGLWidget::rotate()" << endl;
	//TODO at this place we need a nice way to rotate
	//based on certain values. For example, we could use two
	//bool variables for x and y rotation. If x is true the 
	//molecule will rotate in the x-axis, if false not. Same
	//for y. 
	//As I have no idea what this code is doing I just copy&pasted
	//everything from the mousewheel method...
	glPushMatrix();
	glLoadIdentity();

	//Benoit, I took those values pretty much at random,
	//no idea what value is for what... :)
	glRotated( 10.0, 0.0, 1.0, 0.0 );
	glRotated( 10.0, 1.0, 0.0, 0.0 );
	glMultMatrixd( m_RotationMatrix );
	glGetDoublev( GL_MODELVIEW_MATRIX, m_RotationMatrix );
	glPopMatrix();
	updateGL();
*/
}

void KalziumGLWidget::setupObjects()
{
	int sphere_detail = 1, cylinder_faces = 2;

	double typicalAtomRadius = m_molStyle.getAtomRadius( 6 );
	double typicalBondRadius = m_molStyle.m_singleBondRadius;

	if( m_molStyle.m_atomStyle != MolStyle::ATOMS_DISABLED )
	{
		if( typicalAtomRadius < 0.50 )
			sphere_detail = 2 + 2 * m_detail;
		else if( typicalAtomRadius < 1.00 )
			sphere_detail = 3 + 2 * m_detail;
		else sphere_detail = 4 + 3 * m_detail;
	}

	if( m_molStyle.m_bondStyle != MolStyle::BONDS_DISABLED )
	{
		if( typicalBondRadius < 0.10 )
			cylinder_faces = 6 + 6 * m_detail;
		else if( typicalBondRadius < 0.20 )
			cylinder_faces = 8 + 8 * m_detail;
		else cylinder_faces = 10 + 8 * m_detail;
	}

	m_sphere.setup( sphere_detail );
	m_cylinder.setup( cylinder_faces );
}

void KalziumGLWidget::drawAtom( OBAtom *atom )
{
	Color( atom ).applyAsMaterials();
	m_sphere.draw( atom->GetVector(), m_molStyle.getAtomRadius( atom ) );
}

void KalziumGLWidget::drawBond( OBBond *bond )
{
	OBAtom *atom1 = static_cast<OBAtom *>( bond->GetBgn() );
	OBAtom *atom2 = static_cast<OBAtom *>( bond->GetEnd() );

	vector3 v1 = atom1->GetVector();
	vector3 v2 = atom2->GetVector();
	vector3 v3 = ( v1 + v2 ) / 2;

	int order;
	if( m_molStyle.m_renderMultipleBonds == false || bond->IsSingle() )
		order = 1;
	else if( bond->IsDouble() ) order = 2;
	else if( bond->IsTriple() ) order = 3;
	else order = bond->GetBondOrder();

	double radius;
	if( order == 1 ) radius = m_molStyle.m_singleBondRadius;
	else radius = m_molStyle.m_multipleBondRadius;

	switch( m_molStyle.m_bondStyle )
	{
		case MolStyle::BONDS_GRAY:
			Color( 0.55, 0.55, 0.55 ).applyAsMaterials();
			m_cylinder.draw( v1, v2, radius, order,
				m_molStyle.m_multipleBondShift );
			break;

		case MolStyle::BONDS_USE_ATOMS_COLORS:
			Color( atom1 ).applyAsMaterials();
			m_cylinder.draw( v1, v3, radius, order,
				m_molStyle.m_multipleBondShift );
			Color( atom2 ).applyAsMaterials();
			m_cylinder.draw( v2, v3, radius, order,
				m_molStyle.m_multipleBondShift );
			break;

		default: break;
	}
}

void KalziumGLWidget::slotZoomIn()
{
	//TODO
	//This slot can be very easily accessed by simply calling it from
	//the GUI. I guess we need a second pair of zoomin/out slots for 
	//a more finegrained zooming. For example, if we use the mousewheel
	//for zooming, we might want to use the delta()-value of the mouse-
	//wheel as a factor.
	//But as I have no idea how zooming works in OpenGL I cannot do the
	//coding...
}

void KalziumGLWidget::slotZoomOut()
{
	//TODO
	//Comment so slotZoomIn()
}

void KalziumGLWidget::slotSetMolecule( OpenBabel::OBMol* molecule )
{
	if ( !molecule ) return;
	m_molecule = molecule;
	m_haveToRecompileDisplayList = true;
	m_selectedAtoms.clear();
	prepareMoleculeData();
	setupObjects();
	updateGL();
}

void KalziumGLWidget::setMolStyle( int style )
{
	switch( style )
	{
		case 0: // sticks-style
			m_molStyle = MolStyle( MolStyle::BONDS_USE_ATOMS_COLORS,
				MolStyle::ATOMS_USE_FIXED_RADIUS,
				0.20, false, 0.06, 0.14, 0.20 );
			break;
		case 1: // atoms: smaller van der Waals, bonds: gray
			m_molStyle = MolStyle( MolStyle::BONDS_GRAY,
				MolStyle::ATOMS_USE_VAN_DER_WAALS_RADIUS,
				0.08, true, 0.08, 0.14, 0.20 );
			break;
		case 2: // atoms: smaller van der Waals, bonds: use atom colors
			m_molStyle = MolStyle( MolStyle::BONDS_USE_ATOMS_COLORS,
				MolStyle::ATOMS_USE_VAN_DER_WAALS_RADIUS,
				0.08, true, 0.08, 0.14, 0.20 );
			break;
		case 3: // atoms: real van der Waals, bonds: disabled
			m_molStyle = MolStyle( MolStyle::BONDS_DISABLED,
				MolStyle::ATOMS_USE_VAN_DER_WAALS_RADIUS,
				0.00, false, 0.00, 0.00, 1.00 );
			break;

		default: break;
	}
	m_haveToRecompileDisplayList = true;
}

void KalziumGLWidget::slotSetMolStyle( int style )
{
	setMolStyle( style );
	setupObjects();
	updateGL();
}

void KalziumGLWidget::prepareMoleculeData()
{
	//Center the molecule
	m_molecule->Center();

	// calculate the radius of the molecule without the electrons
	// that is, the maximal distance between the center of an atom
	// of the molecule and the center of the molecule
	m_molRadiusWithoutElectrons = 0.0;
	FOR_ATOMS_OF_MOL( a, m_molecule )
	{
		vector3 v = a->GetVector();
		double rad = v.length();
		if( rad > m_molRadiusWithoutElectrons )
			m_molRadiusWithoutElectrons = rad;
	}
}

double KalziumGLWidget::getMolRadius()
{
	return m_molRadiusWithoutElectrons + m_molStyle.getAtomRadius( 6 );
}

void KalziumGLWidget::slotSetDetail( int detail )
{
	m_detail = detail;
	if( m_detail >= 2 ) m_useFog = true;
	else m_useFog = false;
	setupObjects();
	updateGL();
}

void KalziumGLWidget::slotAtomsSelected( QList<OpenBabel::OBAtom*> atoms )
{
	kDebug() << "KalziumGLWidget::slotAtomsSelected() with " << atoms.count() << " atoms" << endl;
	m_selectedAtoms = atoms;
	updateGL();
}

#include "kalziumglwidget.moc"
