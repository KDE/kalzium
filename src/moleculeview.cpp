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
//X 	connect( ui::qualityCombo, SIGNAL(activated( int )), ui::moleculeWidget, SLOT( slotSetDetail( int ) ) );
	fillList();
}

void MoleculeDialog::slotLoadMolecule()
{
//X 	QString filename = m_listView->currentItem()->text();
//X 	filename.prepend( m_path );
//X 	
//X 	kDebug() << "Filename to load: " << filename << endl;
//X 	
//X 	ui.moleculeWidget->slotSetMolecule( OpenBabel2Wrapper::readMolecule( filename ) );
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
