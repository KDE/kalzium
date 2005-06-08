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

#include "element.h"
#include "prefs.h"
#include "settings_colors.h"
#include "settings_misc.h"
#include "settings_units.h"
#include "elementdataviewer.h"
#include "molcalc_impl.h"
#include "detailinfodlg.h"
#include "informationdialog_impl.h"
#include "pse.h"
#include "glossarydialog.h"
#include "molcalcwidget.h"
#include "detailedgraphicaloverview.h"

#include <qdockwindow.h>
#include <qlayout.h>
#include <qtoolbox.h>

#include <kconfigdialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kdebug.h>
#include <kaction.h>
#include <kapplication.h>
#include <kstatusbar.h>
#include <kcombobox.h>

#define IDS_TEMP            1
#define IDS_ENERG           3
#define IDS_UNITS           5
#define PSE_MARGIN          5
#define IDS_ELEMENTINFO     7

Kalzium::Kalzium()
    : KMainWindow( 0, "Kalzium" )
{
	pd = new privatedata( this );

	pd->kalziumData = new KalziumDataObject();

	QWidget *CentralWidget = new QWidget( this, "CentralWidget" );
	m_pCentralLayout = new QVBoxLayout( CentralWidget, PSE_MARGIN, -1, "CentralLayout" );
	
	m_PSE = new PSE( data(), CentralWidget, "PSE");
	m_infoDialog = 0;

	connect( m_PSE, SIGNAL( ElementClicked( int ) ), this, SLOT( openInformationDialog( int ) ));
	connect( m_PSE, SIGNAL( MouseOver( int ) ), this, SLOT( slotStatusbar( int ) ));
	connect( this, SIGNAL( tableLocked( bool ) ), m_PSE, SLOT( slotLock(bool ) ));
	
	// Layouting
	m_pCentralLayout->addWidget( m_PSE );

	setCentralWidget( CentralWidget );
	CentralWidget->show();

	setupStatusBar();
	setupActions();
	setupSidebars();

	if ( Prefs::showlegend() )
		slotShowLegend();
	m_showSidebar = Prefs::showsidebar();
	if ( m_showSidebar )
	{
		m_dockWin->show();
		m_SidebarAction->setText(i18n("Hide &Sidebar"));
	}

	m_PSE->repaint();
}

void Kalzium::setupActions()
{
	m_actionNoScheme = new KToggleAction(i18n("&No Color Scheme"), 0, this, SLOT(slotNoLook()), actionCollection(), "look_noscheme");

	/*
	 * the actions for the colorschemes
	 **/
	m_actionGroups = new KToggleAction(i18n("Show &Groups"), 0, this, SLOT(slotLookGroups()), actionCollection(), "look_groups");
	m_actionBlocks = new KToggleAction(i18n("Show &Blocks"), 0, this, SLOT(slotLookBlocks()), actionCollection(), "look_blocks");
	m_actionAcid = new KToggleAction(i18n("Show &Acid Behavior"), 0, this, SLOT(slotLookAcidBehavior()), actionCollection(), "look_acid");
	m_actionFamily = new KToggleAction(i18n("Show &Family"), 0, this, SLOT(slotLookFamily()), actionCollection(), "look_family");
	m_actionCrystal = new KToggleAction(i18n("Show &Crystal Structures"), 0, this, SLOT(slotLookCrystal()), actionCollection(), "look_crystal");

	/*
	 * the actions for switching PSE
	 **/
	QStringList gradientlist;
	gradientlist.append(i18n("Atomic Radius"));
	gradientlist.append(i18n("Covalent Radius"));
	gradientlist.append(i18n("van der Waals Radius"));
	gradientlist.append(i18n("Atomic Mass"));
	gradientlist.append(i18n("Density"));
	gradientlist.append(i18n("Boiling point"));
	gradientlist.append(i18n("Melting point"));
	gradientlist.append(i18n("Electronegativity"));
	gradientlist.append(i18n("Electron affinity"));
	gradient_action = new KSelectAction(i18n("&Gradient"), 0, this, 0, actionCollection(), "look_gradmenu");
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

	m_SidebarAction = new KAction(i18n("Show &Sidebar"), "sidebar", 0, this, SLOT(slotShowHideSidebar()), actionCollection(), "view_sidebar");

	/*
	 * the misc actions
	 **/
	m_pPlotAction = new KAction(i18n("&Plot Data"), "kmplot", 0, this, SLOT(slotPlotData()), actionCollection(), "plotdata");
	
	m_pCalcAction = new KAction(i18n("&Calculate Molecular Mass"), "calculate", 0, this, SLOT(slotCalculate()), actionCollection(), "calculate_mass");
	
	m_pGlossaryAction = new KAction(i18n("&Glossary"), "glossary", 0, this, SLOT(slotGlossary()), actionCollection(), "glossary");

	//Legend
	m_pLegendAction = new KAction(i18n("Hide &Legend"), "legend", 0, this, SLOT(slotShowLegend()), actionCollection(), "toggle_legend");
	
	m_pLearningmodeAction = new KAction(i18n("Enter &Learning Mode"), "legend", 0, this, SLOT(slotLearningmode()), actionCollection(), "learning_mode");

	//the standardactions
	KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
	KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );

	slotShowScheme( Prefs::colorschemebox() );
	slotSwitchtoNumeration( Prefs::numeration() );

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
}

void Kalzium::setupSidebars()
{
	m_dockWin = new QDockWindow( this );
	m_dockWin->setNewLine( true );
 	m_dockWin->setFixedExtentWidth( 200 );
// 	m_dockWin->setFixedExtentHeight( 300 );
	m_dockWin->setResizeEnabled( true );
	m_dockWin->setFrameShape( QFrame::ToolBarPanel );
	m_dockWin->setCaption( i18n( "Sidebar" ) );
	
	QToolBox *toolbox = new QToolBox( m_dockWin );
	m_dockWin->setWidget( toolbox );

	QWidget *fake = new QWidget( m_dockWin );
	QVBoxLayout *lay = new QVBoxLayout( fake, 5 );
	lay->activate();
	m_detailWidget = new DetailedGraphicalOverview( fake, "DetailedGraphicalOverview" );
	m_detailWidget->setMinimumSize( 180, m_detailWidget->minimumSize().height() );
	connect( m_PSE, SIGNAL( MouseOver( int ) ), this, SLOT( slotSelectedNumber( int ) ));
 	lay->addWidget( m_detailWidget );
	lay->addItem( new QSpacerItem( 10, 10, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ) );
	toolbox->addItem( fake, SmallIcon( "overview" ), i18n( "Overview" ) );
	
	m_calcWidget = new MolcalcWidget( data(), m_dockWin, "molcalcwidget" );
	toolbox->addItem( m_calcWidget, i18n( "Calculate" ) );

	connect( toolbox, SIGNAL( currentChanged( int ) ), this, SLOT( slotToolboxCurrentChanged( int ) ) );

	moveDockWindow( m_dockWin, DockLeft );
	setDockEnabled( /*m_dockWin, */DockTop, false );
	setDockEnabled( /*m_dockWin, */DockBottom, false );
	m_dockWin->hide();
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
		m_pLearningmodeAction->setText(i18n("Enter &Learning Mode"));
		m_PSE->setLearning( false );
		emit tableLocked(false);
		if ( m_info && m_info->isShown() )
		{
			disconnect( m_info, SIGNAL( closed() ), m_PSE, SLOT(slotUnlock()) );
			disconnect( m_info, SIGNAL( closed() ), this, SLOT(slotLearningmode()) );
			m_info->close();
		}
	}
	else
	{
		emit tableLocked(true);
		m_pLearningmodeAction->setText(i18n("Leave &Learning Mode"));
		m_PSE->setLearning( true );
		m_info = new InformationWidget( m_PSE );
		connect( m_info, SIGNAL( closed() ), m_PSE, SLOT(slotUnlock()) );
		connect( m_info, SIGNAL( closed() ), this, SLOT(slotLearningmode()) );
		m_info->show();
	}
}

void Kalzium::setupStatusBar()
{
	t_box = new KComboBox( statusBar() );
	e_box = new KComboBox( statusBar() );
	u_box = new KComboBox( statusBar() );
	t_box->insertItem( i18n( "Kelvin" ) );
	t_box->insertItem( i18n("Celsius") );
	t_box->insertItem( i18n("Fahrenheit" ) );
	e_box->insertItem( i18n("kilojoule per mol. Please enter a capital 'J'", "kJ/mol") );
	e_box->insertItem( i18n("the symbol for electronvolt", "eV" ));
	u_box->insertItem( i18n("SI-Units" ) );
	u_box->insertItem( i18n("Common Units" ) );

	connect( t_box, SIGNAL( activated( int ) ), this, SLOT( adjustUnits() ) );
	connect( e_box, SIGNAL( activated( int ) ), this, SLOT( adjustUnits() ) );
	connect( u_box, SIGNAL( activated( int ) ), this, SLOT( adjustUnits() ) );

	statusBar()->insertItem(i18n( "Temperature" ), IDS_TEMP+1, 0, false);
	statusBar()->setItemAlignment(IDS_TEMP+1, AlignLeft);
	statusBar()->addWidget( t_box , 0, false );

	statusBar()->insertItem(i18n( "Energy" ), IDS_ENERG+1, 0, false);
	statusBar()->setItemAlignment(IDS_ENERG+1, AlignLeft);
	statusBar()->addWidget( e_box , 0, false );
	
	statusBar()->insertItem(i18n( "Units" ), IDS_UNITS+1, 0, false);
	statusBar()->setItemAlignment(IDS_UNITS+1, AlignLeft);
	statusBar()->addWidget( u_box , 0, false );
	
	statusBar()->insertItem(i18n( "" ), IDS_ELEMENTINFO, 1, false);
	statusBar()->setItemAlignment(IDS_ELEMENTINFO, AlignRight);
	
	updateStatusbar();

	statusBar()->show();
}

void Kalzium::adjustUnits()
{
	Prefs::setTemperature( t_box->currentItem() );
	Prefs::setEnergies( e_box->currentItem() );
	Prefs::setUnits( u_box->currentItem() );
	Prefs::writeConfig();
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
	connect( dlg, SIGNAL( closed() ), m_PSE, SLOT(unSelect()) );
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

void Kalzium::slotShowHideSidebar()
{
	if( m_showSidebar )
	{
		m_dockWin->hide();
		m_SidebarAction->setText(i18n("Show &Sidebar"));
	}
	else
	{
		m_dockWin->show();
		m_SidebarAction->setText(i18n("Hide &Sidebar"));
	}
	m_showSidebar = !m_showSidebar;
	
	//save the settings
	Prefs::setShowsidebar( m_showSidebar );
	Prefs::writeConfig();
 
	//JH: redraw the full table next time
	setFullDraw();
}

void Kalzium::slotShowScheme(int i)
{
	switch ( i )
	{
		case PSE::GROUPS:
			m_actionGroups->setChecked( true );
			m_PSE->setLook( PSE::GROUPS );
			break;
		case PSE::BLOCK:
			m_actionBlocks->setChecked( true );
			m_PSE->setLook( PSE::BLOCK );
			break;
		case PSE::ACIDIC:
			m_actionAcid->setChecked( true );
			m_PSE->setLook( PSE::ACIDIC );
			break;
		case PSE::FAMILY:
			m_actionFamily->setChecked( true );
			m_PSE->setLook( PSE::FAMILY );
			break;
		case PSE::CRYSTAL:
			m_actionCrystal->setChecked( true );
			m_PSE->setLook( PSE::CRYSTAL );
			break;
		case PSE::GRADIENT:
			// XXX read better the gradient type!
			gradient_action->setCurrentItem( 1 );
			m_PSE->setLook( PSE::GRADIENT, 2 );
			break;
		case PSE::NOCOLOUR:
		default:
			m_actionNoScheme->setChecked( true );
			m_PSE->setLook( PSE::NOCOLOUR );
	}
}

void Kalzium::slotSwitchtoGradient( int index )
{
	m_PSE->setLook( PSE::GRADIENT, index + 1 );
	m_actionNoScheme->setChecked( false );
	m_actionGroups->setChecked( false );
	m_actionBlocks->setChecked( false );
	m_actionAcid->setChecked( false );
	m_actionFamily->setChecked( false );
	m_pLegendAction->setEnabled( false );
}

void Kalzium::slotSwitchtoNumeration( int index )
{
	m_PSE->setNumerationType( ( PSE::NUMERATIONTYPE )index );
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
	dialog->addPage( new setupUnits( 0, "units_page"), i18n("Units"), "gear");
	dialog->addPage( new setupMisc( 0, "miscpage" ), i18n( "Miscellaneous" ), "misc" );
	dialog->show();
}

void Kalzium::slotUpdateSettings()
{
	m_PSE->reloadColours();
	updateStatusbar();
}
 
void Kalzium::updateStatusbar()
 {
	t_box->setCurrentItem( Prefs::temperature() );
	e_box->setCurrentItem( Prefs::energies() );
	u_box->setCurrentItem( Prefs::units() );
}

void Kalzium::slotStatusbar( int num )
{
	Element *e = data()->element( num );
	statusBar()->changeItem( i18n( "%1, Weight: %2 u" ).arg( e->elname() ).arg( e->mass() ) , IDS_ELEMENTINFO );
}

void Kalzium::openInformationDialog( int number )
{
	if ( !m_PSE->learningMode() && m_PSE->showTooltip() )
	{
		kdDebug() << "Handling Information Dialog" << endl;

		//emit tableLocked(true);
		if (m_infoDialog)
			m_infoDialog->setElement(data()->element(number));
		else {
			m_infoDialog = new DetailedInfoDlg(data(), data()->element(number),
											   this, "detailedDlg" );

			// Remove the selection when this dialog finishes or hides.
			connect(m_infoDialog, SIGNAL(hidden()),
					m_PSE,        SLOT(unSelect()));
			connect(m_infoDialog, SIGNAL(elementChanged(int)),
					m_PSE,        SLOT(selectElement(int)));
		}
		m_infoDialog->show();
		//emit tableLocked(false);
	}
}

void Kalzium::slotNoLook()
{
	m_PSE->setLook( PSE::NOCOLOUR );
	gradient_action->setCurrentItem( -1 );
	m_actionGroups->setChecked( false );
	m_actionBlocks->setChecked( false );
	m_actionAcid->setChecked( false );
	m_actionFamily->setChecked( false );
	m_actionCrystal->setChecked( false );
	m_pLegendAction->setEnabled( true );
}

void Kalzium::slotLookGroups()
{
	m_PSE->setLook( PSE::GROUPS );
	gradient_action->setCurrentItem( -1 );
	m_actionNoScheme->setChecked( false );
	m_actionBlocks->setChecked( false );
	m_actionAcid->setChecked( false );
	m_actionFamily->setChecked( false );
	m_actionCrystal->setChecked( false );
	m_pLegendAction->setEnabled( true );
}

void Kalzium::slotLookBlocks()
{
	m_PSE->setLook( PSE::BLOCK );
	gradient_action->setCurrentItem( -1 );
	m_actionNoScheme->setChecked( false );
	m_actionGroups->setChecked( false );
	m_actionAcid->setChecked( false );
	m_actionFamily->setChecked( false );
	m_actionCrystal->setChecked( false );
	m_pLegendAction->setEnabled( true );
}

void Kalzium::slotLookAcidBehavior()
{
	m_PSE->setLook( PSE::ACIDIC );
	gradient_action->setCurrentItem( -1 );
	m_actionNoScheme->setChecked( false );
	m_actionGroups->setChecked( false );
	m_actionBlocks->setChecked( false );
	m_actionFamily->setChecked( false );
	m_actionCrystal->setChecked( false );
	m_pLegendAction->setEnabled( true );
}

void Kalzium::slotLookFamily()
{
	m_PSE->setLook( PSE::FAMILY );
	gradient_action->setCurrentItem( -1 );
	m_actionNoScheme->setChecked( false );
	m_actionGroups->setChecked( false );
	m_actionBlocks->setChecked( false );
	m_actionAcid->setChecked( false );
	m_actionCrystal->setChecked( false );
	m_pLegendAction->setEnabled( true );
}

void Kalzium::slotLookCrystal()
{
	m_PSE->setLook( PSE::CRYSTAL );
	gradient_action->setCurrentItem( -1 );
	m_actionNoScheme->setChecked( false );
	m_actionGroups->setChecked( false );
	m_actionBlocks->setChecked( false );
	m_actionAcid->setChecked( false );
	m_actionFamily->setChecked( false );
	m_pLegendAction->setEnabled( true );
}

void Kalzium::setFullDraw()
{
	m_PSE->setFullDraw();
}

void Kalzium::slotToolboxCurrentChanged( int id )
{
	m_PSE->unSelect();
	disconnect( m_PSE, SIGNAL( ElementClicked( int ) ), m_calcWidget, SLOT( slotButtonClicked( int ) ) );
//	m_calcWidget->clear();
	switch ( id )
	{
		case 0: // nothing
			emit tableLocked( false );
			m_calcWidget->clear();
			break;
		case 1: // molcalc
			connect( m_PSE, SIGNAL( ElementClicked( int ) ), m_calcWidget, SLOT( slotButtonClicked( int ) ) );
			emit tableLocked( true );
			break;
	}

}

void Kalzium::slotSelectedNumber( int num )
{
	m_detailWidget->setElement( data()->element( num ) );
}

KalziumDataObject* Kalzium::data() const { return pd->kalziumData; }

Kalzium::~Kalzium(){}

#include "kalzium.moc"

