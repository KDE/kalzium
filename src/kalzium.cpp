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

	m_info = new InformationWidget( m_PSE );
	connect( m_info, SIGNAL( closed() ), m_PSE, SLOT(slotUnlock()) );
	connect( m_info, SIGNAL( closed() ), this, SLOT(slotLearningmode()) );

	m_PSE->repaint();
}

void Kalzium::setupActions()
{
	/*
	 * the actions for switching PSE
	 **/
	QStringList gradientlist;
	gradientlist.append(i18n("No Gradient"));
	gradientlist.append(i18n("Atomic Radius"));
	gradientlist.append(i18n("Atomic Weight"));
	gradientlist.append(i18n("Density"));
	gradientlist.append(i18n("Boilingpoint"));
	gradientlist.append(i18n("Meltingpoint"));
	gradientlist.append(i18n("Electronegativity"));
	gradient_action = new KSelectAction (i18n("&Gradient"), 0, this, 0, actionCollection(), "gradmenu");
	gradient_action->setItems(gradientlist);
	connect (gradient_action, SIGNAL(activated(int)), this, SLOT(slotSwitchtoGradient(int)));

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
	
	m_pGlossaryAction = new KAction(i18n("&Glossary"), "glossary", 0, this, SLOT(slotGlossary()), actionCollection(), "glossary");

	//Legend
	m_pLegendAction = new KAction(i18n("Hide &Legend"), "legend", 0, this, SLOT(slotShowLegend()), actionCollection(), "toggle_legend");
	
	m_pLearningmodeAction = new KAction(i18n("Enter Learning Mode"), "legend", 0, this, SLOT(slotLearningmode()), actionCollection(), "learning_mode");

	//the standardactions
	KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
	KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );

	slotShowScheme( Prefs::colorschemebox() );
	slotSwitchtoNumeration( Prefs::numeration() );

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
}

void Kalzium::slotGlossary()
{
	emit tableLocked(true);
	GlossaryDialog *dlg = new GlossaryDialog( this, "glossary" );
	connect( dlg, SIGNAL( closed() ), m_PSE, SLOT(slotUnlock()) );
	dlg->show();
}

void Kalzium::slotLearningmode()
{
	if ( m_PSE->learningMode() )
	{
		m_pLearningmodeAction->setText(i18n("Enter &Learningmode"));
		m_PSE->setLearning( false );
		emit tableLocked(false);
		if ( m_info->isShown() )
		{
			m_info->hide();
		}
	}
	else
	{
		emit tableLocked(true);
		m_pLearningmodeAction->setText(i18n("Leave &Learningmode"));
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
	connect( dlg, SIGNAL( closed() ), m_PSE, SLOT(slotUnlock()) );
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
 
	//JH: redraw the full table next time
	setFullDraw();
}	

void Kalzium::slotShowScheme(int i)
{
	m_PSE->activateColorScheme( i );
	slotSwitchtoGradient( 0 );//deactivate the gradient
	gradient_action->setCurrentItem(0);
	m_PSE->update();

	Prefs::setColorschemebox(i); 
	Prefs::writeConfig();

	//JH: redraw the full table next time
	setFullDraw();
}

void Kalzium::slotSwitchtoGradient( int index )
{
	if ( index == 0 )
	{
		m_PSE->setGradient( false );
		return;
	}
	m_PSE->setGradientType( index );
	m_PSE->setGradient( true );
}

void Kalzium::slotSwitchtoNumeration( int index )
{
	m_PSE->setNumerationType( index );
	Prefs::setNumeration(index); 
	Prefs::writeConfig();
  
  //JH: redraw the full table next time
  setFullDraw();
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

#include "kalzium.moc"

