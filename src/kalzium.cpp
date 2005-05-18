/***************************************************************************
    copyright            : (C) 2003, 2004, 2005 by Carsten Niehaus
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
#include "kalzium.h"
#include "prefs.h"
#include "settings_colors.h"
#include "settings_misc.h"
#include "elementdataviewer.h"
#include "molcalc_impl.h"
#include "detailinfodlg.h"
#include "informationdialog_impl.h"
#include "pse.h"
#include "glossarydialog.h"

#include <qinputdialog.h>
#include <qlayout.h>
#include <qslider.h>
#include <qlcdnumber.h>
#include <qbuttongroup.h>

#include <kconfigdialog.h>
#include <klocale.h>
#include <kdebug.h>
#include <kaction.h>
#include <knuminput.h>
#include <kpushbutton.h>
#include <kapplication.h>
#include <kstatusbar.h>
#include <knuminput.h>
#include <kpushbutton.h>

#define IDS_TEMP           1
#define IDS_ENERG           2
#define IDS_TEMPERATURE    3

Kalzium::Kalzium()
    : KMainWindow( 0, "Kalzium" )
{
	pd = new privatedata( this );

	pd->kalziumData = new KalziumDataObject();

	QWidget *CentralWidget = new QWidget( this, "CentralWidget" );
	m_pCentralLayout = new QVBoxLayout( CentralWidget, 0, -1, "CentralLayout" );
	
	m_PSE = new PSE( data(), CentralWidget, "PSE");

	connect( m_PSE, SIGNAL( ElementClicked( int ) ), this, SLOT( openInformationDialog( int ) ));
	connect( this, SIGNAL( tableLocked( bool ) ), m_PSE, SLOT( slotLock(bool ) ));
	
	// Layouting
	m_pCentralLayout->addWidget( m_PSE );

	setCentralWidget( CentralWidget );
	CentralWidget->show();	
	
	setupStatusBar();
	setupActions();
}

void Kalzium::setupActions()
{
	/*
	 * the actions for switching PSE
	 **/
	QStringList schemalist;
	schemalist.append(i18n("Show &Simple PSE"));
	schemalist.append(i18n("Show &Regular PSE"));
	schema_action = new KSelectAction (i18n("&PSE"), 0, this, 0, actionCollection(), "change_pse");
	schema_action->setItems(schemalist);
	schema_action->setCurrentItem(Prefs::schemaPSE());
	connect (schema_action, SIGNAL(activated(int)), this, SLOT(slotSwitchtoPSE(int)));

	/*
	 * the actions for switching PSE
	 **/
	QStringList numlist;
	numlist.append(i18n("No N&umeration"));
	numlist.append(i18n("Show &IUPAC"));
	numlist.append(i18n("Show &CAS"));
	numlist.append(i18n("Show &Old IUPAC"));
	numeration_action = new KSelectAction (i18n("&Numeration"), 0, this, 0, actionCollection(), "numerationtype");
	numeration_action->setItems(numlist);
	numeration_action->setCurrentItem(Prefs::numeration()); 
	connect (numeration_action, SIGNAL(activated(int)), this, SLOT(slotSwitchtoNumeration(int)));

	/*
	 * the actions for the colorschemes
	 **/
	QStringList looklist;
	looklist.append(i18n("&No Color Scheme"));
	looklist.append(i18n("Show &Groups"));
	looklist.append(i18n("Show &Blocks"));
	looklist.append(i18n("Show &Acid Behavior"));
	looklist.append(i18n("Show &Family"));
	look_action = new KSelectAction (i18n("&Look"), 0, this, 0, actionCollection(), "look_menu");
	look_action->setItems(looklist);
	look_action->setCurrentItem(Prefs::colorschemebox()); 
	connect (look_action, SIGNAL(activated(int)), this, SLOT(slotShowScheme(int)));

	/*
	 * the misc actions
	 **/
	m_pPlotAction = new KAction(i18n("&Plot Data"), "kmplot", 0, this, SLOT(slotPlotData()), actionCollection(), "plotdata");
	
	m_pCalcAction = new KAction(i18n("&Calculate Molecular Weights"), "calculate", 0, this, SLOT(slotCalculate()), actionCollection(), "calculate_weights");
	
	m_pGlossaryAction = new KAction(i18n("&Glossary"), "add", 0, this, SLOT(slotGlossary()), actionCollection(), "glossary");

	//Legend
	m_pLegendAction = new KAction(i18n("Hide &Legend"), "legend", 0, this, SLOT(slotShowLegend()), actionCollection(), "toggle_legend");
	
	m_pLearningmodeAction = new KAction(i18n("Enter Learning Mode"), "legend", 0, this, SLOT(slotLearningmode()), actionCollection(), "learning_mode");

	//the standardactions
	KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
	KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );

	slotShowScheme( Prefs::colorschemebox() );
	slotSwitchtoNumeration( Prefs::numeration() );
	slotSwitchtoPSE( Prefs::schemaPSE() );

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
}

void Kalzium::slotGlossary()
{
	GlossaryDialog *dlg = new GlossaryDialog( this, "glossary" );
	dlg->show();
}

void Kalzium::slotLearningmode()
{
	if ( m_PSE->learningMode() )
	{
		m_pLearningmodeAction->setText(i18n("Enter &Learningmode"));
		m_PSE->setLearning( false );
		emit tableLocked(false);
	}
	else
	{
		emit tableLocked(true);
		m_pLearningmodeAction->setText(i18n("Leave &Learningmode"));
		m_PSE->setLearning( true );
		InformationWidget *l_dlg = new InformationWidget( m_PSE );
		connect( m_PSE, SIGNAL( tableClicked( QPoint ) ), l_dlg, SLOT( slotUpdate( QPoint ) ) );
		connect( l_dlg->buttonGroup, SIGNAL( clicked(int) ), m_PSE , SLOT( setLearningMode(int) ) );
		connect( l_dlg, SIGNAL( closed() ), m_PSE, SLOT(slotLock()) );

		l_dlg->show();
	}
}

void Kalzium::setupStatusBar()
{
//X 	statusBar()->insertItem("", IDS_TEMP, 0, false);
//X  	statusBar()->setItemAlignment(IDS_TEMP, AlignLeft);
 	statusBar()->insertItem("", IDS_ENERG, 0, false);
 	statusBar()->setItemAlignment(IDS_ENERG, AlignLeft);
	displayEnergie();
//X  	statusBar()->insertItem("", IDS_TEMPERATURE, 0, false);
//X  	statusBar()->setItemAlignment(IDS_TEMPERATURE, AlignLeft);	
	// fill the statusbar 
	statusBar()->show();
}

void Kalzium::slotStatusBar(const QString& text, int id)
{
	statusBar()->changeItem(text, id);
}

void Kalzium::slotPlotData()
{
	ElementDataViewer *edw = new ElementDataViewer( data(), this, "edw" );
	edw->show();
}

void Kalzium::slotCalculate()
{
	emit tableLocked(true);

	MolcalcImpl *dlg = new MolcalcImpl( data(), this, "molcalcdialog" );
	connect( m_PSE, SIGNAL( ElementClicked( int ) ), dlg, SLOT(slotButtonClicked( int )) );
	connect( dlg, SIGNAL( closed() ), m_PSE, SLOT(slotLock()) );
	dlg->show();
}

void Kalzium::slotShowLegend()
{
	if(m_PSE->showLegend())
	{
		m_PSE->showLegend(false);
		m_pLegendAction->setText(i18n("Show &Legend"));
	}
	else
	{
		m_PSE->showLegend(true);
		m_pLegendAction->setText(i18n("Hide &Legend"));
	}
	
	//save the settings
	Prefs::setShowlegend( m_PSE->showLegend() ); 
	Prefs::writeConfig();
}	

void Kalzium::slotShowScheme(int i)
{
	m_PSE->activateColorScheme( i );
	m_PSE->update();
	
	Prefs::setColorschemebox(i); 
	Prefs::writeConfig();
}

void Kalzium::slotSwitchtoNumeration( int index )
{
	m_PSE->setNumerationType( index );
	Prefs::setNumeration(index); 
	Prefs::writeConfig();
}

void Kalzium::slotSwitchtoPSE(int index)
{
	if ( index == 0 )
		m_PSE->setPSEType( true );//simple
	else
		m_PSE->setPSEType( false );//complex
	
	m_PSE->update();
	
	Prefs::setSchemaPSE(index);
	Prefs::writeConfig();
}

void Kalzium::showSettingsDialog()
{
	if (KConfigDialog::showDialog("settings"))
		return;

	//KConfigDialog didn't find an instance of this dialog, so lets create it :
	KConfigDialog *dialog = new KConfigDialog(this,"settings", Prefs::self());
	connect( dialog, SIGNAL( settingsChanged() ), this , SLOT( slotUpdateSettings() ) );
	dialog->addPage( new setColors( 0, "colors_page"), i18n("Colors"), "colorize");
	dialog->addPage( new setupMisc( 0, "miscpage" ), i18n( "Miscellaneous" ), "misc" );
	dialog->show();
}

void Kalzium::slotUpdateSettings()
{
	look_action->setCurrentItem(Prefs::colorschemebox()); 
	m_PSE->reloadColours();

	displayEnergie();
}
 
void Kalzium::displayEnergie()
 {
 	QString string;
 	switch (Prefs::units()) {
     		case 0:
 			string = i18n("kilojoule per mol. Please enter a capital 'J'", "kJ/mol");
 			break;
 		case 1:
 			string = i18n("the symbol for electronvolt", "eV");
 			break;
 	}
 	slotStatusBar(i18n("the argument %1 is the unit of the energy (eV or kJ/mol)", "Energy: %1").arg( string ),  IDS_ENERG);
}

void Kalzium::openInformationDialog( int number )
{
	if ( !m_PSE->learningMode() && m_PSE->showTooltip() )
	{
		emit tableLocked(true);
		DetailedInfoDlg info_dlg( data(), data()->element(number), this , "detailedDlg" );

		info_dlg.exec();
		emit tableLocked(false);
	}
}

KalziumDataObject* Kalzium::data() const { return pd->kalziumData; }

Kalzium::~Kalzium(){}

//The names of the elements are stored in a xml-file. As they need to be
//grepped for the translation I am putting them here so that xgettext
//is adding them to the pot-file.
#if 0
	i18n("Hydrogen");
	i18n("Helium");
	i18n("Lithium");
	i18n("Beryllium");
	i18n("Boron");
	i18n("Carbon");
	i18n("Nitrogen");
	i18n("Oxygen");
	i18n("Fluorine");
	i18n("Neon");
	i18n("Sodium");
	i18n("Magnesium");
	i18n("Aluminum");
	i18n("Silicon");
	i18n("Phosphorus");
	i18n("Sulfur");
	i18n("Chlorine");
	i18n("Argon");
	i18n("Potassium");
	i18n("Calcium");
	i18n("Scandium");
	i18n("Titanium");
	i18n("Vanadium");
	i18n("Chromium");
	i18n("Manganese");
	i18n("Iron");
	i18n("Cobalt");
	i18n("Nickel");
	i18n("Copper");
	i18n("Zinc");
	i18n("Gallium");
	i18n("Germanium");
	i18n("Arsenic");
	i18n("Selenium");
	i18n("Bromine");
	i18n("Krypton");
	i18n("Rubidium");
	i18n("Strontium");
	i18n("Yttrium");
	i18n("Zirconium");
	i18n("Niobium");
	i18n("Molybdenum");
	i18n("Technetium");
	i18n("Ruthenium");
	i18n("Rhodium");
	i18n("Palladium");
	i18n("Silver");
	i18n("Cadmium");
	i18n("Indium");
	i18n("Tin");
	i18n("Antimony");
	i18n("Tellurium");
	i18n("Iodine");
	i18n("Xenon");
	i18n("Caesium");
	i18n("Barium");
	i18n("Lanthanum");
	i18n("Cerium");
	i18n("Praseodymium");
	i18n("Neodymium");
	i18n("Promethium");
	i18n("Samarium");
	i18n("Europium");
	i18n("Gadolinium");
	i18n("Terbium");
	i18n("Dysprosium");
	i18n("Holmium");
	i18n("Erbium");
	i18n("Thulium");
	i18n("Ytterbium");
	i18n("Lutetium");
	i18n("Hafnium");
	i18n("Tantalum");
	i18n("Tungsten");
	i18n("Rhenium");
	i18n("Osmium");
	i18n("Iridium");
	i18n("Platinum");
	i18n("Gold");
	i18n("Mercury");
	i18n("Thallium");
	i18n("Lead");
	i18n("Bismuth");
	i18n("Polonium");
	i18n("Astatine");
	i18n("Radon");
	i18n("Francium");
	i18n("Radium");
	i18n("Actinium");
	i18n("Thorium");
	i18n("Protactinium");
	i18n("Uranium");
	i18n("Neptunium");
	i18n("Plutonium");
	i18n("Americium");
	i18n("Curium");
	i18n("Berkelium");
	i18n("Californium");
	i18n("Einsteinium");
	i18n("Fermium");
	i18n("Mendelevium");
	i18n("Nobelium");
	i18n("Lawrencium");
	i18n("Rutherfordium");
	i18n("Dubnium");
	i18n("Seaborgium");
	i18n("Bohrium");
	i18n("Hassium");
	i18n("Meitnerium");
	i18n("Darmstadtium");
	i18n("Roentgenium");
#endif


#include "kalzium.moc"

