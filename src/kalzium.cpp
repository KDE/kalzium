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
#include "settings_colors.h"
#include "settings_misc.h"
#include "slider_widget.h"
#include "elementdataviewer.h"
#include "tempslider.h"
#include "molcalc_impl.h"
#include "detailinfodlg.h"
#include "informationdialog_impl.h"
#include "pse.h"

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
	m_bShowSOM = false;//TODO fix the som
	m_bShowTimeline = false;//TODO fix the som

	pd = new privatedata( this );

	pd->kalziumData = new KalziumDataObject();

	QWidget *CentralWidget = new QWidget( this, "CentralWidget" );
	m_pCentralLayout = new QVBoxLayout( CentralWidget, 0, -1, "CentralLayout" );
	m_pSOMSlider = new TempSlider( CentralWidget, "SOMSlider" );
	m_pSOMSlider->hide();
	m_pTimeSlider = new SliderWidget( CentralWidget, "TimeSlider" );
	m_pTimeSlider->hide();
	
	m_PSE = new PSE( data(), CentralWidget, "PSE");

	connect( m_PSE, SIGNAL( ElementClicked( int ) ), this, SLOT( openInformationDialog( int ) ));
	
	m_info = new InformationWidget( m_PSE );
	connect( m_PSE, SIGNAL( tableClicked( QPoint ) ), m_info, SLOT( slotUpdate( QPoint ) ) );
	connect( m_info->buttonGroup, SIGNAL( clicked(int) ), m_PSE , SLOT( setLearningMode(int) ) );

	// Layouting
	m_pCentralLayout->addWidget( m_PSE );
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
	m_pTimelineAction = new KAction(i18n("Show &Timeline"), "timeline", 0, this, SLOT(slotShowTimeline()), actionCollection(), "use_timeline");
	m_pPlotAction = new KAction(i18n("&Plot Data"), "kmplot", 0, this, SLOT(slotPlotData()), actionCollection(), "plotdata");
	m_pSOMAction = new KAction(i18n("&Show State of Matter"), "chemical", 0, this, SLOT(slotStateOfMatter()), actionCollection(), "show_som");
	
	m_pCalcAction = new KAction(i18n("&Calculate Molecular Weights"), "calculate", 0, this, SLOT(slotCalculate()), actionCollection(), "calculate_weights");

	//Legend
	m_pLengendAction = new KAction(i18n("Hide &Legend"), "legend", 0, this, SLOT(slotShowLegend()), actionCollection(), "toggle_legend");
	
	m_pLearningmodeAction = new KAction(i18n("Enter Learningmode"), "legend", 0, this, SLOT(slotLearningmode()), actionCollection(), "learning_mode");

	//the standardactions
	KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
	KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );

	slotShowScheme( Prefs::colorschemebox() );
	slotSwitchtoNumeration( Prefs::numeration() );
	slotSwitchtoPSE( Prefs::schemaPSE() );

	connect( m_pSOMSlider->slider, SIGNAL( valueChanged( int ) ), this, SLOT( slotTempChanged( int ) ) );
	connect( m_pTimeSlider->pSlider, SIGNAL( valueChanged( int ) ), m_PSE, SLOT( setDate( int ) ) );
	m_pSOMSlider->slider->setValue(Prefs::temperaturevalue());
	m_pTimeSlider->pSlider->setValue(Prefs::sliderdate());

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
	
	//check if the legend, the timeline and the somslider should
	//be displayed or not
	if ( m_bShowLegend ) {
		m_PSE->showLegend( true );
	}
	else {
		m_PSE->showLegend( false );
	}
}

void Kalzium::slotLearningmode()
{
	if ( m_PSE->learningMode() )
		m_PSE->setLearning( false );
	else
		m_PSE->setLearning( true );
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

void Kalzium::slotStatusBar(const QString& text, int id)
{
	statusBar()->changeItem(text, id);
}

void Kalzium::slotShowTimeline()
{
	/*
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
	*/
}

void Kalzium::slotPlotData()
{
	ElementDataViewer *edw = new ElementDataViewer( data(), this, "edw" );
	edw->show();
}

void Kalzium::slotCalculate()
{
	m_PSE->activateMolcalcmode( true );

	MolcalcImpl *dlg = new MolcalcImpl( this, "molcalcdialog" );
	dlg->show();
}

void Kalzium::slotShowLegend()
{
	kdDebug() << "Kalzium::slotShowLegend()" << endl;
	if ( !m_bShowLegend )
	{
		m_pLengendAction->setText( i18n( "Show &Legend" ) );
	}
	else
	{
		m_pLengendAction->setText( i18n( "Hide &Legend" ) );
	}
	
	//now hide or show the legend...
	m_PSE->showLegend( m_bShowLegend );;

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
	kdDebug() << "pse is: " << index << endl;
	
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

	displayTemperature();
	displayEnergie();
	slotTempChanged( Prefs::temperaturevalue());
	//update colors
	if (!m_bShowSOM )
	{
		//X 		m_PSE->activateColorScheme(Prefs::colorschemebox());
		slotStatusBar( "", IDS_TEMPERATURE );
	}
	else 
	{
		//X 		m_PSE->setTemperature((double) Prefs::temperaturevalue() );
	}
}

void Kalzium::displayTemperature()
{
	/*
	   QString string;
	   int min = 0, max = 1000;
	   switch (Prefs::temperature()) {
	   case 0:
	   string = i18n("Kelvin");
	   min = 0;
	   max = 6000;
	   m_pSOMSlider->unit->setText( i18n( "the unit for Kelvin" , "K" ) );
	   break;
	   case 1:
	   string = i18n("Degree Celsius");
	   min = -273 ;
	   max = 5700 ;
	   break;
	   case 2:
	   string = i18n("Degree Fahrenheit");
	   min =  -460 ;
	   max = 6000 ;
	   break;
	   }
	   m_pSOMSlider->slider->setMinValue( min );
	   m_pSOMSlider->slider->setMaxValue( max );

	   slotStatusBar(i18n("Temperature unit: %1 ").arg( string ),  IDS_TEMP);
	   */
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
	/*
	m_bShowSOM ? m_bShowSOM = false : m_bShowSOM = true;
	
	m_PSE->activateSOMMode( m_bShowSOM );
	
	Prefs::setShowsom( m_bShowSOM ); 
	Prefs::writeConfig();

	showSOMWidgets( m_bShowSOM );
	*/
}


void Kalzium::showSOMWidgets( bool show )
{
	/*
	kdDebug() << "Kalzium::showSOMWidgets()" << endl;

	if ( show )
	{
		m_pSOMSlider->show();
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
	m_PSE->update();
	*/
}

void Kalzium::openInformationDialog( int number )
{
	if ( !m_PSE->learningMode() )
	{
		Element *e = new Element( number );

		DetailedInfoDlg *detailedDlg = new DetailedInfoDlg( e, this , "detailedDlg" );
		detailedDlg->show();
		m_info->hide();
	}
	else
	{
		m_info->show( );
	}
}


void Kalzium::slotTempChanged( int temperature )
{
	/*
	double tempTemp=temperature;
 	switch (Prefs::temperature()) {
     		case 0:
 			break;
 		case 1:
			//0 Degree Celsius is 273 K. So I need to add 273
			tempTemp+=273.15;
 			break;
 		case 2:
			tempTemp = (temperature - 32)*5/9 + 273.15;
 			break;
 	}
	//This is in Kelvin
	m_PSE->setTemperature( tempTemp );
	m_PSE->update();

	slotStatusBar( i18n( "the argument %1 is the unit of the temperature (K, C or F)","Temperature: %1" ).arg(tempTemp), IDS_TEMPERATURE );

	//This is in the selected unit
	m_pSOMSlider->value->display((int) tempTemp);
			
	Prefs::setTemperaturevalue(temperature);
	Prefs::writeConfig();

	*/
}

KalziumDataObject* Kalzium::data() const { return pd->kalziumData; }

Kalzium::~Kalzium(){}

#include "kalzium.moc"

