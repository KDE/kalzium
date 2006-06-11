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

#include <kdebug.h>
#include <kurl.h>
#include <klocale.h>
#include <kfiledialog.h>

#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>

MoleculeWidget::MoleculeWidget( QWidget * parent )
	: QGLWidget( parent )
{
	initializeGL();
	setMinimumSize( 100,100 );
}

MoleculeWidget::~MoleculeWidget()
{
}
		
void MoleculeWidget::initializeGL()
{
	qglClearColor( Qt::red );
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
	: KDialog( parent ,
	i18n( "Molecular Viewer" ),
	Help|Close )
{
	QWidget * dummy = new QWidget();
	setMainWidget( dummy );

	QHBoxLayout * hbox = new QHBoxLayout;
	QVBoxLayout * vbox = new QVBoxLayout;
	m_moleculeWidget = new MoleculeWidget( dummy );
	m_listView = new QListWidget( );

	hbox->addLayout( vbox );
	hbox->addWidget( m_moleculeWidget );
	vbox->addWidget( m_listView );
	dummy->setLayout( hbox );

	fillList();
}

void MoleculeDialog::fillList()
{
//X 	KUrl url = KFileDialog::getOpenURL( "/home/", "*.cml", 0, "Select a folder" );
//X 	QString path = url.path();
//X 
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
//X 	m_listView->addItem( path );
}

MoleculeDialog::~MoleculeDialog( )
{
}

#include "moleculeview.moc"
