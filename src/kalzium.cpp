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
#include "elementbutton.h"
#include "element.h"
#include "prefs.h"
#include "settings_colorschemes.h"
#include "settings_colors.h"
#include "settings_quiz.h"
#include "settings_misc.h"
#include "questioneditor_impl.h"
#include "questionadddialog_impl.h"
#include "slider_widget.h"

#include <qlabel.h>
#include <qslider.h>
#include <qwhatsthis.h>
#include <qlcdnumber.h>

#include <kmainwindow.h>
#include <kconfigdialog.h>
#include <klocale.h>
#include <kdebug.h>
#include <kaction.h>
#include <kkeydialog.h>
#include <kedittoolbar.h>
#include <kapplication.h>

Kalzium::Kalzium()
    : KMainWindow( 0, "Kalzium" )
{
	pd = new privatedata( this );

	pd->kalziumData = new KalziumDataObject();

	/*
	 * the actions for switching PSE
	 **/
	 QStringList schemalist;
	 schemalist.append(i18n("Show &Regular PSE"));
	 schemalist.append(i18n("Show &Mendeljev PSE"));
	 schemalist.append(i18n("Show &Simple PSE"));
	 schema_action = new KSelectAction (i18n("Switch &PSE"), 0, this, 0, actionCollection(), "changepse");
	 schema_action->setItems(schemalist);
	 schema_action->setCurrentItem(0);
	 connect (schema_action, SIGNAL(activated(int)), this, SLOT(slotSwitchtoPSE(int)));
	 
	/*
	 * the actions for the quiz
	 **/
	m_pQuizStart = new KAction(i18n("Start &Quiz"), 0, this, SLOT(slotStartQuiz()), actionCollection(), "quiz_startquiz");
	m_pQuizSetup = new KAction(i18n("Set&up Quiz"), 0, this, SLOT(showSettingsDialog()), actionCollection(), "quiz_setupquiz");
	m_pQuizEditQuestions = new KAction(i18n("&Edit Questions"), 0, this, SLOT(slotEditQuestions()), actionCollection(), "quiz_editquestions");
	m_pQuizAddQuestions = new KAction(i18n("&Add Questions"), 0, this, SLOT(slotAddQuestions()), actionCollection(), "quiz_addquestion");
	 
	/*
	 * the actions for the colorschemes
	 **/
	m_pBehAcidAction = new KAction(i18n("Show &Acid Behavior"), 0, this, SLOT(slotShowScheme(void)), actionCollection(), "view_acidic");
	m_pBehBlocksAction = new KAction(i18n("Show Blocks"), 0, this, SLOT(slotShowScheme(void)), actionCollection(), "view_blocks");
	m_pBehBlocksAction = new KAction(i18n("Show Groups"), 0, this, SLOT(slotShowScheme(void)), actionCollection(), "view_groups");
	m_pBehBlocksAction = new KAction(i18n("Show State Of Matter"), 0, this, SLOT(slotShowScheme(void)), actionCollection(), "view_som");
	m_pBehBlocksAction = new KAction(i18n("No Color Scheme"), 0, this, SLOT(slotShowScheme(void)), actionCollection(), "view_normal");
	m_pTimelineAction = new KAction(i18n("Show Timeline"), 0, this, SLOT(slotShowTimeline()), actionCollection(), "use_timeline");
	
	/*
	 * the standardactions
	 **/
	KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
	KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );
	KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()), actionCollection());
	KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()), actionCollection());

	m_pRegularPSE = new RegularPSE( data(), this, "regularPSE");
	m_pSimplePSE = new SimplifiedPSE( data(), this, "SimplifiedPSE");
	m_pMendeljevPSE = new MendeljevPSE( data(), this, "MendeljevPSE");

	m_pCurrentPSE = m_pRegularPSE;

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	createGUI();

	setCentralWidget( m_pCurrentPSE );
	
	createStandardStatusBarAction();
}

void Kalzium::slotStartQuiz()
{
	kdDebug() << "insdide the start of the quiz" << endl;
}

void Kalzium::slotEditQuestions()
{
	kdDebug() << "inside the questionseditor" << endl;
	questionEditorImpl *q = new questionEditorImpl( this, "questionEditor" );
	q->show();
}

void Kalzium::slotAddQuestions()
{
	kdDebug() << "Kalzium::slotAddQuestions()" << endl;

	questionAddDialogImpl *q = new questionAddDialogImpl( this, "questionAddDialogImpl" );
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

	pSliderWidget->show();
}

void Kalzium::slotShowScheme(void)
{
	int i = 0;
	QString n = sender()->name();
	if ( n == QString("view_normal"))
		i = 0;
	if ( n == QString("view_groups")) 
		i = 1;
	if ( n == QString("view_blocks")) 
		i = 2;
	if ( n == QString("view_som")) 
		i = 3;
	if ( n == QString("view_acidic"))
		i = 4;
	
	currentPSE()->activateColorScheme( i );
}

void Kalzium::slotSwitchtoPSE(int index)
{
	m_pRegularPSE->hide();
	m_pSimplePSE->hide();
	m_pMendeljevPSE->hide();

	switch (index) {
    	case 0:
		m_pCurrentPSE = m_pRegularPSE;
		break;
	case 1:
		m_pCurrentPSE = m_pMendeljevPSE;
		break;
	case 2:
		m_pCurrentPSE = m_pSimplePSE;
		break;
	}
	m_pCurrentPSE->show();
	setCentralWidget( m_pCurrentPSE );
	setCaption( m_pCurrentPSE->name() );
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
	connect( dialog, SIGNAL( settingsChanged() ), this , SLOT( slotSaveConfig() ) );
	dialog->addPage( new setColorScheme( 0, "colorscheme_page"), i18n("Configure Default Colorscheme"), "colorize");
	dialog->addPage( new setColors( 0, "colors_page"), i18n("Configure Colors"), "colorize");
	dialog->addPage( new setupQuiz( 0, "quizsetuppage" ), i18n( "Configure Quiz" ), "edit" );
	dialog->addPage( new setupMisc( 0, "miscpage" ), i18n( "Configure Misc" ), "misc" );
	dialog->show();
}

void Kalzium::slotSaveConfig()
{
	Prefs::writeConfig();
}

void Kalzium::optionsConfigureKeys()
{
	KKeyDialog::configure(actionCollection());
}

void Kalzium::optionsConfigureToolbars( )
{
	saveMainWindowSettings( KGlobal::config(), autoSaveGroup() );
	KEditToolbar dlg(actionCollection());
	connect(&dlg, SIGNAL(newToolbarConfig()), this, SLOT(newToolbarConfig()));
	dlg.exec();
}

void Kalzium::newToolbarConfig()
{
    createGUI();
    applyMainWindowSettings( KGlobal::config(), autoSaveGroup() );
}


KalziumDataObject* Kalzium::data() const { return pd->kalziumData; }

Kalzium::~Kalzium(){}

#include "kalzium.moc"

