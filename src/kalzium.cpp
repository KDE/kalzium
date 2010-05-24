/***************************************************************************
    copyright            : (C) 2003, 2004, 2005, 2006, 2007 by Carsten Niehaus
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

#include <kapplication.h>
#include <element.h>
#include <kdeeduglossary.h>
#include <kactioncollection.h>
#include "prefs.h"
#include "ui_settings_colors.h"
#include "ui_settings_misc.h"
#include "ui_settings_units.h"
#include "ui_settings_web.h"
#include "ui_settings_calc.h"
#include "elementdataviewer.h"
#include "detailinfodlg.h"
#include "periodictableview.h"
#include "molcalcwidget.h"
#include "detailedgraphicaloverview.h"
#include "somwidget_impl.h"
#include "timewidget_impl.h"
#include "kalziumdataobject.h"
#include "isotopetabledialog.h"
#include "kalziumnumerationtype.h"
#include "kalziumschemetype.h"
#include "kalziumgradienttype.h"
#include "kalziumtabletype.h"
#include "rsdialog.h"
#include "tablesdialog.h"
#include "legendwidget.h"
#include "exportdialog.h"
#include "search.h"
#include "searchwidget.h"

#include "tableinfowidget.h"

#include <config-kalzium.h>

#include "calculator/calculator.h"

#ifdef HAVE_FACILE
#include "eqchemview.h"
#endif

#ifdef HAVE_OPENBABEL2
#if defined(HAVE_EIGEN) && defined(HAVE_AVOGADRO)
#include "tools/moleculeview.h"
#endif
#include "tools/obconverter.h"
#endif

#include <QDockWidget>
#include <QToolBox>
#include <QScrollArea>
#include <QKeyEvent>
#include <QRegExp>

#include <kmessagebox.h>
#include <kconfigdialog.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kparts/part.h>
#include <kselectaction.h>
#include <kstatusbar.h>
#include <kstandarddirs.h>
#include <kstandardaction.h>
#include <kicon.h>
#include <kservicetypetrader.h>
#include <kurl.h>
#include <kfiledialog.h>
#include <KLocale>
#include <KPluginLoader>
#include <KTabWidget>

#define PeriodicTableView_MARGIN          5
#define IDS_ELEMENTINFO     7

Kalzium::Kalzium() : KXmlGuiWindow( 0 )
{
    setObjectName( "KalziumMainWindow" );

    // adding the libkdeedu catalog
    KGlobal::locale()->insertCatalog( "libkdeedu" );

    // reading the elements from file
    KalziumDataObject::instance();

    Search *newsearch = new Search();
    KalziumDataObject::instance()->setSearch( newsearch );
    connect( newsearch, SIGNAL( searchChanged() ), this, SLOT( slotSearchElements() ) );
    connect( newsearch, SIGNAL( searchReset() ), this, SLOT( slotSearchElements() ) );

    QWidget *fakemain = new QWidget( this );
    QVBoxLayout *fakelay = new QVBoxLayout( fakemain );
    fakelay->setMargin( 0 );
    fakelay->setSpacing( 2 );

    m_searchWidget = new SearchWidget( fakemain );
    fakelay->addWidget( m_searchWidget );

    QScrollArea *helperSV = new QScrollArea( fakemain );
    m_PeriodicTableView = new PeriodicTableView( helperSV->viewport() );
    m_PeriodicTableView->setObjectName( "PeriodicTableView" );
    helperSV->setWidget( m_PeriodicTableView );
    helperSV->setFrameShape( QFrame::NoFrame );
    fakelay->addWidget( helperSV );

    m_legendWidget = new LegendWidget( this );
    connect( m_PeriodicTableView, SIGNAL(ModeChanged( KalziumPainter::MODE) ),
             m_legendWidget, SLOT(setMode(KalziumPainter::MODE) ) );
    connect( m_PeriodicTableView, SIGNAL(TableTypeChanged( KalziumTableType* ) ),
             m_legendWidget, SLOT( setTableType( KalziumTableType * ) ) );
    connect( m_PeriodicTableView, SIGNAL(GradientTypeChanged( KalziumGradientType* ) ),
             m_legendWidget, SLOT( setGradientType( KalziumGradientType * ) ) );
    connect( m_PeriodicTableView, SIGNAL( SchemeChanged( KalziumSchemeType * ) ),
             m_legendWidget, SLOT( setScheme( KalziumSchemeType * ) ) );

    m_TableInfoWidget = new TableInfoWidget( this );
    connect( m_PeriodicTableView, SIGNAL(TableTypeChanged( KalziumTableType* ) ),
             m_TableInfoWidget, SLOT( setTableType( KalziumTableType * ) ) );

    m_infoDialog = 0;
    m_toolboxCurrent = 0;
    m_exportDialog = 0;
    m_prevNormalMode = KalziumPainter::NORMAL;

    connect( m_PeriodicTableView, SIGNAL( ElementClicked( int ) ), this, SLOT( openInformationDialog( int ) ));
    connect( m_PeriodicTableView, SIGNAL( MouseOver( int ) ), this, SLOT( elementHover( int ) ));

    // layouting
    setCentralWidget( fakemain );
    setupSidebars();
    setupActions();

    // creating the glossary dialog and loading the glossaries we have
    m_glossarydlg = new GlossaryDialog( this );
    m_glossarydlg->setObjectName( QLatin1String( "glossary" ) );
    QString dir = KGlobal::dirs()->findResourceDir( "data", "kalzium/data/" );
    QString picturepath = dir + "kalzium/data/bg.jpg";
    KUrl u = KUrl::fromPath( dir + "kalzium/data/knowledge.xml" );
    Glossary *g = new Glossary( u );
    g->setName( i18n( "Knowledge" ) );
    g->setBackgroundPicture( picturepath );
    m_glossarydlg->addGlossary( g, true );
    u = KUrl::fromPath( dir + "kalzium/data/tools.xml" );
    g = new Glossary( u, dir + "kalzium/data/toolpics/" );
    g->setName( i18n( "Tools" ) );
    g->setBackgroundPicture( picturepath );
    m_glossarydlg->addGlossary( g, true );

    setupStatusBar();
}

void Kalzium::setupActions()
{
    export_action = actionCollection()->add<QAction>( "file_exporter" );
    export_action->setText( i18n("&Export Data...") );
    connect( export_action, SIGNAL( triggered( bool ) ), this, SLOT( slotShowExportDialog() ) );

    // the action for swiching look: color schemes and gradients
    QStringList schemes = KalziumSchemeTypeFactory::instance()->schemes();
    QStringList gradients = QStringList(i18n("No Gradient"));
    gradients << KalziumGradientTypeFactory::instance()->gradients();
//     look_action_menu_schemes =  actionCollection()->add<KSelectAction>( "view_look_scheme" );
//     look_action_menu_schemes->setText( i18n( "&Scheme" ) );
//     look_action_menu_schemes->setItems(schemes);
//     connect(look_action_menu_schemes, SIGNAL(triggered(int)), this, SLOT(slotSwitchtoLookScheme(int)));

//     look_action_menu_gradients =  actionCollection()->add<KSelectAction>( "view_look_gradient" );
//     look_action_menu_gradients->setText( i18n( "&Gradient" ) );
//     look_action_menu_gradients->setItems(gradients);
//     connect(look_action_menu_gradients, SIGNAL(triggered(int)), this, SLOT(slotSwitchtoLookGradient(int)));

    // the action for swiching look: schemes
    look_action_schemes = actionCollection()->add<KSelectAction>( "view_look_onlyschemes" );
    look_action_schemes->setText( i18n( "&Scheme" ) );
    look_action_schemes->setItems( schemes );
    look_action_schemes->setToolBarMode( KSelectAction::MenuMode );
    look_action_schemes->setToolButtonPopupMode( QToolButton::InstantPopup );
    connect( look_action_schemes, SIGNAL( triggered( int ) ), this, SLOT( slotSwitchtoLookScheme( int ) ) );

    // the action for swiching look: gradients
    look_action_gradients = actionCollection()->add<KSelectAction>( "view_look_onlygradients" );
    look_action_gradients->setText( i18n( "&Gradients" ) );
    look_action_gradients->setItems( gradients );
    look_action_gradients->setToolBarMode( KSelectAction::MenuMode );
    look_action_gradients->setToolButtonPopupMode( QToolButton::InstantPopup );
    connect( look_action_gradients, SIGNAL( triggered( int ) ), this, SLOT( slotSwitchtoLookGradient( int ) ) );

    // the action for swiching tables
    QStringList table_schemes = KalziumTableTypeFactory::instance()->tables();
    table_action =  actionCollection()->add<KSelectAction>( "view_table" );
    table_action->setText( i18n( "&Tables" ) );
    table_action->setItems(table_schemes);
    table_action->setCurrentItem(Prefs::table());
    connect( table_action, SIGNAL( triggered( int ) ), this, SLOT( slotSwitchtoTable( int ) ) );

    // the actions for switching numeration
    numeration_action = actionCollection()->add<KSelectAction>( "view_numerationtype" );
    numeration_action->setText( i18n( "&Numeration" ) );
    numeration_action->setItems( KalziumNumerationTypeFactory::instance()->numerations() );
    numeration_action->setCurrentItem(Prefs::numeration());
    connect( numeration_action, SIGNAL( triggered( int ) ), this, SLOT( slotSwitchtoNumeration( int ) ) );

    m_EQSolverAction =  actionCollection()->addAction( "tools_eqsolver" );
    m_EQSolverAction->setText( i18n( "&Equation Solver..." ) );
    m_EQSolverAction->setIcon(  KIcon( "eqchem" ) );
    m_EQSolverAction->setWhatsThis( i18nc( "WhatsThis Help", "This tool allows you to solve chemical equations." ) );

#ifdef HAVE_FACILE
    connect( m_EQSolverAction, SIGNAL( triggered() ), this, SLOT( slotShowEQSolver() ) );
    m_EQSolverAction->setEnabled( true );
#else
    m_EQSolverAction->setEnabled( false );
#endif

    // tools actions
    m_pPlotAction = actionCollection()->addAction( "tools_plotdata" );
    m_pPlotAction->setText( i18n( "&Plot Data..." ) );
    m_pPlotAction->setIcon( KIcon( "plot" ) );
    connect( m_pPlotAction, SIGNAL( triggered() ), this, SLOT( slotPlotData() ) );

    // calculator actions
    m_pcalculator = actionCollection()->addAction( "tools_calculate" );
    m_pcalculator->setText( i18n( "Perform &Calculations..." ) );
    m_pcalculator->setIcon( KIcon( "calculate" ) );
    m_pcalculator->setWhatsThis( i18nc( "WhatsThis Help", "This is the calculator, it performs basic chemical calculations." ) );
    connect( m_pcalculator, SIGNAL( triggered() ), this, SLOT( showCalculator() ) );

    m_pIsotopeTableAction= actionCollection()->addAction( "tools_isotopetable" );
    m_pIsotopeTableAction->setText( i18n( "&Isotope Table..." ) );
    m_pIsotopeTableAction->setIcon(  KIcon( "isotopemap" ) );
    m_pIsotopeTableAction->setWhatsThis( i18nc( "WhatsThis Help", "This table shows all of the known isotopes of the chemical elements." ) );
    connect( m_pIsotopeTableAction, SIGNAL( triggered() ), this, SLOT( slotIsotopeTable() ) );

    m_pGlossaryAction = actionCollection()->addAction( "tools_glossary" );
    m_pGlossaryAction->setText(i18n( "&Glossary..." ) );
    m_pGlossaryAction->setIcon( KIcon( "glossary" ) );
    connect( m_pGlossaryAction, SIGNAL( triggered() ), this, SLOT( slotGlossary() ) );

    m_pRSAction = actionCollection()->addAction( "tools_rs" );
    m_pRSAction->setText( i18n( "&R/S Phrases..." ) );
    m_pRSAction->setIcon( KIcon( "kalzium_rs" ) );
    connect( m_pRSAction, SIGNAL( triggered() ), this, SLOT( slotRS() ) );

    m_pOBConverterAction = actionCollection()->addAction( "tools_obconverter" );
    m_pOBConverterAction->setText( i18n( "Convert chemical files..." ) );
    m_pOBConverterAction->setIcon( KIcon( "edit-copy" ) );
    m_pOBConverterAction->setWhatsThis( i18nc( "WhatsThis Help", "With this tool, you can convert files containing chemical data between various file formats." ) );
    connect( m_pOBConverterAction, SIGNAL( triggered() ), this, SLOT( slotOBConverter() ) );
#ifndef HAVE_OPENBABEL2
    m_pOBConverterAction->setEnabled( false );
#endif

    m_pMoleculesviewer = actionCollection()->addAction( "tools_moleculeviewer" );
    m_pMoleculesviewer->setText( i18n( "Molecular Editor..." ) );
    m_pMoleculesviewer->setIcon( KIcon( "kalzium_molviewer" ) );
    m_pMoleculesviewer->setWhatsThis( i18nc( "WhatsThis Help", "This tool allows you to view and edit 3D molecular structures." ) );
    connect( m_pMoleculesviewer, SIGNAL( triggered() ), this, SLOT( slotMoleculeviewer() ) );
#if !defined(HAVE_OPENBABEL2) || !defined(HAVE_EIGEN) || !defined(HAVE_AVOGADRO)
    m_pMoleculesviewer->setEnabled( false );
#endif

    m_pTables = actionCollection()->addAction( "tools_tables" );
    m_pTables->setText( i18n( "&Tables..." ) );
    m_pTables->setIcon( KIcon( "kalzium_tables" ) );
    m_pTables->setWhatsThis(i18nc("WhatsThis Help", "This will open a dialog with listings of symbols and numbers related to chemistry."));

    connect( m_pTables, SIGNAL( triggered() ), this, SLOT( slotTables() ) );

    // other period view options
    m_pLegendAction = m_InfoDock->toggleViewAction();
    actionCollection()->addAction( "view_legend", m_pLegendAction );
    m_pLegendAction->setIcon( KIcon( "legend" ) );
    m_pLegendAction->setWhatsThis( i18nc( "WhatsThis Help", "This will show or hide the legend for the periodic table." ) );
    connect( m_pLegendAction, SIGNAL( triggered(bool) ),
             this, SLOT( slotShowLegend(bool) ) );

    m_SidebarAction = m_dockWin->toggleViewAction();
    actionCollection()->addAction( "view_sidebar", m_SidebarAction );
    m_SidebarAction->setIcon( KIcon( "sidebar" ) );
    m_SidebarAction->setWhatsThis( i18nc( "WhatsThis Help", "This will show or hide a sidebar with additional information and a set of tools." ) );
    connect( m_SidebarAction, SIGNAL( triggered( bool ) ),
             this, SLOT( slotShowHideSidebar( bool ) ) );

    // the standard actions
    actionCollection()->addAction( "saveAs", KStandardAction::saveAs(this, SLOT(slotExportTable()), actionCollection()));
    
    KStandardAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
    
    actionCollection()->addAction( "quit", KStandardAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() ));

    m_legendWidget->LockWidget();

// //     if (Prefs::schemaSelected())
//     {
//         slotSwitchtoLookGradient( Prefs::colorgradientbox() );
//         slotSwitchtoLookScheme( Prefs::colorschemebox() );
//     }
//     else
//     {
        slotSwitchtoLookScheme( Prefs::colorschemebox() );
        slotSwitchtoLookGradient( Prefs::colorgradientbox() );
//     }

    slotSwitchtoNumeration( Prefs::numeration() );
    slotSwitchtoTable( Prefs::table() );
    slotShowHideSidebar( m_SidebarAction->isChecked(), false );
    slotShowLegend( m_pLegendAction->isChecked(), false );

    m_legendWidget->UnLockWidget();
    m_legendWidget->updateContent();

    // set the shell's ui resource file
    setXMLFile("kalziumui.rc");
    setupGUI();
}

void Kalzium::setupSidebars()
{
    m_InfoDock = new QDockWidget( i18n("Legend"), this );
    m_InfoDock->setObjectName( QLatin1String( "kalzium-infobar" ) );
    m_InfoDock->setFeatures( QDockWidget::AllDockWidgetFeatures );

    m_InfoDock->setWidget(m_legendWidget);

    m_infoTabWidget = new KTabWidget(this);
    m_infoTabWidget->setObjectName( "kalzium-infobar" );
    m_InfoDock->setWidget(m_infoTabWidget);
    m_infoTabWidget->addTab(m_legendWidget, i18n("Legend"));
    m_infoTabWidget->addTab(m_TableInfoWidget, i18n("Table Information"));

    m_dockWin = new QDockWidget(i18n("Information"), this );
    m_dockWin->setObjectName( QLatin1String( "kalzium-sidebar" ) );
    m_dockWin->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

    m_toolbox = new QToolBox( m_dockWin );
    m_dockWin->setWidget( m_toolbox );

    QWidget *fake = new QWidget( m_toolbox );
    QVBoxLayout *lay = new QVBoxLayout( fake );
    lay->setMargin( 5 );
    lay->activate();
    m_detailWidget = new DetailedGraphicalOverview( fake );
    m_detailWidget->setObjectName( "DetailedGraphicalOverview" );
    m_detailWidget->setMinimumSize( 200, m_detailWidget->minimumSize().height() );
    connect( m_PeriodicTableView, SIGNAL( MouseOver( int ) ),
             m_detailWidget, SLOT( setElement( int ) ) );
    lay->addWidget( m_detailWidget );
    lay->addItem( new QSpacerItem( 10, 10, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ) );
    m_toolbox->addItem( fake, KIcon( "overview" ), i18n( "Overview" ) );

    m_somWidget = new SOMWidgetIMPL( m_toolbox );
    m_somWidget->setObjectName( "somWidget" );
    connect( m_somWidget, SIGNAL( temperatureChanged( int ) ),
             m_PeriodicTableView, SLOT( setTemperature( int ) ) );
    m_toolbox->addItem( m_somWidget, KIcon( "statematter" ), i18n( "State of Matter" ) );

    m_timeWidget = new TimeWidgetImpl( m_toolbox );
    m_timeWidget->setObjectName( "timeWidget" );
    connect( m_timeWidget->time_box, SIGNAL( valueChanged( int ) ),
             m_PeriodicTableView, SLOT( setTime( int ) ) );
    m_toolbox->addItem( m_timeWidget, KIcon( "timeline" ), i18n( "Timeline" ) );

    m_calcWidget = new MolcalcWidget( m_toolbox );
    m_calcWidget->setObjectName( "molcalcwidget" );
    m_calcWidget->hideExtra();
    m_toolbox->addItem( m_calcWidget, KIcon( "calculate" ), i18n( "Calculate" ) );

    connect( m_toolbox, SIGNAL( currentChanged( int ) ),
             this, SLOT( slotToolboxCurrentChanged( int ) ) );

    addDockWidget( Qt::LeftDockWidgetArea, m_dockWin );
    addDockWidget( Qt::BottomDockWidgetArea, m_InfoDock );
    
    connect (m_InfoDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), m_legendWidget, SLOT(setDockArea(Qt::DockWidgetArea)));
}

void Kalzium::slotExportTable()
{
    QString fileName = KFileDialog::getSaveFileName(QString(), i18n("*.png *.xpm *.jpg *.svg"),
                       this,
                       i18n("Save Kalzium's Table In"));

    if (fileName.endsWith(".svg")) {
        m_PeriodicTableView->generateSvg( fileName );
    } else {
        QPixmap pix = QPixmap::grabWidget( m_PeriodicTableView );
        pix.save( fileName );
    }
}

void Kalzium::slotGlossary()
{
    m_glossarydlg->show();
}

void Kalzium::slotRS()
{
    RSDialog *rs = new RSDialog( this );
    rs->setAttribute(Qt::WA_DeleteOnClose);
    rs->show();
}

void Kalzium::slotOBConverter()
{
#ifdef HAVE_OPENBABEL2
    KOpenBabel * d = new KOpenBabel(this);
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->show();
#endif
}

void Kalzium::slotMoleculeviewer()
{
#if defined(HAVE_OPENBABEL2) && defined(HAVE_EIGEN) && defined(HAVE_AVOGADRO)

    if (!QGLFormat::hasOpenGL()) {
        KMessageBox::error(0, i18n("This system does not support OpenGL."), i18n("Kalzium Error"));
        return;
    }

    MoleculeDialog * d = new MoleculeDialog( this );
    d->show();

#if 0
    KPluginLoader loader("libkalziumglpart" );
    KPluginFactory* factory = loader.factory();

    if (factory) {
        KParts::ReadOnlyPart *part = 0;
        part = static_cast<KParts::ReadOnlyPart*> ( factory->create( this, "KalziumGLPart" ) );

        part->widget()->show();
    }
#endif
#endif
}

void Kalzium::slotTables()
{
    TablesDialog *t = new TablesDialog( this );
    t->setAttribute(Qt::WA_DeleteOnClose);
    t->show();
}

void Kalzium::slotShowEQSolver()
{
#ifdef HAVE_FACILE
    EQChemDialog *dlg = new EQChemDialog( this );
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
#endif
}

void Kalzium::slotIsotopeTable()
{
    IsotopeTableDialog *ndialog = new IsotopeTableDialog( this );
    ndialog->setAttribute(Qt::WA_DeleteOnClose);
    ndialog->show();
}

void Kalzium::slotPlotData()
{
    ElementDataViewer *edw = new ElementDataViewer( this );
    edw->setAttribute(Qt::WA_DeleteOnClose);
    edw->show();
}

void Kalzium::showCalculator()
{
    calculator *Cal = new calculator ( this );
    Cal->setAttribute(Qt::WA_DeleteOnClose);
    Cal -> show();
}

void Kalzium::slotShowLegend( bool checked, bool changeconfig)
{
    if ( !checked ) {
        m_pLegendAction->setText( i18n( "Show &Legend") );
    } else {
        m_pLegendAction->setText( i18n( "Hide &Legend" ) );
    }

    if ( changeconfig ) {
        Prefs::setShowlegend( checked );
        //save the settings
        Prefs::self()->writeConfig();
    }
}

void Kalzium::slotShowHideSidebar( bool checked, bool changeconfig )
{
    if ( !checked ) {
        m_SidebarAction->setText( i18n( "Show &Sidebar" ) );
        slotToolboxCurrentChanged( 0 );
    } else {
        m_SidebarAction->setText( i18n( "Hide &Sidebar" ) );
        slotToolboxCurrentChanged( m_toolboxCurrent );
    }

    if ( changeconfig ) {
        Prefs::setShowsidebar( checked );
        //save the settings
        Prefs::self()->writeConfig();
    }
}

void Kalzium::slotSwitchtoTable( int index )
{
    m_PeriodicTableView->slotChangeTable(index);
    if ( m_infoDialog ) {
        m_infoDialog->setTableType( m_PeriodicTableView->tableType() );
    }
    Prefs::setTable(index);
    Prefs::self()->writeConfig();
}

void Kalzium::slotSwitchtoNumeration( int index )
{
    m_PeriodicTableView->setNumeration( index );
    Prefs::setNumeration(index);
    Prefs::self()->writeConfig();
}

void Kalzium::slotSwitchtoLookGradient( int which )
{
    Prefs::setSchemaSelected(false);
    kDebug() << "slotSwitchtoLookGradient Kalzium";
    Prefs::setColorgradientbox(which);
    Prefs::self()->writeConfig();
    if (which > 0)
    {
        m_PeriodicTableView->setGradient( which - 1 );
        if (m_PeriodicTableView->scheme()->name() == "Iconic" ||
                m_PeriodicTableView->scheme()->name() == "MonoColor") {

            if (m_PeriodicTableView->mode() == KalziumPainter::NORMAL ||
                    m_PeriodicTableView->mode() == KalziumPainter::NORMAL_GRADIENT) {

                m_PeriodicTableView->setMode( KalziumPainter::GRADIENT );
                m_legendWidget->setMode( KalziumPainter::GRADIENT );
            }
        } else {
            if (m_PeriodicTableView->mode() == KalziumPainter::NORMAL ||
                    m_PeriodicTableView->mode() == KalziumPainter::GRADIENT) {

                m_PeriodicTableView->setMode( KalziumPainter::NORMAL_GRADIENT );
                m_legendWidget->setMode( KalziumPainter::NORMAL_GRADIENT );
            }
        }
    } else {
        slotSwitchtoLookScheme( Prefs::colorschemebox() );
    }


    look_action_gradients->blockSignals( true );
    look_action_gradients->setCurrentItem( which );
    look_action_gradients->blockSignals( false );
}

void Kalzium::slotSwitchtoLookScheme( int which )
{
    Prefs::setSchemaSelected(true);
    kDebug() << "slotSwitchtoLookScheme Kalzium";
    m_PeriodicTableView->activateColorScheme( which );
    if (Prefs::colorgradientbox() == 0 || m_PeriodicTableView->scheme()->name() == "Iconic" ||
            m_PeriodicTableView->scheme()->name() == "MonoColor") {

        if (m_PeriodicTableView->mode() == KalziumPainter::GRADIENT ||
                m_PeriodicTableView->mode() == KalziumPainter::NORMAL_GRADIENT) {

            m_PeriodicTableView->setMode( KalziumPainter::NORMAL );
            m_legendWidget->setMode( KalziumPainter::NORMAL );
        }
    } else {
        if (m_PeriodicTableView->mode() == KalziumPainter::GRADIENT ||
                m_PeriodicTableView->mode() == KalziumPainter::NORMAL) {

            m_PeriodicTableView->setMode( KalziumPainter::NORMAL_GRADIENT );
            m_legendWidget->setMode( KalziumPainter::NORMAL_GRADIENT );
        }
    }

    look_action_schemes->blockSignals( true );
    look_action_schemes->setCurrentItem( which );
    look_action_schemes->blockSignals( false );

    Prefs::setColorschemebox(which);
    Prefs::self()->writeConfig();
}

void Kalzium::showSettingsDialog()
{
    if (KConfigDialog::showDialog("settings")) {
        return;
    }

    //KConfigDialog didn't find an instance of this dialog, so lets create it :
    KConfigDialog *dialog = new KConfigDialog(this,"settings", Prefs::self());
    connect( dialog, SIGNAL( settingsChanged( const QString &) ), this , SLOT( slotUpdateSettings() ) );
    connect( dialog, SIGNAL( settingsChanged( const QString &) ), m_somWidget, SLOT( reloadUnits() ) );
    connect( dialog, SIGNAL( settingsChanged( const QString &) ), m_legendWidget, SLOT( updateContent() ) );
    // colors page
    Ui_setupColors ui_colors;
    QWidget *w_colors = new QWidget( 0 );
    w_colors->setObjectName( "colors_page" );
    ui_colors.setupUi( w_colors );
    dialog->addPage( w_colors, i18n( "Colors" ), "preferences-desktop-color" );
    // units page
    Ui_setupUnits ui_units;
    QWidget *w_units = new QWidget( 0 );
    w_units->setObjectName( "units_page" );
    ui_units.setupUi( w_units );
    dialog->addPage( w_units, i18n( "Units" ), "system-run" );
    // misc page
    Ui_setupMisc ui_misc;
    QWidget *w_misc = new QWidget( 0 );
    w_misc->setObjectName( "miscpage" );
    ui_misc.setupUi( w_misc );
    dialog->addPage( w_misc, i18n( "Miscellaneous" ), "preferences-other" );
    // web page
    Ui_setupWeb ui_web;
    QWidget *w_web = new QWidget( 0 );
    //w_web->setobjectName( "Web_lookup" );
    ui_web.setupUi( w_web );
    dialog->addPage( w_web, i18n(" Web look up" ), "preferences-system-network");

    Ui_setupCalc ui_calc;
    QWidget *w_calc = new QWidget( 0 );
    ui_calc.setupUi( w_calc );
    dialog->addPage( w_calc, i18n("Calculator"), "accessories-calculator");

    // showing the dialog
    dialog->show();
}

void Kalzium::slotUpdateSettings()
{
}

void Kalzium::slotShowExportDialog()
{
    if (!m_exportDialog) {
        m_exportDialog = new ExportDialog( this );
    }
    m_exportDialog->show();
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
    extractIconicInformationAboutElement( num );

    Element *e = KalziumDataObject::instance()->element( num );
    statusBar()->changeItem( i18nc( "For example: \"Carbon (6), Mass: 12.0107 u\"", "%1 (%2), Mass: %3 u" ,
                                    e->dataAsString( ChemicalDataObject::name ) ,
                                    e->dataAsString( ChemicalDataObject::atomicNumber ) ,
                                    e->dataAsString( ChemicalDataObject::mass ) ) , IDS_ELEMENTINFO );

    m_detailWidget->setBackgroundColor( m_PeriodicTableView->brushForElement( num ).color() );
}

void Kalzium::extractIconicInformationAboutElement( int elementNumber )
{
    QString setname = "school";
    QString pathname = KGlobal::dirs()->findResourceDir( "appdata", "data/iconsets/" ) + "data/iconsets/";
    QString filename = pathname + setname + '/' + "iconinformation.txt";

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString infoline;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString tmp = in.readLine();
        if ( tmp.startsWith( QString::number( elementNumber ) ) )
            infoline = tmp;
    }

    QString realText = "Moin dies ist ein test!";
//X         QString realText = infoline.remove( QRegExp("\\d+ ") );
}

void Kalzium::openInformationDialog( int number )
{
    if (m_infoDialog) {
        m_infoDialog->setElement( number );
    } else {
        m_infoDialog = new DetailedInfoDlg(number, this );

        // Remove the selection when this dialog finishes or hides.
        connect(m_infoDialog, SIGNAL(hidden()),
                m_PeriodicTableView,        SLOT(unSelect()));
        connect(m_infoDialog, SIGNAL(elementChanged(int)),
                m_PeriodicTableView,        SLOT(selectElement(int)));
    }
    m_infoDialog->setOverviewBackgroundColor( m_PeriodicTableView->brushForElement( number ).color() );
    m_infoDialog->setTableType( m_PeriodicTableView->tableType() );
    m_infoDialog->show();
}

void Kalzium::slotToolboxCurrentChanged( int id )
{
    KalziumPainter::MODE cur = m_PeriodicTableView->mode();
    if ( ( id > 1 ) && ((cur == KalziumPainter::NORMAL) || (cur == KalziumPainter::GRADIENT) ||
                        (cur == KalziumPainter::NORMAL_GRADIENT)) ) {
        m_prevNormalMode = cur;
    }
    m_PeriodicTableView->setMode( m_prevNormalMode );
    m_legendWidget->setMode( m_prevNormalMode );

    //In the timeline and the calculator-mode we have to disable the searchwidget
    //because of focus-stealing and a conflict with the "hiding" of the elements
    switch ( id )
    {
    case 0: // nothing (overview)
        m_searchWidget->setEnabled( true );
        break;
    case 1: //state of matter
        m_PeriodicTableView->setTemperature( m_somWidget->temperature() );
        m_PeriodicTableView->setMode( KalziumPainter::SOM );
        m_legendWidget->setMode( KalziumPainter::SOM );
        m_searchWidget->setEnabled( true );
        break;
    case 2: // timeline
        m_PeriodicTableView->setTime( m_timeWidget->time_box->value() );
        m_PeriodicTableView->setMode( KalziumPainter::TIME );
        m_legendWidget->setMode( KalziumPainter::TIME );
        m_searchWidget->setEnabled( false );
        break;
    case 3: // molecular calculator
        m_searchWidget->setEnabled( false );
        break;
    }
    if ( m_dockWin->isVisible() )
        m_toolboxCurrent = id;
}

Kalzium::~Kalzium()
{
}

void Kalzium::keyPressEvent( QKeyEvent *e)
{
    m_searchWidget->appendSearchText( e->text() );
    m_searchWidget->giveFocus();
    e->accept();
}

void Kalzium::slotSearchElements()
{
    m_PeriodicTableView->setFullDraw();
    m_PeriodicTableView->update();
}

#include "kalzium.moc"
