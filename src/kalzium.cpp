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
#include "elementdataviewer.h"
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
	pd = new privatedata( this );

	pd->kalziumData = new KalziumDataObject();

	QWidget *CentralWidget = new QWidget( this, "CentralWidget" );
	m_pCentralLayout = new QVBoxLayout( CentralWidget, 0, -1, "CentralLayout" );
	
	m_PSE = new PSE( data(), CentralWidget, "PSE");

	connect( m_PSE, SIGNAL( ElementClicked( int ) ), this, SLOT( openInformationDialog( int ) ));
	
	m_info = new InformationWidget( m_PSE );
	connect( m_PSE, SIGNAL( tableClicked( QPoint ) ), m_info, SLOT( slotUpdate( QPoint ) ) );
	connect( m_info->buttonGroup, SIGNAL( clicked(int) ), m_PSE , SLOT( setLearningMode(int) ) );

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

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
}

void Kalzium::slotLearningmode()
{
	if ( m_PSE->learningMode() )
	{
		m_PSE->setLearning( false );
		m_info->hide();
	}
	else
	{
		m_PSE->setLearning( true );
		m_info->show();
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

void Kalzium::slotShowTimeline()
{
	if ( m_PSE->timeline() )
	{//don NOT display the timeline
		m_PSE->setTimeline( false );
		m_pTimelineAction->setText(i18n("Show &Timeline"));
		
		Prefs::setSliderdate( m_PSE->date() );

		m_info->show();
		m_info->showTimeline();
	}
	else
	{
		m_PSE->setTimeline( true );
		m_PSE->setDate( Prefs::sliderdate() );
		m_pTimelineAction->setText(i18n("Hide &Timeline"));
		
		m_info->hide();
	}
	
	Prefs::writeConfig();
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
	//save the settings
	Prefs::setShowlegend( m_PSE->showLegend() ); 
	Prefs::writeConfig();
}	

void Kalzium::slotShowScheme(int i)
{
	kdDebug() << "Kalzium::slotShowScheme() " << i << endl;
	
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

	displayEnergie();
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
	if ( m_PSE->som() )
	{
		m_PSE->activateSOMMode( false );
		m_info->show( );
		m_info->showSOM();
	}
	else
	{
		m_PSE->activateSOMMode( true );
		m_info->hide();
	}
		
	Prefs::setShowsom( m_PSE->som() ); 
	Prefs::writeConfig();

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

KalziumDataObject* Kalzium::data() const { return pd->kalziumData; }

Kalzium::~Kalzium(){}

#include "kalzium.moc"

