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
#include "settings_quiz.h"
#include "settings_misc.h"
#include "questioneditor_impl.h"
#include "questionadddialog_impl.h"
#include "slider_widget.h"
#include "elementdataviewer.h"

#include <qslider.h>

#include <kconfigdialog.h>
#include <klocale.h>
#include <kdebug.h>
#include <kaction.h>
#include <kapplication.h>
#include <kstatusbar.h>

#define IDS_TEMP           1
#define IDS_ENERG           2

Kalzium::Kalzium()
    : KMainWindow( 0, "Kalzium" )
{
	pd = new privatedata( this );

	pd->kalziumData = new KalziumDataObject();
	setupStatusBar();
	setupActions();

	setCentralWidget( m_pCurrentPSE );
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
	 numlist.append(i18n("Show &CAS"));
	 numlist.append(i18n("Show &IUPAC"));
	 numlist.append(i18n("Show &Old IUPAC"));
	 numeration_action = new KSelectAction (i18n("&Numeration"), 0, this, 0, actionCollection(), "numerationtype");
	 numeration_action->setItems(numlist);
	 numeration_action->setCurrentItem(Prefs::numeration()); 
	 connect (numeration_action, SIGNAL(activated(int)), this, SLOT(slotSwitchtoNumeration(int)));
	 
	/*
	 * the actions for the quiz
	 **/
	 QStringList quizlist;
	 quizlist.append(i18n("Start &Quiz"));
	 quizlist.append(i18n("Set&up Quiz"));
	 quizlist.append(i18n("&Edit Questions"));
	 quiz_action = new KSelectAction (i18n("&Quiz"), 0, this, 0, actionCollection(), "quiz_menu");
	 quiz_action->setItems(quizlist);
	 //quiz_action->setCurrentItem(0);
	 connect (quiz_action, SIGNAL(activated(int)), this, SLOT(slotQuiz(int)));
	 
	/*
	 * the actions for the colorschemes
	 **/
	 QStringList looklist;
	 looklist.append(i18n("&No Color Scheme"));
	 looklist.append(i18n("Show &Groups"));
	 looklist.append(i18n("Show &Blocks"));
	 looklist.append(i18n("Show &State Of Matter"));
	 looklist.append(i18n("Show &Acid Behavior"));
	 look_action = new KSelectAction (i18n("&Look"), 0, this, 0, actionCollection(), "look_menu");
	 look_action->setItems(looklist);
	 look_action->setCurrentItem(Prefs::colorschemebox()); 
	 connect (look_action, SIGNAL(activated(int)), this, SLOT(slotShowScheme(int)));

	/*
	 * the misc actions
	 **/
	m_pTimelineAction = new KAction(i18n("Show &Timeline"), "timeline.png",0, this, SLOT(slotShowTimeline()), actionCollection(), "use_timeline");
	m_pPlotAction = new KAction(i18n("&Plot Data"), "kmplot", 0, this, SLOT(slotPlotData()), actionCollection(), "plotdata");
	
	/*
	 * the standardactions
	 **/
	KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
	KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );

	m_pRegularPSE = new RegularPSE( data(), this, "regularPSE");
	m_pSimplePSE = new SimplifiedPSE( data(), this, "SimplifiedPSE");
//	m_pMendeljevPSE = new MendeljevPSE( data(), this, "MendeljevPSE");

	m_pCurrentPSE = m_pRegularPSE;
	slotSwitchtoNumeration(Prefs::numeration() );
	slotSwitchtoPSE(Prefs::schemaPSE());
	slotSwitchtoNumeration(Prefs::numeration() );
	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
}

void Kalzium::setupStatusBar()
{
	statusBar()->insertItem("", IDS_TEMP, 0, false);
	statusBar()->setItemAlignment(IDS_TEMP, AlignLeft);
	// fill the statusbar 
	displayTemperature();
	statusBar()->insertItem("", IDS_ENERG, 0, false);
	statusBar()->setItemAlignment(IDS_ENERG, AlignLeft);
	// fill the statusbar 
	displayEnergie();	
	statusBar()->show();
}

void Kalzium::slotStatusBar(QString text, int id)
{
	statusBar()->changeItem(text, id);
}


void Kalzium::slotStartQuiz()
{
	kdDebug() << "inside the start of the quiz" << endl;

	//I will only allow the defaultvaulues until the quiz itself works.
	//After that is done I will use KConfigXT to load the userdefinded
	//values and make the quiz complete
	Quiz *q = new Quiz( 10, 2 , 3 , 10, 15 ); 
	
	QuizMaster *qm = new QuizMaster( q );
	qm->startQuiz();
}

void Kalzium::slotEditQuestions()
{
	kdDebug() << "inside the questionseditor" << endl;
	questionEditorImpl *q = new questionEditorImpl( this, "questionEditor" );
	q->show();
}

void Kalzium::slotShowTimeline()
{
	kdDebug() << "Kalzium::slotShowTimeline()" << endl;

	SliderWidget *pSliderWidget = new SliderWidget();

	/**
	 * now do the connections
	 **/
	connect( pSliderWidget->pSlider, SIGNAL( valueChanged( int ) ), currentPSE(), SLOT( setDate(int) ) );

	pSliderWidget->pSlider->setValue( 2004 );
	pSliderWidget->show();
}

void Kalzium::slotPlotData()
{
	ElementDataViewer *edw = new ElementDataViewer( data(), this, "edw" );
	edw->show();
}

void Kalzium::slotShowScheme(int i)
{
	currentPSE()->activateColorScheme( i );
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

	switch (index) {
    	case 0:
		m_pCurrentPSE = m_pRegularPSE;
		break;
//X 	case 1:
//X 		m_pCurrentPSE = m_pMendeljevPSE;
//X 		break;
	case 1:
		m_pCurrentPSE = m_pSimplePSE;
		break;
	}
	m_pCurrentPSE->show();
	setCentralWidget( m_pCurrentPSE );
	setCaption( m_pCurrentPSE->shortName() );
	Prefs::setSchemaPSE(index);
	Prefs::writeConfig();
}

void Kalzium::slotQuiz(int index)
{
	switch (index) {
    	case 0:
		slotStartQuiz();
		break;
	case 1:
		showSettingsDialog();
		break;
	case 2:
		slotEditQuestions();
		break;
	}
	quiz_action->setCurrentItem(-1);
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
	dialog->addPage( new setupQuiz( 0, "quizsetuppage" ), i18n( "Quiz" ), "edit" );
	dialog->addPage( new setupMisc( 0, "miscpage" ), i18n( "Misc" ), "misc" );
	dialog->show();
}

void Kalzium::slotUpdateSettings()
{
	look_action->setCurrentItem(Prefs::colorschemebox()); 
	displayTemperature();
	displayEnergie();
}

void Kalzium::displayTemperature()
{
	QString m_string;
	switch (Prefs::temperature()) {
    		case 0:
			m_string = i18n("Kelvin");
			break;
		case 1:
			m_string = i18n("degrees Fahrenheit");
			break;
		case 2:
			m_string = i18n("degrees Celsius");
			break;
	}
	slotStatusBar(i18n("Temperature: ")+m_string,  IDS_TEMP);
}

void Kalzium::displayEnergie()
{
	QString m_string;
	switch (Prefs::units()) {
    		case 0:
			m_string = i18n("kJ/mol");
			break;
		case 1:
			m_string = i18n("eV");
			break;
	}
	slotStatusBar(i18n("Unit: ")+m_string,  IDS_ENERG);
}

void Kalzium::slotQuizAction()
{
	quiz_action->setCurrentItem(-1);
}

KalziumDataObject* Kalzium::data() const { return pd->kalziumData; }

Kalzium::~Kalzium(){}

#include "kalzium.moc"

