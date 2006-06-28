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
#include "kalziumglwidget.h"

#include <QString>
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
	setButtons( Help | Close );
	setDefaultButton( Close );
	
	QWidget * dummy = new QWidget();
	setMainWidget( dummy );

	ui.setupUi( dummy );

	ui.treeWidget->setSelectionMode( QAbstractItemView::MultiSelection );

	m_path = QString( "" );

	connect( ui.qualityCombo, SIGNAL(activated( int )), 
			ui.glWidget , SLOT( slotSetDetail( int ) ) );
	connect( ui.styleCombo, SIGNAL(activated( int )), 
			ui.glWidget , SLOT( slotChooseStylePreset( int ) ) );
	connect( this, SIGNAL( atomsSelected( QList<OpenBabel::OBAtom*> ) ), 
				ui.glWidget, SLOT( slotAtomsSelected( QList<OpenBabel::OBAtom*> ) ) );
	
	connect( ui.selectButton, SIGNAL( clicked() ), 
			this, SLOT(slotAtomsSelected( ) ) );
	connect( ui.loadButton, SIGNAL( clicked() ), 
			this, SLOT(slotLoadMolecule() ) );
	connect( ui.xButton, SIGNAL( clicked() ), 
			this, SLOT(slotUpdateGUI() ) );
	connect( ui.yButton, SIGNAL( clicked() ), 
			this, SLOT(slotUpdateGUI() ) );
	connect( ui.zButton, SIGNAL( clicked() ), 
			this, SLOT(slotUpdateGUI() ) );
	connect( ui.zoomButton, SIGNAL( clicked() ), 
			this, SLOT(slotUpdateGUI() ) );
	connect( ui.measureButton, SIGNAL( clicked() ), 
			this, SLOT(slotUpdateGUI() ) );

	slotLoadMolecule();
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
	ui.glWidget->update();
	updateStatistics();
}

MoleculeDialog::~MoleculeDialog( )
{
}

void MoleculeDialog::updateStatistics()
{
	OpenBabel::OBMol* mol = ui.glWidget->molecule();
	if ( !mol ) return;

	ui.treeWidget->clear();

	ui.nameLabel->setText( mol->GetTitle() );
	ui.weightLabel->setText( QString::number( mol->GetMolWt() ));
	ui.formulaLabel->setText( OpenBabel2Wrapper::getPrettyFormula( mol ) );
	ui.glWidget->update();
		
	QTreeWidgetItem* carbon = new QTreeWidgetItem( ui.treeWidget, QStringList( i18n( "Carbon" ) ) );
	QTreeWidgetItem* oxygen = new QTreeWidgetItem( ui.treeWidget, QStringList( i18n( "Oxygen" ) ) );
	QTreeWidgetItem* hydrogen = new QTreeWidgetItem( ui.treeWidget, QStringList( i18n( "Hydrogen" ) ) );
	QTreeWidgetItem* nitrogen = new QTreeWidgetItem( ui.treeWidget, QStringList( i18n( "Nitrogen" ) ) );
	QTreeWidgetItem* rest = new QTreeWidgetItem( ui.treeWidget, QStringList( i18n( "Rest" ) ) ) ;

	FOR_ATOMS_OF_MOL( a, mol )
	{
 		QStringList content;
		content.append( QString::number( a->GetIdx() ) );
		content.append( QString::number( a->GetExactMass() ) );
 		QTreeWidgetItem* i = new QTreeWidgetItem( content );
		if ( a->IsCarbon() )
	 		carbon->addChild( i );
		else if ( a->IsHydrogen() )
			hydrogen->addChild( i );
		else if ( a->IsOxygen() )
			oxygen->addChild( i );
		else if ( a->IsNitrogen() )
			nitrogen->addChild( i );
		else
			rest->addChild( i );
	}
}

void MoleculeDialog::slotAtomsSelected()
{
	QList<OpenBabel::OBAtom*> atoms;
	OpenBabel::OBMol* molecule = ui.glWidget->molecule();

	QList<QTreeWidgetItem *> itemList = ui.treeWidget->selectedItems();
	foreach( QTreeWidgetItem* item , itemList )
	{
		int id = item->text(0).toInt();
		atoms.append( molecule->GetAtom( id ) );
	}

	emit atomsSelected( atoms );
}

void MoleculeDialog::slotUpdateGUI()
{
	ui.xButton->setEnabled( true );
	ui.yButton->setEnabled( true );
	ui.zButton->setEnabled( true );
	ui.measureButton->setEnabled( true );
	ui.zoomButton->setEnabled( true );
	
	if ( ui.measureButton->isChecked() )
	{
		ui.xButton->setEnabled( false );
		ui.yButton->setEnabled( false );
		ui.zButton->setEnabled( false );
		ui.zoomButton->setEnabled( false );

		ui.glWidget->slotMeasure( true );
	}
	else if ( ui.zoomButton->isChecked() )
	{
		ui.xButton->setEnabled( false );
		ui.yButton->setEnabled( false );
		ui.zButton->setEnabled( false );
		ui.measureButton->setEnabled( false );
		
		ui.glWidget->slotZoom( true );
	}
}

#include "moleculeview.moc"
