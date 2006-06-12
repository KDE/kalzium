/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
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

#include <kdebug.h>


#include <QMouseEvent>
#include <QListWidget>

#include "openbabel2wrapper.h"
#include <openbabel/mol.h>
#include <openbabel/obiter.h>

using namespace OpenBabel;

KalziumGLWidget::KalziumGLWidget( QWidget * parent )
	: QGLWidget( parent )
{
	sphereDisplayList = 0;
	m_molecule = 0;
	m_detail = 2;
	isDragging = false;
	
	setMinimumSize( 100,100 );
}

KalziumGLWidget::~KalziumGLWidget()
{
	if( sphereDisplayList )
		glDeleteLists( sphereDisplayList, 1 );
}

void KalziumGLWidget::initializeGL()
{
	glClearColor( 0.0, 0.0, 0.0, 1.0);
	glShadeModel( GL_SMOOTH );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glEnable( GL_NORMALIZE );
	glDisable( GL_BLEND );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glGetDoublev( GL_MODELVIEW_MATRIX, RotationMatrix );
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambientLight[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[] = { 0.6, 0.5, 1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_FOG);
	GLfloat fogColor[] = { 0.0, 0.0, 0.0, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_DENSITY, 0.4);
	glFogf(GL_FOG_START, 11.0);
	glFogf(GL_FOG_END, 17.0);

	glEnable (GL_COLOR_SUM_EXT);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_EXT, GL_SEPARATE_SPECULAR_COLOR_EXT);
}

void KalziumGLWidget::paintGL()
{
	if ( !m_molecule )
		return;
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glTranslated( 0.0, 0.0, -12.0);
	glMultMatrixd( RotationMatrix );

	FOR_ATOMS_OF_MOL( a, m_molecule )
	{
		GLdouble x = ( GLdouble )a->GetX();
		GLdouble y = ( GLdouble )a->GetY();
		GLdouble z = ( GLdouble )a->GetZ();

		double r = 0.5;
		double g = 0.5;
		double b = 0.5;

		if ( a->IsOxygen() )
		{//red
			r = 1.0;
			g = 0.0;
			b = 0.0;
		}
		else if ( a->IsSulfur() )
		{//yellow
			r = 1.0;
			g = 1.0;
			b = 0.0;
		}//almost black
		else if ( a->IsCarbon() )
		{
			r = 0.95;
			g = 0.95;
			b = 0.95;
		}
		
		double radius = 1.0;
		drawSphere(
				x, y, z,
				radius, 
				r, g, b);
	}
}

void KalziumGLWidget::resizeGL( int width, int height )
{
	glViewport( 0, 0, width, height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0, float(width) / height, 0.5, 50.0 );
	glMatrixMode( GL_MODELVIEW );
}

void KalziumGLWidget::mousePressEvent( QMouseEvent * event )
{
	if( event->buttons () & Qt::LeftButton )
	{	
		isDragging = true;
		lastDraggingPosition = event->pos ();
	}
}

void KalziumGLWidget::mouseReleaseEvent( QMouseEvent * event )
{
	if( !( event->buttons () & Qt::LeftButton ) )
	{
		isDragging = false;
	}
}

void KalziumGLWidget::mouseMoveEvent( QMouseEvent * event )
{
	if( isDragging )
	{
		deltaDragging = event->pos() - lastDraggingPosition;
		lastDraggingPosition = event->pos();

		glPushMatrix();
		glLoadIdentity();
		glRotated( deltaDragging.x(), 0.0, 1.0, 0.0 );
		glRotated( deltaDragging.y(), 1.0, 0.0, 0.0 );
		glMultMatrixd( RotationMatrix );
		glGetDoublev( GL_MODELVIEW_MATRIX, RotationMatrix );
		glPopMatrix();
		updateGL();
	}
}

void KalziumGLWidget::drawGenericSphere()
{
	if( 0 == sphereDisplayList )
	{
		sphereDisplayList = glGenLists( 1 );
		if( 0 == sphereDisplayList ) return;
		GLUquadricObj *q = gluNewQuadric();
		if( 0 == q) return;
		glNewList( sphereDisplayList, GL_COMPILE );
		gluSphere( q, 1.0, SPHERE_TESSELATE_SLICES,  SPHERE_TESSELATE_STACKS );
		glEndList();
		gluDeleteQuadric( q );
	}
	else
	{
		glCallList( sphereDisplayList );
	}
}

void KalziumGLWidget::drawSphere( GLdouble x, GLdouble y, GLdouble z, GLdouble radius,
	GLfloat red, GLfloat green, GLfloat blue )
{
	GLfloat ambientColor [] = { red / 2, green / 2, blue / 2, 1.0 };
	GLfloat diffuseColor [] = { red, green, blue, 1.0 };
	GLfloat specularColor [] = { (2.0 + red) / 3, (2.0 + green) / 3, (2.0 + blue) / 3, 1.0 };
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

void KalziumGLWidget::slotSetMolecule( OpenBabel::OBMol* molecule )
{
	m_molecule = molecule;
}

void KalziumGLWidget::slotSetDetail( int detail )
{
	m_detail = detail;
}

#include "kalziumglwidget.moc"
