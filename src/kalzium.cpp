/***************************************************************************
    copyright            : (C) 2003, 2004 by Carsten Niehaus
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
#include "settings_colorschemes.h"
#include "settings_colors.h"
#include "settings_misc.h"
#include "slider_widget.h"
#include "elementdataviewer.h"
#include "tempslider.h"

#include <qinputdialog.h>
#include <qslider.h>
#include <qinputdialog.h>

#include <qlayout.h>
#include <qslider.h>


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

	QWidget *m_pCentralWidget = new QWidget( this, "CentralWidget" );
	m_pCentralLayout = new QVBoxLayout( m_pCentralWidget, 0, -1, "CentralLayout" );
	m_pSOMSlider = new TempSlider( m_pCentralWidget, "SOMSlider" );

	m_pRegularPSE = new RegularPSE( data(), m_pCentralWidget, "regularPSE");
	m_pSimplePSE = new SimplifiedPSE( data(), m_pCentralWidget, "SimplifiedPSE");
	m_pCurrentPSE = m_pRegularPSE;
	m_pCurrentPSE->show();
	
	// Layouting
	m_pCentralLayout->addWidget( m_pCurrentPSE );
	m_pCentralLayout->addWidget( m_pSOMSlider );

	setCentralWidget( m_pCentralWidget );
	m_pCentralWidget->show();	
	setupActions();
	setupStatusBar();
}


void Kalzium::setupActions()
{
	/*
	 * the actions for switching PSE
	 **/
	 QStringList schemalist;
	 schemalist.append(i18n("Show &Regular PSE"));
//	 schemalist.append(i18n("Mendeleev - localized name of Russian chemist Dmitri Mendeleev","Show &Mendeleev PSE"));
	 schemalist.append(i18n("Show &Simple PSE"));
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
	 look_action = new KSelectAction (i18n("&Look"), 0, this, 0, actionCollection(), "look_menu");
	 look_action->setItems(looklist);
	 look_action->setCurrentItem(Prefs::colorschemebox()); 
	 connect (look_action, SIGNAL(activated(int)), this, SLOT(slotShowScheme(int)));
	
	/*
	 * the misc actions
	 **/
	m_pTimelineAction = new KAction(i18n("Show &Timeline"), "timeline", 0, this, SLOT(slotShowTimeline()), actionCollection(), "use_timeline");
	m_pPlotAction = new KAction(i18n("&Plot Data"), "kmplot", 0, this, SLOT(slotPlotData()), actionCollection(), "plotdata");
	m_pSOMAction = new KAction(i18n("&Show State of Matter"), "chemical", 0, this, SLOT(slotStateOfMatter()), actionCollection(), "show_som");
		 //Legend
	 m_pLengendAction = new KAction(i18n("Hide &Legend"), "legend", 0, this, SLOT(slotShowLegend()), actionCollection(), "toggle_legend");
	/*
	 * the standardactions
	 **/
	KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
	KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );

	slotSwitchtoNumeration(Prefs::numeration() );
	slotSwitchtoPSE(Prefs::schemaPSE());
	slotSwitchtoNumeration(Prefs::numeration() );

	m_bShowLegend = Prefs::showlegend();
	m_pRegularPSE->setLegend( Prefs::colorschemebox() );
	m_pSimplePSE->setLegend( Prefs::colorschemebox() );
	
	connect( m_pSOMSlider->slider, SIGNAL( valueChanged( int ) ), this, SLOT( slotTempChanged( int ) ) );
	
	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
}

void Kalzium::setupStatusBar()
{
	statusBar()->insertItem("", IDS_TEMP, 0, false);
 	statusBar()->setItemAlignment(IDS_TEMP, AlignLeft);
	displayTemperature();
 	statusBar()->insertItem("", IDS_ENERG, 0, false);
 	statusBar()->setItemAlignment(IDS_ENERG, AlignLeft);
	displayEnergie();	
 	statusBar()->insertItem("", IDS_TEMPERATURE, 0, false);
 	statusBar()->setItemAlignment(IDS_TEMPERATURE, AlignLeft);
	displayTemperaturevalue();	
	// fill the statusbar 
	statusBar()->show();
}

void Kalzium::slotStatusBar(QString text, int id)
{
	statusBar()->changeItem(text, id);
}

void Kalzium::slotShowTimeline()
{
	kdDebug() << "Kalzium::slotShowTimeline()" << endl;

	m_pSliderWidget = new SliderWidget();

	/**
	 * now do the connections
	 **/
	connect( m_pSliderWidget->pSlider, SIGNAL( valueChanged( int ) ), currentPSE(), SLOT( setDate(int) ) );
	date = Prefs::sliderdate();
	m_pSliderWidget->pSlider->setValue( date );
	m_pSliderWidget->show();
}

void Kalzium::slotPlotData()
{
	ElementDataViewer *edw = new ElementDataViewer( data(), this, "edw" );
	edw->show();
}

void Kalzium::slotShowLegend()
{
	kdDebug() << "Kalzium::slotShowLegend()" << endl;
	if ( m_bShowLegend )
	{
		m_bShowLegend = FALSE;
		m_pLengendAction->setText("Show &Legend");
		
	}
	else
	{
		m_bShowLegend = TRUE;
		m_pLengendAction->setText("Hide &Legend");
	}
	currentPSE()->showLegend( m_bShowLegend );
	Prefs::setShowlegend( m_bShowLegend ); 
}	

void Kalzium::slotShowScheme(int i)
{
	kdDebug() << "Kalzium::slotShowScheme()" << endl;
	currentPSE()->activateColorScheme( i );
	currentPSE()->setLegend( i );
	Prefs::setColorschemebox(i); 
	Prefs::writeConfig();
}

void Kalzium::slotSwitchtoNumeration( int index )
{
	currentPSE()->setNumerationType( index );
	Prefs::setNumeration(index); 
	Prefs::writeConfig();
}

void Kalzium::slotSwitchtoPSE(int index)
{
	m_pRegularPSE->hide();
	m_pSimplePSE->hide();
//	m_pMendeljevPSE->hide();

	m_pCentralLayout->remove( m_pCurrentPSE );
	switch (index) {
    	case 0:
		m_pCurrentPSE = m_pRegularPSE;
		break;
	case 1:
		m_pCurrentPSE = m_pSimplePSE;
		break;
	}
	m_pCurrentPSE->show();
	m_pCentralLayout->insertWidget( 0, m_pCurrentPSE );
	// setCentralWidget( m_pCurrentPSE );
	setCaption( m_pCurrentPSE->shortName() );
	Prefs::setSchemaPSE(index);
	Prefs::writeConfig();
}

PSE* Kalzium::currentPSE() const
{
	return m_pCurrentPSE;
}

void Kalzium::showSettingsDialog()
{
	if (KConfigDialog::showDialog("settings"))
		return;

	//KConfigDialog didn't find an instance of this dialog, so lets create it :
	KConfigDialog *dialog = new KConfigDialog(this,"settings", Prefs::self());
	connect( dialog, SIGNAL( settingsChanged() ), m_pCurrentPSE, SLOT( slotUpdatePSE() ) );
	connect( dialog, SIGNAL( settingsChanged() ), this , SLOT( slotUpdateSettings() ) );
	dialog->addPage( new colorScheme( 0, "colorscheme_page"), i18n("Color Scheme"), "colorize");
	dialog->addPage( new setColors( 0, "colors_page"), i18n("Colors"), "colorize");
	dialog->addPage( new setupMisc( 0, "miscpage" ), i18n( "Misc" ), "misc" );
	dialog->show();
}

void Kalzium::slotUpdateSettings()
{
	look_action->setCurrentItem(Prefs::colorschemebox()); 
	
	displayTemperature();
    displayEnergie();
    displayTemperaturevalue();
}

void Kalzium::displayTemperature()
 {
 	QString string;
 	switch (Prefs::temperature()) {
     		case 0:
 			string = i18n("Kelvin");
			m_pSOMSlider->slider->setMinValue( 0 );
			m_pSOMSlider->slider->setMaxValue( 1000 );
			m_pSOMSlider->unit->setText( i18n( "K" ) );
 			break;
 		case 1:
 			string = i18n("Degree Celsius");
			m_pSOMSlider->slider->setMinValue( -273 );
			m_pSOMSlider->slider->setMaxValue( 727 );
			m_pSOMSlider->unit->setText( i18n( "°C" ) );
 			break;
 		case 2:
 			string = i18n("Degree Fahrenheit");
			m_pSOMSlider->slider->setMinValue( -460 );
			m_pSOMSlider->slider->setMaxValue( 1341 );
			m_pSOMSlider->unit->setText( i18n( "°F" ) );
 			break;
 	}
 	slotStatusBar(i18n("Temperature unit: %1 ").arg( string ),  IDS_TEMP);
 }
 
 void Kalzium::displayEnergie()
 {
 	QString string;
 	switch (Prefs::units()) {
     		case 0:
 			string = i18n("kJ/mol");
 			break;
 		case 1:
 			string = i18n("eV");
 			break;
 	}
 	slotStatusBar(i18n("Energy: %1").arg( string ),  IDS_ENERG);
}

void Kalzium::displayTemperaturevalue()
{
	int t = Prefs::temperature();
	slotStatusBar( i18n( "Temperature: %1" ).arg( t ), IDS_TEMPERATURE );
}

void Kalzium::slotStateOfMatter()
{
	//only the elements 1 to 95 (Americium) will be calculated because
	//only for these both the boiling _and_ melting point are known.
	//The other elements will have the color color_artificial
}

void Kalzium::slotTempChanged( int temperature )
{
 	switch (Prefs::temperature()) {
     		case 0:
			m_pCurrentPSE->setTemperature( (double) temperature );
 			break;
 		case 1:
			m_pCurrentPSE->setTemperature( (double) temperature + 273.15 );
 			break;
 		case 2:
			m_pCurrentPSE->setTemperature( (double) (temperature - 32)*5/9 + 273.15);
 			break;
 	}	
}

KalziumDataObject* Kalzium::data() const { return pd->kalziumData; }

Kalzium::~Kalzium(){}

#include "kalzium.moc"

