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
#include <QDir>

MoleculeDialog::MoleculeDialog( QWidget * parent )
	: KDialog( parent )
{
	setCaption( i18n( "Molecular Viewer" ) );
	setButtons( Help | Close );
	setDefaultButton( Close );
	
	QWidget * dummy = new QWidget();
	setMainWidget( dummy );

	ui.setupUi( dummy );

	m_path = QString( "" );

 	connect( ui.loadButton, SIGNAL( clicked() ), this, SLOT( slotLoadMolecule() ) );
	connect( ui.qualityCombo, SIGNAL(activated( int )), ui.glWidget , SLOT( slotSetDetail( int ) ) );
	connect( ui.moleculeList, SIGNAL( itemDoubleClicked (  QListWidgetItem* ) ), this, SLOT( slotLoadMolecule( QListWidgetItem* ) ) );
	fillList();
}

void MoleculeDialog::slotLoadMolecule( QListWidgetItem * item )
{
	QString filename = item->text();
	filename.prepend( m_path  + "/");
	
	kDebug() << "Filename to load: " << filename << endl;
	ui.glWidget->slotSetMolecule( OpenBabel2Wrapper::readMolecule( filename ) );
}
	
void MoleculeDialog::slotLoadMolecule()
{
	slotLoadMolecule( ui.moleculeList->currentItem() );
}

void MoleculeDialog::fillList()
{
//X 	QString s = QFileDialog::getExistingDirectory( 
//X 			this,
//X 			"Choose a directory",
//X 			"/home/kde4/chemical-structures/amino_acids",
//X 			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
//X 
//X 	kDebug() << "Path: " << s <<  endl;
	
	m_path = "/home/kde4/chemical-structures/amino_acids";
	QDir dir( m_path );
	
	QStringList ff;
	ff << "*.cml";
	
	QStringList list = dir.entryList( ff, QDir::Readable | QDir::Files );

	foreach( QString filename, list )
		ui.moleculeList->addItem( filename );
}

MoleculeDialog::~MoleculeDialog( )
{
}

#include "moleculeview.moc"
