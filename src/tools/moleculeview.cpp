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
#include <kcombobox.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kstandarddirs.h>

#include <openbabel2wrapper.h>

#include <QMouseEvent>
#include <QLayout>
#include <QFileDialog>
#include <QDir>
#include <QStringList>

using namespace OpenBabel;

MoleculeDialog::MoleculeDialog( QWidget * parent )
	: KDialog( parent )
{
	setCaption( i18n( "Molecular Viewer" ) );
	setButtons( Help | User1 | Close );
	setDefaultButton( User1 );
	setButtonGuiItem( User1, KGuiItem( i18n( "Load molecule" ), "open", i18n( "Loading a molecule" ) ) );
	
	ui.setupUi(mainWidget());

	ui.qualityCombo->setCurrentIndex(2); //default to high quality

	m_path = QString( "" );

	connect( ui.qualityCombo, SIGNAL(activated( int )), 
			ui.glWidget , SLOT( setGlobalQualitySetting( int ) ) );
	connect( ui.styleCombo, SIGNAL(activated( int )), 
			ui.glWidget , SLOT( setStyle( int ) ) );

	connect( this, SIGNAL( user1Clicked() ), 
			this, SLOT( slotLoadMolecule() ) );
}

void MoleculeDialog::slotLoadMolecule()
{
        m_path = KGlobal::dirs()->findResourceDir( "appdata", "data/molecules/" ) + "data/molecules/";

	QString filename = QFileDialog::getOpenFileName( 
			this,
			"Choose a file to open",
			m_path,
			"Molecules (*.cml)" );

	if( filename.isEmpty() ) return;

	kDebug() << "Filename to load: " << filename << endl;

	Avogadro::Molecule* molecule = OpenBabel2Wrapper::readMolecule( filename );

	if (molecule->NumAtoms() != 0)
	{
		molecule->Center();
		ui.glWidget->setMolecule( molecule );
		ui.glWidget->update();
		updateStatistics();
	}
}

MoleculeDialog::~MoleculeDialog( )
{
}

void MoleculeDialog::updateStatistics()
{
	Avogadro::Molecule* mol = ui.glWidget->molecule();
	if ( !mol ) return;

	ui.nameLabel->setText( mol->GetTitle() );
	ui.weightLabel->setText( QString::number( mol->GetMolWt() ));
	ui.formulaLabel->setText( OpenBabel2Wrapper::getPrettyFormula( mol ) );
	ui.glWidget->update();
}

#include "moleculeview.moc"
