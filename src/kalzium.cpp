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

#ifdef HAVE_FACILE
#include "eqchemview.h"
#endif

#include "element.h"
#include "prefs.h"
#include "settings_colors.h"
#include "settings_misc.h"
#include "settings_units.h"
#include "elementdataviewer.h"
#include "detailinfodlg.h"
#include "periodictableview.h"
#include "molcalcwidget.h"
#include "detailedgraphicaloverview.h"
#include "timewidget_impl.h"
#include "somwidget_impl.h"
#include "kalziumdataobject.h"
#include "kalziumutils.h"
#include "config.h"

#include <qdockwindow.h>
#include <qlayout.h>
#include <qtoolbox.h>
#include <qslider.h>
#include <qscrollview.h>
#include <qspinbox.h>

#include <kconfigdialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kdebug.h>
#include <kaction.h>
#include <kapplication.h>
#include <kstatusbar.h>
#include <kstandarddirs.h>
#include <kdialogbase.h>
#include <knuminput.h>

#include <kdeeduglossary.h>

#define PerodicTableView_MARGIN          5
#define IDS_ELEMENTINFO     7

Kalzium::Kalzium()
    : KMainWindow( 0, "Kalzium" )
{
	// reading the elements from file
	KalziumDataObject::instance();

	QWidget *centralWidget = new QWidget( this, "centralWidget" );
	m_pCentralLayout = new QVBoxLayout( centralWidget, PerodicTableView_MARGIN, -1, "CentralLayout" );
	
	QScrollView *helperSV = new QScrollView(centralWidget);
	m_pCentralLayout->addWidget(helperSV);
	helperSV->viewport()->setPaletteBackgroundColor(paletteBackgroundColor());  
	helperSV->setFrameShape(QFrame::NoFrame);

	m_PerodicTableView = new PerodicTableView( helperSV->viewport(), "PerodicTableView");
	helperSV->addChild( m_PerodicTableView );
	m_infoDialog = 0;
	m_toolboxCurrent = 0;

	connect( m_PerodicTableView, SIGNAL( ElementClicked( int ) ), this, SLOT( openInformationDialog( int ) ));
	connect( m_PerodicTableView, SIGNAL( MouseOver( int ) ), this, SLOT( slotStatusbar( int ) ));
	
	// layouting
	setCentralWidget( centralWidget );
	centralWidget->show();

	setupSidebars();
	setupActions();

	// adding the libkdeedu catalog
	KGlobal::locale()->insertCatalogue( "libkdeedu" );

	// creating the glossary dialog and loading the glossaries we have
	m_glossarydlg = new GlossaryDialog( true, this, "glossary" );
	QString dir = KGlobal::dirs()->findResourceDir( "data", "kalzium/data/" );
	QString picturepath = dir + "kalzium/data/bg.jpg";
	KURL u = dir + "kalzium/data/knowledge.xml";
	Glossary *g = Glossary::readFromXML( u );
	g->setName( i18n( "Knowledge" ) );
	g->setBackgroundPicture( picturepath );
	m_glossarydlg->addGlossary( g );
	u = dir + "kalzium/data/tools.xml";
	g = Glossary::readFromXML( u, dir+"kalzium/data/toolpics/" );
	g->setName( i18n( "Tools" ) );
	g->setBackgroundPicture( picturepath );
	m_glossarydlg->addGlossary( g );

	setupStatusBar();
}

void Kalzium::setupActions()
{
	m_actionNoScheme = new KToggleAction(i18n("&No Color Scheme"), 0, this, SLOT(slotNoLook()), actionCollection(), "view_look_noscheme");

	// the actions for the color schemes
	m_actionGroups = new KToggleAction(i18n("Show &Groups"), 0, this, SLOT(slotLookGroups()), actionCollection(), "view_look_groups");
	m_actionBlocks = new KToggleAction(i18n("Show &Blocks"), 0, this, SLOT(slotLookBlocks()), actionCollection(), "view_look_blocks");
	m_actionAcid = new KToggleAction(i18n("Show &Acid Behavior"), 0, this, SLOT(slotLookAcidBehavior()), actionCollection(), "view_look_acid");
	m_actionFamily = new KToggleAction(i18n("Show &Family"), 0, this, SLOT(slotLookFamily()), actionCollection(), "view_look_family");
	m_actionCrystal = new KToggleAction(i18n("Show &Crystal Structures"), 0, this, SLOT(slotLookCrystal()), actionCollection(), "view_look_crystal");

	//the actions for switching PerodicTableView
	QStringList gradientlist;
	gradientlist.append(i18n("Atomic Radius"));
	gradientlist.append(i18n("Covalent Radius"));
	gradientlist.append(i18n("van der Waals Radius"));
	gradientlist.append(i18n("Atomic Mass"));
	gradientlist.append(i18n("Density"));
	gradientlist.append(i18n("Boiling Point"));
	gradientlist.append(i18n("Melting Point"));
	gradientlist.append(i18n("Electronegativity"));
	gradientlist.append(i18n("Electron Affinity"));
	gradient_action = new KSelectAction(i18n("&Gradient"), 0, this, 0, actionCollection(), "view_look_gradmenu");
	gradient_action->setItems(gradientlist);
	connect (gradient_action, SIGNAL(activated(int)), this, SLOT(slotSwitchtoGradient(int)));

	// the actions for switching PerodicTableView
	QStringList numlist;
	numlist.append(i18n("No N&umeration"));
	numlist.append(i18n("Show &IUPAC"));
	numlist.append(i18n("Show &CAS"));
	numlist.append(i18n("Show &Old IUPAC"));
	numeration_action = new KSelectAction (i18n("&Numeration"), 0, this, 0, actionCollection(), "view_numerationtype");
	numeration_action->setItems(numlist);
	numeration_action->setCurrentItem(Prefs::numeration()); 
	connect (numeration_action, SIGNAL(activated(int)), this, SLOT(slotSwitchtoNumeration(int)));

	m_SidebarAction = new KAction(i18n("Show &Sidebar"), "sidebar", 0, this, SLOT(slotShowHideSidebar()), actionCollection(), "view_sidebar");
	
#ifdef HAVE_FACILE
	m_EQSolverAction = new KAction(i18n("&Equation Solver..."), "eqchem", 0, this, SLOT(slotShowEQSolver()), actionCollection(), "tools_eqsolver");
#endif
	
	// tools actions
	m_pPlotAction = new KAction(i18n("&Plot Data..."), "plot", 0, this, SLOT(slotPlotData()), actionCollection(), "tools_plotdata");
	m_pGlossaryAction = new KAction(i18n("&Glossary..."), "glossary", 0, this, SLOT(slotGlossary()), actionCollection(), "tools_glossary");

	// other period view options
	m_pLegendAction = new KAction(i18n("Show &Legend"), "legend", 0, this, SLOT(slotShowLegend()), actionCollection(), "view_legend");
	m_pTooltipAction = new KAction(i18n("Show &Tooltip"), "tooltip", 0, this, SLOT(slotEnableTooltips()), actionCollection(), "view_tooltip");
	
	// the standard actions
	KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
	KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );

	slotShowScheme( Prefs::colorschemebox() );
	slotSwitchtoNumeration( Prefs::numeration() );

	if ( Prefs::showsidebar() ) {
		m_dockWin->show();
		m_SidebarAction->setText( i18n( "Hide &Sidebar" ) );
	}
	else {
		m_dockWin->hide();
		m_SidebarAction->setText( i18n( "Show &Sidebar" ) );
	}

	if ( Prefs::showlegend() ) {
		m_PerodicTableView->showLegend(true);
		m_pLegendAction->setText( i18n( "Hide &Legend" ) );
	} else
	{
		m_PerodicTableView->showLegend(false);
		m_pLegendAction->setText( i18n( "Show &Legend" ) );
	}

	if ( Prefs::tooltip() ) {
		m_PerodicTableView->setTooltipsEnabled( true );
		m_pTooltipAction->setText( i18n( "Hide &Tooltips" ) );
	} else
	{
		m_PerodicTableView->setTooltipsEnabled( false );
		m_pTooltipAction->setText( i18n( "Show &Tooltips" ) );
	}

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
}

void Kalzium::setupSidebars()
{
	m_dockWin = new QDockWindow( this );
	m_dockWin->setNewLine( true );
	m_dockWin->setFixedExtentWidth( 220 );
	m_dockWin->setResizeEnabled( true );
	m_dockWin->setFrameShape( QFrame::ToolBarPanel );
	m_dockWin->setCaption( i18n( "Sidebar" ) );
	m_dockWin->setCloseMode( QDockWindow::Always );
	
	QToolBox *m_toolbox = new QToolBox( m_dockWin );
	m_dockWin->setWidget( m_toolbox );

	QWidget *fake = new QWidget( m_dockWin );
	QVBoxLayout *lay = new QVBoxLayout( fake, 5 );
	lay->activate();
	m_detailWidget = new DetailedGraphicalOverview( fake, "DetailedGraphicalOverview" );
	m_detailWidget->setMinimumSize( 200, m_detailWidget->minimumSize().height() );
	connect( m_PerodicTableView, SIGNAL( MouseOver( int ) ), this, SLOT( slotSelectedNumber( int ) ));
 	lay->addWidget( m_detailWidget );
	lay->addItem( new QSpacerItem( 10, 10, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ) );
	m_toolbox->addItem( fake, SmallIcon( "overview" ), i18n( "Overview" ) );
	
	m_calcWidget = new MolcalcWidget( m_dockWin, "molcalcwidget" );
	m_toolbox->addItem( m_calcWidget, SmallIcon( "calculate" ), i18n( "Calculate" ) );

	m_timeWidget = new TimeWidgetIMPL( this, "TimeWidget" );
	connect( m_timeWidget->time_slider, SIGNAL( valueChanged( int ) ),
	         m_PerodicTableView, SLOT( setDate( int ) ) );
	connect( m_timeWidget->time_slider, SIGNAL( valueChanged( int ) ),
	         m_timeWidget, SLOT( slotChanged( int ) ) );
	connect( m_timeWidget->Number1, SIGNAL( valueChanged( int ) ),
	         m_timeWidget, SLOT( slotChanged( int ) ) );
	m_toolbox->addItem( m_timeWidget, SmallIcon( "timeline" ), i18n( "Timeline" ) );

	m_somWidget = new SOMWidgetIMPL( this, "somWidget" );
	connect( m_somWidget->temp_slider, SIGNAL( valueChanged( int ) ),
	         m_PerodicTableView, SLOT( setTemperature( int ) ) );
	m_toolbox->addItem( m_somWidget, SmallIcon( "statematter" ), i18n( "State of Matter" ) );
	
	connect( m_toolbox, SIGNAL( currentChanged( int ) ), this, SLOT( slotToolboxCurrentChanged( int ) ) );

	moveDockWindow( m_dockWin, DockLeft );
	setDockEnabled( /*m_dockWin, */DockTop, false );
	setDockEnabled( /*m_dockWin, */DockBottom, false );
	m_dockWin->hide();
	connect( m_dockWin, SIGNAL(visibilityChanged(bool)), this, SLOT(slotSidebarVisibilityChanged(bool)));

}

void Kalzium::slotGlossary()
{
	m_glossarydlg->show();
}

void Kalzium::slotShowEQSolver()
{
#ifdef HAVE_FACILE
	EQChemDialog *dlg = new EQChemDialog( this );
	
	QWidget *page = new QWidget( dlg );
	dlg->setMainWidget( page );
	QVBoxLayout *vbox = new QVBoxLayout( page , 0, KDialogBase:: spacingHint() );
	
	eqchemView *eqsolver = new eqchemView( page );
	eqsolver->setMinimumSize( 600,400 );

	vbox->addWidget( eqsolver );
        connect(dlg, SIGNAL(applyClicked()), eqsolver, SLOT(compute()));
	dlg->show();
#endif
}

void Kalzium::slotPlotData()
{
	ElementDataViewer *edw = new ElementDataViewer( this, "edw" );
	edw->show();
}

void Kalzium::slotEnableTooltips()
{
	bool enabled = m_PerodicTableView->tooltipsEnabled();
	enabled = !enabled;

	if ( enabled )
		m_pTooltipAction->setText( i18n( "Hide &Tooltips" ) );
	else
		m_pTooltipAction->setText( i18n( "Show &Tooltips" ) );

	m_PerodicTableView->setTooltipsEnabled( enabled );
	
	Prefs::setTooltip( enabled );
	Prefs::writeConfig();
}

void Kalzium::slotShowLegend()
{
	if(m_PerodicTableView->showLegend())
	{
		m_PerodicTableView->showLegend(false);
		m_pLegendAction->setText( i18n( "Show &Legend" ) );
	}
	else
	{
		m_PerodicTableView->showLegend(true);
		m_pLegendAction->setText( i18n( "Hide &Legend" ) );
	}
	m_PerodicTableView->update();
	
	//save the settings
	Prefs::setShowlegend( m_PerodicTableView->showLegend() ); 
	Prefs::writeConfig();
 
	//JH: redraw the full table next time
	setFullDraw();
}

void Kalzium::slotShowHideSidebar()
{
	if( m_dockWin->isShown() )
	{
		m_dockWin->hide();
		Prefs::setShowsidebar( false );
		m_SidebarAction->setText( i18n( "Show &Sidebar" ) );
	}
	else
	{
		m_dockWin->show();
		Prefs::setShowsidebar( true );
		m_SidebarAction->setText( i18n( "Hide &Sidebar" ) );
	}
	
	//save the settings
	Prefs::writeConfig();
}

void Kalzium::slotShowScheme(int i)
{
	switch ( i )
	{
		case PerodicTableView::GROUPS:
			m_actionGroups->setChecked( true );
			m_PerodicTableView->setLook( PerodicTableView::GROUPS );
			break;
		case PerodicTableView::BLOCK:
			m_actionBlocks->setChecked( true );
			m_PerodicTableView->setLook( PerodicTableView::BLOCK );
			break;
		case PerodicTableView::ACIDIC:
			m_actionAcid->setChecked( true );
			m_PerodicTableView->setLook( PerodicTableView::ACIDIC );
			break;
		case PerodicTableView::FAMILY:
			m_actionFamily->setChecked( true );
			m_PerodicTableView->setLook( PerodicTableView::FAMILY );
			break;
		case PerodicTableView::CRYSTAL:
			m_actionCrystal->setChecked( true );
			m_PerodicTableView->setLook( PerodicTableView::CRYSTAL );
			break;
		case PerodicTableView::GRADIENT:
			gradient_action->setCurrentItem( Prefs::gradient()-1 );
			m_PerodicTableView->setLook( PerodicTableView::GRADIENT, Prefs::gradient() );
			break;
		case PerodicTableView::NOCOLOUR:
		default:
			m_actionNoScheme->setChecked( true );
			m_PerodicTableView->setLook( PerodicTableView::NOCOLOUR );
	}
}

void Kalzium::slotSwitchtoGradient( int index )
{
	m_PerodicTableView->setLook( PerodicTableView::GRADIENT, index + 1 );
	m_actionNoScheme->setChecked( false );
	m_actionGroups->setChecked( false );
	m_actionBlocks->setChecked( false );
	m_actionAcid->setChecked( false );
	m_actionFamily->setChecked( false );
	m_actionCrystal->setChecked( false );
	m_pLegendAction->setEnabled( false );

	Prefs::setGradient(index + 1);
	Prefs::writeConfig();
}

void Kalzium::slotSwitchtoNumeration( int index )
{
	m_PerodicTableView->setNumerationType( ( PerodicTableView::NUMERATIONTYPE )index );
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
	connect( dialog, SIGNAL( settingsChanged() ), m_somWidget, SLOT( reloadUnits() ) );
	dialog->addPage( new setColors( 0, "colors_page"), i18n("Colors"), "colorize");
	dialog->addPage( new setupUnits( 0, "units_page"), i18n("Units"), "gear");
	dialog->addPage( new setupMisc( 0, "miscpage" ), i18n( "Miscellaneous" ), "misc" );
	dialog->show();
}

void Kalzium::slotUpdateSettings()
{
	m_PerodicTableView->reloadColours();
	m_PerodicTableView->setFullDraw();
}
 
void Kalzium::setupStatusBar()
{
	statusBar()->insertItem(  "" , IDS_ELEMENTINFO, 1, false );
	statusBar()->setItemAlignment( IDS_ELEMENTINFO, AlignRight );
	statusBar()->show();
}

void Kalzium::slotStatusbar( int num )
{
	Element *e = KalziumDataObject::instance()->element( num );
	statusBar()->changeItem( i18n( "For example: \"Carbon (6), Mass: 12.0107 u\"", "%1 (%2), Mass: %3 u" ).arg( e->elname() ).arg(e->number() ).arg( KalziumUtils::localizedValue( e->mass(), 6 ) ) , IDS_ELEMENTINFO );
}

void Kalzium::openInformationDialog( int number )
{
	if ( m_PerodicTableView->showTooltip() )
	{
		kdDebug() << "Handling Information Dialog" << endl;

		if (m_infoDialog)
			m_infoDialog->setElement(KalziumDataObject::instance()->element(number));
		else
		{
			m_infoDialog = new DetailedInfoDlg(
			                        KalziumDataObject::instance()->element(number),
			                        this, "detailedDlg" );

			// Remove the selection when this dialog finishes or hides.
			connect(m_infoDialog, SIGNAL(hidden()),
			        m_PerodicTableView,        SLOT(unSelect()));
			connect(m_infoDialog, SIGNAL(elementChanged(int)),
			        m_PerodicTableView,        SLOT(selectElement(int)));
		}
		m_infoDialog->show();
	}
}

void Kalzium::slotNoLook()
{
	m_PerodicTableView->setLook( PerodicTableView::NOCOLOUR );
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
	m_PerodicTableView->setLook( PerodicTableView::GROUPS );
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
	m_PerodicTableView->setLook( PerodicTableView::BLOCK );
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
	m_PerodicTableView->setLook( PerodicTableView::ACIDIC );
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
	m_PerodicTableView->setLook( PerodicTableView::FAMILY );
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
	m_PerodicTableView->setLook( PerodicTableView::CRYSTAL );
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
	m_PerodicTableView->setFullDraw();
}

void Kalzium::slotToolboxCurrentChanged( int id )
{
	m_PerodicTableView->unSelect();
	m_PerodicTableView->setTimeline( false );
	m_PerodicTableView->activateSOMMode( false );

	switch ( id )
	{
		case 0: // nothing
			break;
		case 1: // molcalc
			break;
		case 2: // timeline
			m_PerodicTableView->setTimeline( true );
			m_PerodicTableView->setDate( m_timeWidget->time_slider->value() );
			break;
		case 3: // state of matter
			m_PerodicTableView->activateSOMMode( true );
			m_PerodicTableView->setTemperature( m_somWidget->temp_slider->value() );
			break;
	}
	if ( m_dockWin->isShown() )
		m_toolboxCurrent = id;
}

void Kalzium::slotSelectedNumber( int num )
{
	m_detailWidget->setElement( KalziumDataObject::instance()->element( num ) );
}

void Kalzium::slotSidebarVisibilityChanged( bool visible )
{
	if( visible )
		slotToolboxCurrentChanged( m_toolboxCurrent );
	else
		slotToolboxCurrentChanged( 0 );

	//save the settings
	Prefs::setShowsidebar( m_dockWin->isShown() );
	Prefs::writeConfig();
 
	//JH: redraw the full table next time
	setFullDraw();
}

Kalzium::~Kalzium()
{
}

#include "kalzium.moc"
