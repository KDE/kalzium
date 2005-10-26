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
//#include "eqchemview.h"
#endif

#include <element.h>

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
#include "nuclideboard.h"
#include "config.h"
#include "simplecrystalviewer.h"
#include "kalziumnumerationtype.h"
#include "kalziumschemetype.h"
#include "kalziumgradienttype.h"
#include "printwidget.h"

#include <q3dockwindow.h>
#include <QLayout>
#include <QToolBox>
#include <QSlider>
#include <q3scrollview.h>
#include <QScrollArea>
#include <QSpinBox>

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

#define PeriodicTableView_MARGIN          5
#define IDS_ELEMENTINFO     7

Kalzium::Kalzium()
    : KMainWindow( 0, "Kalzium" )
{
	// reading the elements from file
	KalziumDataObject::instance();

//	QWidget *centralWidget = new QWidget( this, "centralWidget" );
//	m_pCentralLayout = new QVBoxLayout( centralWidget, PeriodicTableView_MARGIN, -1, "CentralLayout" );

	QScrollArea *helperSV = new QScrollArea( this );
	m_PeriodicTableView = new PeriodicTableView( helperSV->viewport() );
	m_PeriodicTableView->setObjectName( "PeriodicTableView" );
	helperSV->setWidget( m_PeriodicTableView );
//	helperSV->viewport()->setPaletteBackgroundColor(paletteBackgroundColor());  
//	helperSV->setFrameShape(QFrame::NoFrame);

	m_infoDialog = 0;
	m_toolboxCurrent = 0;

	connect( m_PeriodicTableView, SIGNAL( ElementClicked( int ) ), this, SLOT( openInformationDialog( int ) ));
	connect( m_PeriodicTableView, SIGNAL( MouseOver( int ) ), this, SLOT( slotStatusbar( int ) ));
	
	// layouting
	setCentralWidget( helperSV );

	setupSidebars();
	setupActions();

	// creating the glossary dialog and loading the glossaries we have
	m_glossarydlg = new GlossaryDialog( true, this );
	m_glossarydlg->setObjectName( QLatin1String( "glossary" ) );
	QString dir = KGlobal::dirs()->findResourceDir( "data", "kalzium/data/" );
	QString picturepath = dir + "kalzium/data/bg.jpg";
	KURL u = dir + "kalzium/data/knowledge.xml";
	Glossary *g = new Glossary( u );
	g->setName( i18n( "Knowledge" ) );
	g->setBackgroundPicture( picturepath );
	m_glossarydlg->addGlossary( g );
	u = dir + "kalzium/data/tools.xml";
	g = new Glossary( u, dir + "kalzium/data/toolpics/" );
	g->setName( i18n( "Tools" ) );
	g->setBackgroundPicture( picturepath );
	m_glossarydlg->addGlossary( g );

	setupStatusBar();

	m_prevNormalMode = KalziumPainter::NORMAL;
}

void Kalzium::setupActions()
{
	// the action for swiching look: color schemes and gradients
	QStringList looklist;
	QStringList schemes = KalziumSchemeTypeFactory::instance()->schemes();
	QString schemeprefix = i18n( "Scheme: %1" );
	for ( int i = 0; i < schemes.count(); i++ )
		looklist << schemeprefix.arg( schemes.at( i ) );
	QStringList gradients = KalziumGradientTypeFactory::instance()->gradients();
	QString gradprefix = i18n( "Gradient: %1" );
	for ( int i = 0; i < gradients.count(); i++ )
		looklist << gradprefix.arg( gradients.at( i ) );
	look_action = new KSelectAction( i18n( "&Look" ), 0, this, 0, actionCollection(), "view_look" );
	look_action->setItems(looklist);
	connect( look_action, SIGNAL( activated( int ) ), this, SLOT( slotSwitchtoLook( int ) ) );

	// the actions for switching numeration
	numeration_action = new KSelectAction (i18n("&Numeration"), 0, this, 0, actionCollection(), "view_numerationtype");
	numeration_action->setItems( KalziumNumerationTypeFactory::instance()->numerations() );
	connect (numeration_action, SIGNAL(activated(int)), this, SLOT(slotSwitchtoNumeration(int)));

	m_SidebarAction = new KAction(i18n("Show &Sidebar"), "sidebar", 0, this, SLOT(slotShowHideSidebar()), actionCollection(), "view_sidebar");
	
#ifdef HAVE_FACILE
//	m_EQSolverAction = new KAction(i18n("&Equation Solver..."), "eqchem", 0, this, SLOT(slotShowEQSolver()), actionCollection(), "tools_eqsolver");
#endif
	
	// tools actions
	m_pPlotAction = new KAction(i18n("&Plot Data..."), "plot", 0, this, SLOT(slotPlotData()), actionCollection(), "tools_plotdata");
	m_pIsotopeTableAction = new KAction(i18n("&Isotope Table..."), "isotopemap", 0, this, SLOT(slotIsotopeTable()), actionCollection(), "tools_isotopetable");
	m_pGlossaryAction = new KAction(i18n("&Glossary..."), "glossary", 0, this, SLOT(slotGlossary()), actionCollection(), "tools_glossary");
	m_pCrystalViewer = new KAction(i18n("&Crystal Viewer..."), "crystal", 0, this, SLOT(slotCrystalViewer()), actionCollection(), "tools_crystalviewer");

	// other period view options
	m_pLegendAction = new KAction(i18n("Show &Legend"), "legend", 0, this, SLOT(slotShowLegend()), actionCollection(), "view_legend");
	m_pTooltipAction = new KAction(i18n("Show &Tooltip"), "tooltip", 0, this, SLOT(slotEnableTooltips()), actionCollection(), "view_tooltip");
	
	// the standard actions
	KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
	KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );

//	slotShowScheme( Prefs::colorschemebox() );
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
		m_PeriodicTableView->showLegend(true);
		m_pLegendAction->setText( i18n( "Hide &Legend" ) );
	} else
	{
		m_PeriodicTableView->showLegend(false);
		m_pLegendAction->setText( i18n( "Show &Legend" ) );
	}

	if ( Prefs::tooltip() ) {
		m_PeriodicTableView->setTooltipsEnabled( true );
		m_pTooltipAction->setText( i18n( "Hide &Tooltips" ) );
	} else
	{
		m_PeriodicTableView->setTooltipsEnabled( false );
		m_pTooltipAction->setText( i18n( "Show &Tooltips" ) );
	}

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
}

void Kalzium::setupSidebars()
{
	m_dockWin = new Q3DockWindow( this );
	m_dockWin->setNewLine( true );
	m_dockWin->setFixedExtentWidth( 220 );
	m_dockWin->setResizeEnabled( true );
	m_dockWin->setFrameShape( QFrame::ToolBarPanel );
	m_dockWin->setCaption( i18n( "Sidebar" ) );
	m_dockWin->setCloseMode( Q3DockWindow::Always );
	
	QToolBox *m_toolbox = new QToolBox( m_dockWin );
	m_dockWin->setWidget( m_toolbox );

	QWidget *fake = new QWidget( m_dockWin );
	QVBoxLayout *lay = new QVBoxLayout( fake );
	lay->setMargin( 5 );
	lay->activate();
	m_detailWidget = new DetailedGraphicalOverview( fake );
	m_detailWidget->setObjectName( "DetailedGraphicalOverview" );
	m_detailWidget->setMinimumSize( 200, m_detailWidget->minimumSize().height() );
	connect( m_PeriodicTableView, SIGNAL( MouseOver( int ) ), this, SLOT( slotSelectedNumber( int ) ));
 	lay->addWidget( m_detailWidget );
	lay->addItem( new QSpacerItem( 10, 10, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ) );
	m_toolbox->addItem( fake, SmallIcon( "overview" ), i18n( "Overview" ) );
	
	m_calcWidget = new MolcalcWidget( m_dockWin, "molcalcwidget" );
	m_toolbox->addItem( m_calcWidget, SmallIcon( "calculate" ), i18n( "Calculate" ) );

	m_timeWidget = new TimeWidgetIMPL( this );
	m_timeWidget->setObjectName( "TimeWidget" );
	connect( m_timeWidget, SIGNAL( dateChanged( int ) ),
	         m_PeriodicTableView, SLOT( setDate( int ) ) );
	m_toolbox->addItem( m_timeWidget, SmallIcon( "timeline" ), i18n( "Timeline" ) );

	m_somWidget = new SOMWidgetIMPL( this );
	m_somWidget->setObjectName( "somWidget" );
	connect( m_somWidget, SIGNAL( temperatureChanged( int ) ),
	         m_PeriodicTableView, SLOT( setTemperature( int ) ) );
	m_toolbox->addItem( m_somWidget, SmallIcon( "statematter" ), i18n( "State of Matter" ) );
	
	connect( m_toolbox, SIGNAL( currentChanged( int ) ), this, SLOT( slotToolboxCurrentChanged( int ) ) );

	m_printWidget = new PrintWidget( this );
	m_toolbox->addItem( m_printWidget, SmallIcon( "fileprint" ), i18n( "Print Elements" ) );

	moveDockWindow( m_dockWin, Qt::DockLeft );
	setDockEnabled( /*m_dockWin, */Qt::DockTop, false );
	setDockEnabled( /*m_dockWin, */Qt::DockBottom, false );
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
//X 	EQChemDialog *dlg = new EQChemDialog( this );
//X 	
//X 	QWidget *page = new QWidget( dlg );
//X 	dlg->setMainWidget( page );
//X 	QVBoxLayout *vbox = new QVBoxLayout( page , 0, KDialogBase:: spacingHint() );
//X 	
//X 	eqchemView *eqsolver = new eqchemView( page );
//X 	eqsolver->setMinimumSize( 600,400 );
//X 
//X 	vbox->addWidget( eqsolver );
//X 
//X 	connect(dlg, SIGNAL(applyClicked()), eqsolver, SLOT(compute()));
//X 	dlg->show();
#endif
}

void Kalzium::slotCrystalViewer()
{
	SimpleCrystalViewer *dlg = new SimpleCrystalViewer( this );
	dlg->show();
}

void Kalzium::slotIsotopeTable()
{
	IsotopeTableDialog *ndialog = new IsotopeTableDialog( this );
	ndialog->show();
}

void Kalzium::slotPlotData()
{
	ElementDataViewer *edw = new ElementDataViewer( this, "edw" );
	edw->show();
}

void Kalzium::slotEnableTooltips()
{
	bool enabled = m_PeriodicTableView->tooltipsEnabled();
	enabled = !enabled;

	if ( enabled )
		m_pTooltipAction->setText( i18n( "Hide &Tooltips" ) );
	else
		m_pTooltipAction->setText( i18n( "Show &Tooltips" ) );

	m_PeriodicTableView->setTooltipsEnabled( enabled );
	
	Prefs::setTooltip( enabled );
	Prefs::writeConfig();
}

void Kalzium::slotShowLegend()
{
	if(m_PeriodicTableView->showLegend())
	{
		m_PeriodicTableView->showLegend(false);
		m_pLegendAction->setText( i18n( "Show &Legend" ) );
	}
	else
	{
		m_PeriodicTableView->showLegend(true);
		m_pLegendAction->setText( i18n( "Hide &Legend" ) );
	}
	m_PeriodicTableView->update();
	
	//save the settings
	Prefs::setShowlegend( m_PeriodicTableView->showLegend() ); 
	Prefs::writeConfig();
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

void Kalzium::slotSwitchtoNumeration( int index )
{
	m_PeriodicTableView->setNumeration( index );
	Prefs::setNumeration(index);
	Prefs::writeConfig();
}

void Kalzium::slotSwitchtoLook( int which )
{
	int id = which - KalziumSchemeTypeFactory::instance()->schemes().count();
	if ( id < 0 )
	{
		m_PeriodicTableView->activateColorScheme( which );
		if ( m_PeriodicTableView->mode() == KalziumPainter::GRADIENT )
			m_PeriodicTableView->setMode( KalziumPainter::NORMAL );
	}
	else
	{
		m_PeriodicTableView->setGradient( id );
		if ( m_PeriodicTableView->mode() == KalziumPainter::NORMAL )
			m_PeriodicTableView->setMode( KalziumPainter::GRADIENT );
	}
/*
	Prefs::setNumeration(index);
	Prefs::writeConfig();
*/
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
}
 
void Kalzium::setupStatusBar()
{
	statusBar()->insertItem(  "" , IDS_ELEMENTINFO, 1, false );
	statusBar()->setItemAlignment( IDS_ELEMENTINFO, Qt::AlignRight );
	statusBar()->show();
}

void Kalzium::slotStatusbar( int num )
{
	Element *e = KalziumDataObject::instance()->element( num );
	statusBar()->changeItem( i18n( "For example: \"Carbon (6), Mass: 12.0107 u\"", "%1 (%2), Mass: %3 u" )
			.arg( e->dataAsString( ChemicalDataObject::name ) )
			.arg( e->dataAsString( ChemicalDataObject::atomicNumber ) )
			.arg( e->dataAsString( ChemicalDataObject::mass ) ) , IDS_ELEMENTINFO );
}

void Kalzium::openInformationDialog( int number )
{
	if ( m_PeriodicTableView->showTooltip() )
	{
		kdDebug() << "Handling Information Dialog" << endl;

		if (m_infoDialog)
			m_infoDialog->setElement(KalziumDataObject::instance()->element(number));
		else
		{
			m_infoDialog = new DetailedInfoDlg(
			                        KalziumDataObject::instance()->element(number),
			                        this );

			// Remove the selection when this dialog finishes or hides.
			connect(m_infoDialog, SIGNAL(hidden()),
			        m_PeriodicTableView,        SLOT(unSelect()));
			connect(m_infoDialog, SIGNAL(elementChanged(int)),
			        m_PeriodicTableView,        SLOT(selectElement(int)));
		}
		m_infoDialog->show();
	}
}

void Kalzium::slotToolboxCurrentChanged( int id )
{
	m_PeriodicTableView->setTimeline( false );
	KalziumPainter::MODE cur = m_PeriodicTableView->mode();
	if ( ( id > 1 ) && ( cur == KalziumPainter::NORMAL ) || ( cur == KalziumPainter::GRADIENT ) )
		m_prevNormalMode = cur;
	m_PeriodicTableView->setMode( m_prevNormalMode );

	switch ( id )
	{
		case 0: // nothing
		case 1: // molcalc
			break;
		case 2: // timeline
			m_PeriodicTableView->setTimeline( true );
			m_PeriodicTableView->setDate( m_timeWidget->date() );
			break;
		case 3: // state of matter
			m_PeriodicTableView->setTemperature( m_somWidget->temperature() );
			m_PeriodicTableView->setMode( KalziumPainter::SOM );
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
}

Kalzium::~Kalzium()
{
}

#include "kalzium.moc"
