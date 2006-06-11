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
#include <kdebug.h>
#include "moleculeview.h"

#include <QMouseEvent>
#include <QLayout>
#include <QListWidget>

//includes for OpenBabel2
#include <openbabel/mol.h>
#include <openbabel/obiter.h>

using namespace OpenBabel;

MoleculeWidget::MoleculeWidget( QWidget * parent )
	: KalziumGLWidget( parent )
{
	setMinimumSize( 100,100 );
//this is how to use OpenBabel
	OBAtom *atom;
	FOR_NBORS_OF_ATOM( nbr, atom )
	{
		double x,y,z;
		x = atom->GetX();
		y = atom->GetY();
		z = atom->GetZ();
	}

//X 	FOR_BONDS_OF_ATOM( bonds, bond )
//X 	{
//X 		atom = bond->GetBeginAtom();
//X 		if ( bond->isTripple() )//draw three lines
//X 		{}
//X 		if ( bond->isDouble() )//draw two lines
//X 		{}
//X 		if ( bond->isSingle() )//one line
//X 		{}
//X 	}
}

MoleculeWidget::~MoleculeWidget()
{
}
//X 
//X void MoleculeWidget::initializeGL()
//X {
//X }
//X 
//X void MoleculeWidget::paintGL()
//X {
//X }
//X 
//X void MoleculeWidget::resizeGL( int width, int height )
//X {
//X }
//X 
//X void MoleculeWidget::mousePressEvent( QMouseEvent * event )
//X {
//X }
//X 
//X void MoleculeWidget::mouseReleaseEvent( QMouseEvent * event )
//X {
//X }
//X 
//X void MoleculeWidget::mouseMoveEvent( QMouseEvent * event )
//X {
//X }
//X 
//X void MoleculeWidget::drawGenericSphere()
//X {
//X }

/////////MoleculeDialog/////////

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
}

MoleculeDialog::~MoleculeDialog( )
{
}

#include "moleculeview.moc"
