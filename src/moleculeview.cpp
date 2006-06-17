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

#include <openbabel/mol.h>
#include <openbabel/obiter.h>

#include "moleculeview.h"
#include "openbabel2wrapper.h"
#include "kalziumglwidget.h"

#include <QString>
#include <QMouseEvent>
#include <QLayout>
#include <QFileDialog>
#include <QDir>

using namespace OpenBabel;

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
}

void MoleculeDialog::slotLoadMolecule()
{
	m_path = "/home/kde4/chemical-structures/amino_acids";

	QString filename = QFileDialog::getOpenFileName( 
			this,
			"Choose a file to open",
			m_path,
			"Molecules (*.cml)" );

	kDebug() << "Filename to load: " << filename << endl;
	
	OpenBabel::OBMol* mol = OpenBabel2Wrapper::readMolecule( filename );
	ui.glWidget->slotSetMolecule( mol );
	updateStatistics();
}

MoleculeDialog::~MoleculeDialog( )
{
}

void MoleculeDialog::updateStatistics()
{
	OpenBabel::OBMol* mol = ui.glWidget->molecule();
	if ( !mol ) return;
	
	ui.nameLabel->setText( mol->GetTitle() );
	ui.weightLabel->setText( QString::number( mol->GetMolWt() ));
	ui.formulaLabel->setText( OpenBabel2Wrapper::getPrettyFormula( mol ) );
	ui.glWidget->update();
	
//X 	FOR_ATOMS_OF_MOL( a, mol )
//X 	{
//X 		QTreeWidgetItem* carbon = new QTreeWidgetItem( ui.treeWidget );
//X 		carbon->setText( 0, i18n( "Carbon" ) );
//X 	}
}


#include "moleculeview.moc"
