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
#include "legendwidget.h"

#include <qinputdialog.h>
#include <qlayout.h>
#include <qslider.h>
#include <qlcdnumber.h>

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
	m_pLegend = new Legend( CentralWidget, "Legend" );
	m_pSOMSlider = new TempSlider( CentralWidget, "SOMSlider" );
	m_pTimeSlider = new SliderWidget( CentralWidget, "TimeSlider" );

	m_pRegularPSE = new RegularPSE( data(), CentralWidget, "regularPSE");
	m_pSimplePSE = new SimplifiedPSE( data(), CentralWidget, "SimplifiedPSE");

	//XXX get the correct PSE
	m_pCurrentPSE = m_pRegularPSE;
	m_pCurrentPSE->show();
	
	// Layouting
	m_pCentralLayout->addWidget( m_pCurrentPSE );
	m_pCentralLayout->addWidget( m_pLegend );
	m_pCentralLayout->addWidget( m_pSOMSlider );
	m_pCentralLayout->addWidget( m_pTimeSlider );

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
	schemalist.append(i18n("Show &Regular PSE"));
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

	//the standardactions
	KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
	KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );

	slotSwitchtoNumeration(Prefs::numeration() );
	slotSwitchtoPSE(Prefs::schemaPSE());
	slotSwitchtoNumeration(Prefs::numeration() );

	//invert the bool because it will be toggled in the three slots
	m_bShowLegend = !Prefs::showlegend();
	m_bShowSOM = !Prefs::showsom();
	m_bShowTimeline = !Prefs::showtimeline();

	connect( m_pSOMSlider->slider, SIGNAL( valueChanged( int ) ), this, SLOT( slotTempChanged( int ) ) );
	connect( m_pTimeSlider->pSlider, SIGNAL( valueChanged( int ) ), m_pRegularPSE, SLOT( setDate( int ) ) );
	m_pSOMSlider->slider->setValue(Prefs::temperaturevalue());
	m_pTimeSlider->pSlider->setValue(Prefs::sliderdate());

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
	
	//check if the legend, the timeline and the somslider should
	//be displayed or not
	slotShowScheme(Prefs::colorschemebox());
	slotShowTimeline();
	slotShowLegend();
	slotStateOfMatter();
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
	
	if ( m_bShowTimeline )
		m_bShowTimeline = false;
	else
		m_bShowTimeline = true;
		
	Prefs::setShowtimeline( m_bShowTimeline ); 

	if ( m_bShowTimeline )
	{
		m_pTimeSlider->pSlider->setValue( Prefs::sliderdate() );
		m_pTimelineAction->setText(i18n("Hide &Timeline"));
		m_pTimeSlider->show();
	}
	else
	{
		m_pTimeSlider->hide();
		Prefs::setSliderdate( m_pTimeSlider->pSlider->value() );
		m_pTimelineAction->setText(i18n("Show &Timeline"));
	}
	
	Prefs::writeConfig();
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
		m_pLengendAction->setText( i18n( "Show &Legend" ) );
	}
	else
	{
		m_bShowLegend = TRUE;
		m_pLengendAction->setText( i18n( "Hide &Legend" ) );
	}
	
	//now hide or show the legend...
	m_bShowLegend ? m_pLegend->show() : m_pLegend->hide();

	//save the settings
	Prefs::setShowlegend( m_bShowLegend ); 
	Prefs::writeConfig();
}	

void Kalzium::slotShowScheme(int i)
{
	kdDebug() << "Kalzium::slotShowScheme() " << i << endl;
	
	//hide the SOM because a look has been selected
	showSOMWidgets( false );

	if ( m_bShowSOM )
		m_bShowSOM = false;
	
	currentPSE()->activateColorScheme( i );
	
	m_pLegend->setScheme( i );
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
	connect( dialog, SIGNAL( settingsChanged() ), this , SLOT( slotUpdateSettings() ) );
	dialog->addPage( new colorScheme( 0, "colorscheme_page"), i18n("Color Scheme"), "colorize");
	dialog->addPage( new setColors( 0, "colors_page"), i18n("Colors"), "colorize");
	dialog->addPage( new setupMisc( 0, "miscpage" ), i18n( "Miscellaneous" ), "misc" );
	dialog->show();
}

void Kalzium::slotUpdateSettings()
{
	look_action->setCurrentItem(Prefs::colorschemebox()); 

	displayTemperature();
    	displayEnergie();
	slotTempChanged( Prefs::temperaturevalue());
	//update colors
	if (!m_bShowSOM )
	{
		m_pCurrentPSE->activateColorScheme(Prefs::colorschemebox());
		m_pLegend->setScheme( Prefs::colorschemebox() );
	}
	else 
	{
		m_pCurrentPSE->setTemperature((double) Prefs::temperaturevalue() );
		m_pLegend->setScheme( 4 );
	}
}

void Kalzium::displayTemperature()
 {
 	QString string;
	int min = 0, max = 1000;
 	switch (Prefs::temperature()) {
     		case 0:
 			string = i18n("Kelvin");
			min = 0;
			max = 1000;
			m_pSOMSlider->unit->setText( i18n( "the unit for Kelvin" , "K" ) );
 			break;
 		case 1:
 			string = i18n("Degree Celsius");
			min = -273 ;
			max = 727 ;
			m_pSOMSlider->unit->setText( i18n( "the unit for degree celsius" , "°C" ) );
 			break;
 		case 2:
 			string = i18n("Degree Fahrenheit");
			min =  -460 ;
			max = 1341 ;
			m_pSOMSlider->unit->setText( i18n( "the unit for degree Fahrenheit" , "°F" ) );
 			break;
 	}
	m_pSOMSlider->slider->setMinValue( min );
	m_pSOMSlider->slider->setMaxValue( max );

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
 			string = i18n("the symbol for electronvolt", "eV");
 			break;
 	}
 	slotStatusBar(i18n("the argument %1 is the unit of the energy (eV or kj/mol)", "Energy: %1").arg( string ),  IDS_ENERG);
}

void Kalzium::slotStateOfMatter()
{
	kdDebug() << "Kalzium::slotStateOfMatter()" << endl;

	if ( m_bShowSOM )
		m_bShowSOM = false;
	else
		m_bShowSOM = true;
	
	if ( m_bShowSOM )
	{
		kdDebug( ) << "now show the widgets" << endl;
		showSOMWidgets( true );
	}
	else
	{
		kdDebug( ) << "now HIDE the widgets" << endl;
		slotShowScheme( Prefs::colorschemebox() );
	}

	Prefs::setShowsom( m_bShowSOM ); 
	Prefs::writeConfig();
}


void Kalzium::showSOMWidgets( bool show )
{
	kdDebug() << "Kalzium::showSOMWidgets()" << endl;

	if ( show )
	{
		m_pSOMSlider->show();
		m_pLegend->setScheme( 4 );
		slotTempChanged( m_pSOMSlider->slider->value() );
		displayTemperature();
		m_pSOMAction->setText( i18n( "&Hide State of Matter" ));
	}
	else
	{
		m_pSOMSlider->hide();
		slotStatusBar( "", IDS_TEMPERATURE );
		m_pSOMAction->setText( i18n( "&Show State of Matter" ));
	}
}
void Kalzium::slotTempChanged( int temperature )
{
	kdDebug() << "Kalzium::slotTempChanged()" << endl;
	double tempTemp=295;
 	switch (Prefs::temperature()) {
     		case 0:
			tempTemp = temperature;
 			break;
 		case 1:
			tempTemp = temperature + 273.15;
 			break;
 		case 2:
			tempTemp = (temperature - 32)*5/9 + 273.15 ;
 			break;
 	}
	m_pCurrentPSE->setTemperature( tempTemp);
	slotStatusBar( i18n( "the argument %1 is the unit of the temperature (K, C or F)","Temperature: %1" ).arg(tempTemp), IDS_TEMPERATURE );
	m_pSOMSlider->value->display((int) tempTemp);
			
	Prefs::setTemperaturevalue(temperature);
	Prefs::writeConfig();
}

KalziumDataObject* Kalzium::data() const { return pd->kalziumData; }

Kalzium::~Kalzium(){}

#include "kalzium.moc"

