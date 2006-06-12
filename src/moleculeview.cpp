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
#include <kcombobox.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kfiledialog.h>

#include "moleculeview.h"
#include "openbabel2wrapper.h"

#include <QString>
#include <QMouseEvent>
#include <QLayout>
#include <QListWidget>
#include <QFileDialog>

MoleculeWidget::MoleculeWidget( QWidget * parent )
	: KalziumGLWidget( parent )
{
	setMinimumSize( 100,100 );
}

MoleculeWidget::~MoleculeWidget()
{
}
/////////MoleculeDialog/////////

MoleculeDialog::MoleculeDialog( QWidget * parent )
	: KDialog( parent ,
	i18n( "Molecular Viewer" ),
	Help|Close )
{
	
	QWidget * dummy = new QWidget();
	setMainWidget( dummy );

	m_path = QString( "" );

	QHBoxLayout * hbox = new QHBoxLayout;
	QVBoxLayout * vbox = new QVBoxLayout;
	m_moleculeWidget = new MoleculeWidget( dummy );
	m_listView = new QListWidget( );

	//The load-button
	m_loadButton = new KPushButton( i18n("&Load molecule"), this );
	connect( m_loadButton, SIGNAL( clicked() ), this, SLOT( slotLoadMolecule() ) );
	
	//The detail comboxbox
	m_detailCombobox = new KComboBox();
	m_detailCombobox->addItem( i18n( "Low Detail" ) );
	m_detailCombobox->addItem( i18n( "Medium Detail" ) );
	m_detailCombobox->addItem( i18n( "High Detail" ) );
	connect( m_detailCombobox, SIGNAL(activated( int )), m_moleculeWidget, SLOT( slotSetDetail( int ) ) );

	hbox->addLayout( vbox );
	hbox->addWidget( m_moleculeWidget );
	vbox->addWidget( m_listView );
	vbox->addWidget( m_detailCombobox );
	vbox->addWidget( m_loadButton );
	dummy->setLayout( hbox );

	fillList();
}

void MoleculeDialog::slotLoadMolecule()
{
	QString filename = m_listView->currentItem()->text();
	filename.prepend( m_path );
	
	kDebug() << "Filename to load: " << filename << endl;
//X 	
//X 	m_moleculeWidget->slotSetMolecule( OpenBabel2Wrapper::readMolecule( filename ) );
}

void MoleculeDialog::fillList()
{
//X 	QString s = KFileDialog::getExistingDirectory( 
//X 			"/home",
//X 			this,
//X 			"Choose a directory" );
//X 
//X 	m_path = s;
}

MoleculeDialog::~MoleculeDialog( )
{
}

#include "moleculeview.moc"
