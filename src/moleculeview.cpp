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
#include "moleculeview.h"

#include <QVBoxLayout>

MoleculeWidget::MoleculeWidget( QWidget * parent )
	: QGLWidget( parent )
{
}

MoleculeWidget::~MoleculeWidget()
{
}
		
void MoleculeWidget::initializeGL()
{
	qglClearColor( Qt::white );
	glShadeModel( GL_FLAT );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
}

void MoleculeWidget::paintGL()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

}

void MoleculeWidget::resizeGL( int width, int height )
{
	int side = qMin( width, height );
	glViewport( ( width - side ) / 2, ( height - side ) / 2, side, side );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -0.5, +0.5, +0.5, -0.5, 4.0, 15.0 );
	glMatrixMode( GL_MODELVIEW );

}

MoleculeDialog::MoleculeDialog( QWidget * parent )
	: KDialog( parent )
{
	MoleculeWidget * w = new MoleculeWidget( this );
	QVBoxLayout * vbox = new QVBoxLayout( this );
	vbox->addWidget( w );
}

MoleculeDialog::~MoleculeDialog( )
{
}

#include "moleculeview.moc"
