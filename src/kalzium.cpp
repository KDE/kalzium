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
#include "detailinfodlg.h"
#include "pse.h"
#include "molcalcwidget.h"
#include "detailedgraphicaloverview.h"
#include "timewidget.h"
#include "somwidget_impl.h"
#include "kalziumdataobject.h"
#include "nuclideboard.h"

#include <qdockwindow.h>
#include <qlayout.h>
#include <qtoolbox.h>
#include <qslider.h>
#include <qscrollview.h>

#include <kconfigdialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kdebug.h>
#include <kaction.h>
#include <kapplication.h>
#include <kstatusbar.h>
#include <kstandarddirs.h>

#include <kdeeduglossary.h>

#define PSE_MARGIN          5
#define IDS_ELEMENTINFO     7

Kalzium::Kalzium()
    : KMainWindow( 0, "Kalzium" )
{
	pd = new privatedata( this );

	pd->kalziumData = new KalziumDataObject();

	QWidget *centralWidget = new QWidget( this, "centralWidget" );
	m_pCentralLayout = new QVBoxLayout( centralWidget, PSE_MARGIN, -1, "CentralLayout" );
	
	QScrollView *helperSV = new QScrollView(centralWidget);
	m_pCentralLayout->addWidget(helperSV);
	helperSV->viewport()->setPaletteBackgroundColor(paletteBackgroundColor());  
	helperSV->setFrameShape(QFrame::NoFrame);

	m_PSE = new PSE( data(), helperSV->viewport(), "PSE");
	helperSV->addChild( m_PSE );
	m_infoDialog = 0;
	m_toolboxCurrent = 0;

	connect( m_PSE, SIGNAL( ElementClicked( int ) ), this, SLOT( openInformationDialog( int ) ));
	connect( m_PSE, SIGNAL( MouseOver( int ) ), this, SLOT( slotStatusbar( int ) ));
	connect( this, SIGNAL( tableLocked( bool ) ), m_PSE, SLOT( slotLock(bool ) ));
	
	// Layouting

	setCentralWidget( centralWidget );
	centralWidget->show();

	setupSidebars();
	setupActions();

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
	g = Glossary::readFromXML( u );
	g->setName( i18n( "Tools" ) );
	g->setBackgroundPicture( picturepath );
	m_glossarydlg->addGlossary( g );
	connect( m_glossarydlg, SIGNAL( closed() ), m_PSE, SLOT(slotUnlock()) );

	setupStatusBar();
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
	gradientlist.append(i18n("Boiling Point"));
	gradientlist.append(i18n("Melting Point"));
	gradientlist.append(i18n("Electronegativity"));
	gradientlist.append(i18n("Electron Affinity"));
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
	m_pPlotAction = new KAction(i18n("&Plot Data..."), "plot", 0, this, SLOT(slotPlotData()), actionCollection(), "plotdata");
	
	m_pGlossaryAction = new KAction(i18n("&Glossary..."), "glossary", 0, this, SLOT(slotGlossary()), actionCollection(), "glossary");

	//Legend
	m_pLegendAction = new KAction(i18n("Show &Legend"), "legend", 0, this, SLOT(slotShowLegend()), actionCollection(), "toggle_legend");
	
	m_pTooltipAction = new KAction(i18n("Show &Tooltip"), "tooltip", 0, this, SLOT(slotEnableTooltips()), actionCollection(), "toggle_tooltip");
	
	//the standardactions
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
		m_PSE->showLegend(true);
		m_pLegendAction->setText( i18n( "Hide &Legend" ) );
	} else
	{
		m_PSE->showLegend(false);
		m_pLegendAction->setText( i18n( "Show &Legend" ) );
	}

	if ( Prefs::tooltip() ) {
		m_PSE->setTooltipsEnabled( true );
		m_pTooltipAction->setText( i18n( "Hide &Tooltips" ) );
	} else
	{
		m_PSE->setTooltipsEnabled( false );
		m_pTooltipAction->setText( i18n( "Show &Tooltips" ) );
	}

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	setupGUI();
	
	KDialog *dlg = new KDialog( this, "dlg" );
	QVBoxLayout *vbox = new QVBoxLayout( dlg );
	NuclideBoard *b = new NuclideBoard( data()->ElementList, dlg, "nb" );
	vbox->addWidget( b );
	b->show();
	dlg->show();
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
	connect( m_PSE, SIGNAL( MouseOver( int ) ), this, SLOT( slotSelectedNumber( int ) ));
 	lay->addWidget( m_detailWidget );
	lay->addItem( new QSpacerItem( 10, 10, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ) );
	m_toolbox->addItem( fake, SmallIcon( "overview" ), i18n( "Overview" ) );
	
	m_calcWidget = new MolcalcWidget( data(), m_dockWin, "molcalcwidget" );
	m_toolbox->addItem( m_calcWidget, SmallIcon( "calculate" ), i18n( "Calculate" ) );

	m_timeWidget = new timeWidget( this, "timeWidget" );
	connect( m_timeWidget->time_slider, SIGNAL( valueChanged( int ) ), 
			m_PSE, 						SLOT( setDate( int ) ) );
	m_toolbox->addItem( m_timeWidget, SmallIcon( "timeline" ), i18n( "Timeline" ) );

	m_somWidget = new SOMWidgetIMPL( data()->ElementList, this, "somWidget" );
	connect( m_somWidget->temp_slider, SIGNAL( valueChanged( int ) ), 
			m_PSE, 						SLOT( setTemperature( int ) ) );
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
	emit tableLocked(true);
	m_glossarydlg->show();
}

void Kalzium::slotPlotData()
{
	ElementDataViewer *edw = new ElementDataViewer( data(), this, "edw" );
	edw->show();
}

void Kalzium::slotEnableTooltips()
{
	bool enabled = m_PSE->tooltipsEnabled();
	enabled = !enabled;

	if ( enabled )
		m_pTooltipAction->setText( i18n( "Hide &Tooltips" ) );
	else
		m_pTooltipAction->setText( i18n( "Show &Tooltips" ) );

	m_PSE->setTooltipsEnabled( enabled );
	
	Prefs::setTooltip( enabled ); 
	Prefs::writeConfig();
}

void Kalzium::slotShowLegend()
{
	if(m_PSE->showLegend())
	{
		m_PSE->showLegend(false);
		m_pLegendAction->setText( i18n( "Show &Legend" ) );
	}
	else
	{
		m_PSE->showLegend(true);
		m_pLegendAction->setText( i18n( "Hide &Legend" ) );
	}
	m_PSE->update();
	
	//save the settings
	Prefs::setShowlegend( m_PSE->showLegend() ); 
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
			gradient_action->setCurrentItem( Prefs::gradient()-1 );
			m_PSE->setLook( PSE::GRADIENT, Prefs::gradient() );
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
	m_actionCrystal->setChecked( false );
	m_pLegendAction->setEnabled( false );

	Prefs::setGradient(index + 1);
	Prefs::writeConfig();
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
	m_PSE->setFullDraw();
    slotEnableTooltips();
}
 
void Kalzium::setupStatusBar()
{
	statusBar()->insertItem(  "" , IDS_ELEMENTINFO, 1, false );
	statusBar()->setItemAlignment( IDS_ELEMENTINFO, AlignRight );
	statusBar()->show();
}

void Kalzium::slotStatusbar( int num )
{
	Element *e = data()->element( num );
	statusBar()->changeItem( i18n( "For example: \"Carbon (6), Mass: 12.0107 u\"", "%1 (%2), Mass: %3 u" ).arg( e->elname() ).arg(e->number() ).arg( e->mass() ) , IDS_ELEMENTINFO );
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
	m_PSE->setTimeline( false );
	m_PSE->activateSOMMode( false );

	disconnect( m_PSE, SIGNAL( ElementClicked( int ) ), m_calcWidget, SLOT( slotButtonClicked( int ) ) );
	switch ( id )
	{
		case 0: // nothing
			emit tableLocked( false );
//			m_calcWidget->clear();
			break;
		case 1: // molcalc
			connect( m_PSE, SIGNAL( ElementClicked( int ) ), m_calcWidget, SLOT( slotButtonClicked( int ) ) );
			emit tableLocked( true );
			break;
		case 2: // timeline
			m_PSE->setTimeline( true );
			m_PSE->setDate( m_timeWidget->time_slider->value() );
			break;
		case 3: // state of matter
			m_PSE->activateSOMMode( true );
			m_PSE->setTemperature( m_somWidget->temp_slider->value() );
			break;
	}
	if ( m_dockWin->isShown() )
		m_toolboxCurrent = id;
}

void Kalzium::slotSelectedNumber( int num )
{
	m_detailWidget->setElement( data()->element( num ) );
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

KalziumDataObject* Kalzium::data() const
{
	return pd->kalziumData;
}

Kalzium::~Kalzium()
{
}

#include "kalzium.moc"
