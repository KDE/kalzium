/***************************************************************************
    copyright            : (C) 2003, 2004, 2005, 2006 by Carsten Niehaus
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
#include <kdeeduglossary.h>

#include "prefs.h"
#include "ui_settings_colors.h"
#include "ui_settings_misc.h"
#include "ui_settings_units.h"
#include "elementdataviewer.h"
#include "detailinfodlg.h"
#include "periodictableview.h"
#include "molcalcwidget.h"
#include "detailedgraphicaloverview.h"
#include "propertysliderwidget.h"
#include "somwidget_impl.h"
#include "kalziumdataobject.h"
#include "nuclideboard.h"
#include "config.h"
#include "simplecrystalviewer.h"
#include "kalziumnumerationtype.h"
#include "kalziumschemetype.h"
#include "kalziumgradienttype.h"
#include "printwidget.h"

#include <QDockWidget>
#include <QLayout>
#include <QToolBox>
#include <QScrollArea>
#include <QEvent>
#include <QTimer>

#include <kconfigdialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kdebug.h>
#include <kaction.h>
#include <kapplication.h>
#include <kstatusbar.h>
#include <kstandarddirs.h>
#include <kdialogbase.h>

#define PeriodicTableView_MARGIN          5
#define IDS_ELEMENTINFO     7

Kalzium::Kalzium()
    : KMainWindow( 0, "Kalzium" )
{
	// reading the elements from file
	KalziumDataObject::instance();
	
	m_activeTypeSearch = false;
	m_searchTimer = new QTimer(this);
	connect( m_searchTimer, SIGNAL( timeout() ), this, SLOT( slotSearchElements() ) );

	QScrollArea *helperSV = new QScrollArea( this );
	m_PeriodicTableView = new PeriodicTableView( helperSV->viewport() );
	m_PeriodicTableView->setObjectName( "PeriodicTableView" );
	helperSV->setWidget( m_PeriodicTableView );
	helperSV->setFrameShape( QFrame::NoFrame );

	m_infoDialog = 0;
	m_toolboxCurrent = 0;

	connect( m_PeriodicTableView, SIGNAL( ElementClicked( int ) ), this, SLOT( openInformationDialog( int ) ));
	connect( m_PeriodicTableView, SIGNAL( MouseOver( int ) ), this, SLOT( elementHover( int ) ));
	
	// layouting
	setCentralWidget( helperSV );

	setupSidebars();
	setupActions();

	// adding the libkdeedu catalog
	KGlobal::locale()->insertCatalog( "libkdeedu" );

	// creating the glossary dialog and loading the glossaries we have
	m_glossarydlg = new GlossaryDialog( true, this );
	m_glossarydlg->setObjectName( QLatin1String( "glossary" ) );
	QString dir = KGlobal::dirs()->findResourceDir( "data", "kalzium/data/" );
	QString picturepath = dir + "kalzium/data/bg.jpg";
	KUrl u = KUrl::fromPath( dir + "kalzium/data/knowledge.xml" );
	Glossary *g = new Glossary( u );
	g->setName( i18n( "Knowledge" ) );
	g->setBackgroundPicture( picturepath );
	m_glossarydlg->addGlossary( g );
	u = KUrl::fromPath( dir + "kalzium/data/tools.xml" );
	g = new Glossary( u, dir + "kalzium/data/toolpics/" );
	g->setName( i18n( "Tools" ) );
	g->setBackgroundPicture( picturepath );
	m_glossarydlg->addGlossary( g );

	setupStatusBar();

	m_prevNormalMode = KalziumPainter::NORMAL;
}

static QStringList prependToListItems( const QStringList& list, const KLocalizedString& strprefix )
{
	QStringList l;
	for ( int i = 0; i < list.count(); i++ )
		l << strprefix.subs( list.at( i ) ).toString();
	return l;
}

void Kalzium::setupActions()
{
	// the action for swiching look: color schemes and gradients
	QStringList looklist;
	looklist << prependToListItems( KalziumSchemeTypeFactory::instance()->schemes(), ki18n( "Scheme: %1" ) );
	looklist << prependToListItems( KalziumGradientTypeFactory::instance()->gradients(), ki18n( "Gradient: %1" ) );
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

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
}

void Kalzium::setupSidebars()
{
	m_dockWin = new QDockWidget( i18n( "Sidebar" ), this );
	m_dockWin->setObjectName( QLatin1String( "kalzium-sidebar" ) );
	m_dockWin->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea ); 

	m_toolbox = new QToolBox( m_dockWin );
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
	m_toolbox->addItem( fake, SmallIconSet( "overview" ), i18n( "Overview" ) );
	
	m_calcWidget = new MolcalcWidget( m_dockWin );
	m_calcWidget->setObjectName( "molcalcwidget" );
	m_toolbox->addItem( m_calcWidget, SmallIconSet( "calculate" ), i18n( "Calculate" ) );
	
	m_SliderWidget = new PropertySliderWidget( this );
	m_SliderWidget->setObjectName( "m_SliderWidget" );
	m_toolbox->addItem( m_SliderWidget, SmallIconSet( "timeline" ), i18n( "Properties Slider" ) );
	connect( m_SliderWidget, SIGNAL(valueHasChanged( ChemicalDataObject::BlueObelisk, int ) ),
			m_PeriodicTableView, SLOT(setSliderValue( ChemicalDataObject::BlueObelisk, int ) ) );

	m_somWidget = new SOMWidgetIMPL( this );
	m_somWidget->setObjectName( "somWidget" );
	connect( m_somWidget, SIGNAL( temperatureChanged( int ) ),
	         m_PeriodicTableView, SLOT( setTemperature( int ) ) );
	m_toolbox->addItem( m_somWidget, SmallIconSet( "statematter" ), i18n( "State of Matter" ) );
	
	connect( m_toolbox, SIGNAL( currentChanged( int ) ), this, SLOT( slotToolboxCurrentChanged( int ) ) );

	m_printWidget = new PrintWidget( this );
	m_toolbox->addItem( m_printWidget, SmallIcon( "fileprint" ), i18n( "Print Elements" ) );

	addDockWidget( Qt::LeftDockWidgetArea, m_dockWin );
}

void Kalzium::slotGlossary()
{
	m_glossarydlg->show();
}

void Kalzium::slotShowEQSolver()
{
#ifdef HAVE_FACILE
//X 	EQChemDialog *dlg = new EQChemDialog( this );
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
	ElementDataViewer *edw = new ElementDataViewer( this );
	edw->show();
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
	
	//save the settings
	Prefs::setShowlegend( m_PeriodicTableView->showLegend() ); 
	Prefs::writeConfig();
}

void Kalzium::slotShowHideSidebar()
{
	if( m_dockWin->isVisible() )
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
	connect( dialog, SIGNAL( settingsChanged( const QString &) ), this , SLOT( slotUpdateSettings() ) );
	connect( dialog, SIGNAL( settingsChanged( const QString &) ), m_somWidget, SLOT( reloadUnits() ) );
	// colors page
	Ui_setupColors ui_colors;
	QWidget *w_colors = new QWidget( 0 );
	w_colors->setObjectName( "colors_page" );
	ui_colors.setupUi( w_colors );
	dialog->addPage( w_colors, i18n( "Colors" ), "colorize" );
 	// units page
	Ui_setupUnits ui_units;
	QWidget *w_units = new QWidget( 0 );
	w_units->setObjectName( "units_page" );
	ui_units.setupUi( w_units );
	dialog->addPage( w_units, i18n( "Units" ), "gear" );
	// misc page
	Ui_setupMisc ui_misc;
	QWidget *w_misc = new QWidget( 0 );
	w_misc->setObjectName( "miscpage" );
	ui_misc.setupUi( w_misc );
	dialog->addPage( w_misc, i18n( "Miscellaneous" ), "misc" );
	// showing the dialog
	dialog->show();
}

void Kalzium::slotUpdateSettings()
{
}

void Kalzium::setupStatusBar()
{
	statusBar()->insertItem( "", 0, 0 );
	statusBar()->setItemAlignment( 0, Qt::AlignRight );
	
	statusBar()->insertItem(  "" , IDS_ELEMENTINFO, 1 );
	statusBar()->setItemAlignment( IDS_ELEMENTINFO, Qt::AlignRight );
	statusBar()->show();
}

void Kalzium::elementHover( int num )
{
	Element *e = KalziumDataObject::instance()->element( num );
	statusBar()->changeItem( i18nc( "For example: \"Carbon (6), Mass: 12.0107 u\"", "%1 (%2), Mass: %3 u" ,
			  e->dataAsString( ChemicalDataObject::name ) ,
			  e->dataAsString( ChemicalDataObject::atomicNumber ) ,
			  e->dataAsString( ChemicalDataObject::mass ) ) , IDS_ELEMENTINFO );

	QColor schemeColor = m_PeriodicTableView->scheme()->elementBrush( num, QRect() ).color();
	m_detailWidget->setBackgroundColor( schemeColor );
}

void Kalzium::openInformationDialog( int number )
{
	if (m_infoDialog)
		m_infoDialog->setElement( number );
	else
	{
		m_infoDialog = new DetailedInfoDlg(number, this );

		// Remove the selection when this dialog finishes or hides.
		connect(m_infoDialog, SIGNAL(hidden()),
				m_PeriodicTableView,        SLOT(unSelect()));
		connect(m_infoDialog, SIGNAL(elementChanged(int)),
				m_PeriodicTableView,        SLOT(selectElement(int)));
	}
	m_infoDialog->show();
	QColor schemeColor = m_PeriodicTableView->scheme()->elementBrush( number, QRect() ).color();
	m_infoDialog->setOverviewBackgroundColor( schemeColor );
}

void Kalzium::slotToolboxCurrentChanged( int id )
{
	KalziumPainter::MODE cur = m_PeriodicTableView->mode();
	if ( ( id > 1 ) && ( cur == KalziumPainter::NORMAL ) || ( cur == KalziumPainter::GRADIENT ) )
		m_prevNormalMode = cur;
	m_PeriodicTableView->setMode( m_prevNormalMode );

	switch ( id )
	{
		case 0: // nothing
		case 1: // molcalc
			break;
		case 2: // sliderwidget
			m_PeriodicTableView->setMode( KalziumPainter::SLIDE );
			break;
		case 3: // state of matter
			m_PeriodicTableView->setTemperature( m_somWidget->temperature() );
			m_PeriodicTableView->setMode( KalziumPainter::SOM );
			break;
	}
	if ( m_dockWin->isVisible() )
		m_toolboxCurrent = id;
}

void Kalzium::slotSelectedNumber( int num )
{
	m_detailWidget->setElement( num );
}

void Kalzium::slotSidebarVisibilityChanged( bool visible )
{
	if( visible )
		slotToolboxCurrentChanged( m_toolboxCurrent );
	else
		slotToolboxCurrentChanged( 0 );

	//save the settings
	Prefs::setShowsidebar( m_dockWin->isVisible() );
	Prefs::writeConfig();
}

Kalzium::~Kalzium()
{
}

void Kalzium::keyPressEvent( QKeyEvent *e)
{
	e->accept();
	
	if ( m_activeTypeSearch )
	{
		m_searchTimer->stop();

		if (  e->key() == Qt::Key_Backspace )
		{
			if ( m_typeAheadString.length( ) > 1 )
			{
				m_typeAheadString = m_typeAheadString.left(  m_typeAheadString.length() - 1 );
				statusBar()->changeItem(i18n( "Searching for: %1", m_typeAheadString ),0);
			}
			else
				findAheadStop();
		}
		else if (  e->key() == Qt::Key_Escape || e->key() == Qt::Key_Return )
		{
			findAheadStop();
		}
		else if (  !e->text().isEmpty() )
		{
			m_typeAheadString += e->text();
			statusBar()->changeItem(i18n( "Searching for: %1", m_typeAheadString ),0);
		}

		m_searchTimer->start( 500 );

		return;
	}
	else if (  e->key() == '/' ){
		grabKeyboard();
		m_activeTypeSearch = true;
		statusBar()->changeItem( i18n( "Starting the search -- matching elements will be highlighted" ), 0 );
	}
}

void Kalzium::findAheadStop()
{
	m_searchTimer->stop();
	m_activeTypeSearch = false;
	KalziumDataObject::instance()->stopSearch();
	m_typeAheadString = QString();
	releaseKeyboard();
	statusBar()->changeItem( QString(),0); //don't display the search in the statusbar
}

void Kalzium::slotSearchElements()
{
	if ( m_typeAheadString != QString() )//don't search if emtpy
		KalziumDataObject::instance()->findElements( m_typeAheadString );

	m_PeriodicTableView->setFullDraw();
	m_PeriodicTableView->update();
}

#include "kalzium.moc"
