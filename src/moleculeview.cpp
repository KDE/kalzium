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
			ui.glWidget , SLOT( slotSetMolStyle( int ) ) );
	connect( this, SIGNAL( atomsSelected( QList<OpenBabel::OBAtom*> ) ), 
				ui.glWidget, SLOT( slotAtomsSelected( QList<OpenBabel::OBAtom*> ) ) );
	
	connect( this, SIGNAL( atomsSelected( QList<OpenBabel::OBAtom*> ) ), 
				this, SLOT( slotCalculate( QList<OpenBabel::OBAtom*> ) ) );
	connect( ui.glWidget, SIGNAL( atomsSelected( QList<OpenBabel::OBAtom*> ) ), 
				this, SLOT( slotCalculate( QList<OpenBabel::OBAtom*> ) ) );
	
	connect( ui.selectButton, SIGNAL( clicked() ), 
			this, SLOT(slotAtomsSelected( ) ) );
	connect( ui.loadButton, SIGNAL( clicked() ), 
			this, SLOT(slotLoadMolecule() ) );
//X 	connect( ui.xyButton, SIGNAL( clicked() ), 
//X 			this, SLOT(slotUpdateGUI() ) );
//X 	connect( ui.rotationButton, SIGNAL( clicked() ), 
//X 			this, SLOT(slotUpdateGUI() ) );
//X 	connect( ui.zButton, SIGNAL( clicked() ), 
//X 			this, SLOT(slotUpdateGUI() ) );
//X 	connect( ui.zoomButton, SIGNAL( clicked() ), 
//X 			this, SLOT(slotUpdateGUI() ) );
//X 	connect( ui.measureButton, SIGNAL( clicked() ), 
//X 			this, SLOT(slotUpdateGUI() ) );

//	slotLoadMolecule();
}

void MoleculeDialog::slotLoadMolecule()
{
	m_path = "/home/kde4/chemical-structures/amino_acids";

	QString filename = QFileDialog::getOpenFileName( 
			this,
			"Choose a file to open",
			m_path,
			"Molecules (*.cml)" );

	if( filename.isEmpty() ) return;

	kDebug() << "Filename to load: " << filename << endl;

	OpenBabel::OBMol* mol = OpenBabel2Wrapper::readMolecule( filename );
	mol->Center();
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
//X 	ui.xButton->setEnabled( true );
//X 	ui.yButton->setEnabled( true );
//X 	ui.zButton->setEnabled( true );
//X 	ui.measureButton->setEnabled( true );
//X 	ui.zoomButton->setEnabled( true );
//X 	
//X 	if ( ui.measureButton->isChecked() )
//X 	{
//X 		ui.xButton->setEnabled( false );
//X 		ui.yButton->setEnabled( false );
//X 		ui.zButton->setEnabled( false );
//X 		ui.zoomButton->setEnabled( false );
//X 
//X 		ui.glWidget->slotMeasure( true );
//X 	}
//X 	else if ( ui.zoomButton->isChecked() )
//X 	{
//X 		ui.xButton->setEnabled( false );
//X 		ui.yButton->setEnabled( false );
//X 		ui.zButton->setEnabled( false );
//X 		ui.measureButton->setEnabled( false );
//X 		
//X 		ui.glWidget->slotZoom( true );
//X 	}
}

void MoleculeDialog::slotCalculate( QList<OpenBabel::OBAtom*> atoms )
{
	//if 2 atoms are selected: calculate the distance	
	//if 3 atoms are selected: calculate the angle	
	//if 4 atoms are selected: calculate the torsion	
	//if more than 4 atoms are selected: do nothing

	if ( atoms.count() < 2 || atoms.count() > 4 )
		return;
	
	OpenBabel::OBAtom* a1 = NULL;
	OpenBabel::OBAtom* a2 = NULL;
	OpenBabel::OBAtom* a3 = NULL;
	OpenBabel::OBAtom* a4 = NULL;
		
	OpenBabel::OBMol* mol = ui.glWidget->molecule();
	double d = 0.0;
	double a = 0.0;
	double t = 0.0;

	if ( atoms.count() == 2 )
	{//calculate the distance
		a1 = atoms.at( 0 );
		a1 = atoms.at( 1 );
		if ( a1 && a2 )
			d = a1->GetDistance( a2 );
	}
	else if ( atoms.count() == 3 )
	{//calculate the angle
		//the next line needs OpenBabel-trunk from June 29
		//or later to compile.
		a1 = atoms.at( 0 );
		a1 = atoms.at( 1 );
		a2 = atoms.at( 2 );
		a = 0;//mol->GetAngle( a1, a2, a3 );
	}
	else if ( atoms.count() == 4 )
	{//calculate the torsion
		if ( a1 && a2 && a3 && a4 )
		a1 = atoms.at( 0 );
		a2 = atoms.at( 1 );
		a3 = atoms.at( 2 );
		a4 = atoms.at( 3 );
		t = mol->GetTorsion( a1, a2, a3, a4 );
	}

	kDebug() << "Distance: " << d << " Angstrom" << endl;
	kDebug() << "Angle: " << a << " Degree" << endl;
	kDebug() << "Torsion: " << t << " Degree" << endl;
}

#include "moleculeview.moc"
