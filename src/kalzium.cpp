/***************************************************************************
    copyright            : (C) 2003-2007 by Carsten Niehaus
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

#include <element.h>
#include <kdeeduglossary.h>
#include "prefs.h"
#include "ui_settings_colors.h"
#include "ui_settings_gradients.h"
#include "ui_settings_calc.h"
#include "detailinfodlg.h"
#include "detailedgraphicaloverview.h"
#include "gradientwidget_impl.h"
#include "kalziumdataobject.h"
#include "kalziumnumerationtype.h"
#include "kalziumschemetype.h"
#include "kalziumgradienttype.h"
#include "legendwidget.h"
#include "exportdialog.h"
#include "search.h"
#include "searchwidget.h"
#include "tableinfowidget.h"
#include "psetables.h"
#include <config-kalzium.h>

#ifdef HAVE_FACILE
#include "eqchemview.h"
#endif

#ifdef HAVE_OPENBABEL2
#if defined(HAVE_EIGEN) && defined(HAVE_AVOGADRO)
#include "tools/moleculeview.h"
#include <QGLFormat>
#endif
#include "tools/obconverter.h"
#endif

#include <QDockWidget>
#include <QFileDialog>
#include <QGridLayout>
#include <QIcon>
#include <QKeyEvent>
#include <QLocale>
#include <QMessageBox>
#include <QRegExp>
#include <QStandardPaths>
#include <QStatusBar>
#include <QToolBox>
#include <QUrl>

#include <KActionCollection>
#include <KConfigDialog>
#include <KLocalizedString>
#include <KParts/Part>
#include <KPluginLoader>
#include <KSelectAction>
#include <KServiceTypeTrader>
#include <KStandardAction>

#define IDS_ELEMENTINFO     7

Kalzium::Kalzium() : KXmlGuiWindow(nullptr)
{
    setObjectName(QStringLiteral("KalziumMainWindow"));

    // Init pointers with null
    m_infoDialog = nullptr;
    m_isotopeDialog = nullptr;
    m_elementDataPlotter = nullptr;
    m_tablesDialog = nullptr;
    m_rsDialog = nullptr;
    m_calculator = nullptr;
    m_exportDialog = nullptr;
    m_glossarydlg = nullptr;
    m_elementInfo = nullptr;

    // reading the elements from file
    KalziumDataObject::instance();

    Search *newsearch = new Search();
    KalziumDataObject::instance()->setSearch(newsearch);

    // Main pse-Table Tablewidget
    QWidget *pseTempWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(pseTempWidget);
    layout->setMargin(0);
    layout->setSpacing(2);

    SearchWidget *searchWidget = new SearchWidget(pseTempWidget);
    searchWidget->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum));

    // Creating the periodic table
    m_periodicTable = new PeriodicTableView(pseTempWidget);

    // Connecting the search to the periodic table
    connect(newsearch, &Search::searchChanged,
             KalziumElementProperty::instance(), &KalziumElementProperty::propertyChanged);
    connect(newsearch, &Search::searchReset,
             KalziumElementProperty::instance(), &KalziumElementProperty::propertyChanged);

    layout->addWidget(searchWidget);
    layout->addWidget(m_periodicTable);

    setCentralWidget(pseTempWidget);

    connect(m_periodicTable->pseScene(), &PeriodicTableScene::elementChanged, this, &Kalzium::openInformationDialog);
    connect(m_periodicTable->pseScene(), &PeriodicTableScene::elementHovered, this, &Kalzium::elementHover);
    connect(this, &Kalzium::numerationChanged,  m_periodicTable, &PeriodicTableView::numerationChange);

    // layouting
    setupSidebars();
    setupActions();

    setupStatusBar();
}

void Kalzium::setupActions()
{
    export_action = actionCollection()->add<QAction>(QStringLiteral("file_exporter"));
    export_action->setText(i18n("&Export Data..."));
    connect(export_action, &QAction::triggered, this, &Kalzium::slotShowExportDialog);

    // the action for switching look: color schemes and gradients
    QStringList schemes = KalziumElementProperty::instance()->schemeList();    /*KalziumSchemeTypeFactory::instance()->schemes();*/
    QStringList gradients = KalziumElementProperty::instance()->gradientList();

    // the action for switching look: schemes
    look_action_schemes = actionCollection()->add<KSelectAction>(QStringLiteral("view_look_onlyschemes"));
    look_action_schemes->setText(i18n("&Scheme"));
    look_action_schemes->setItems(schemes);
    look_action_schemes->setToolBarMode(KSelectAction::MenuMode);
    look_action_schemes->setToolButtonPopupMode(QToolButton::InstantPopup);
    connect(look_action_schemes, SIGNAL(triggered(int)), this, SLOT(slotSwitchtoLookScheme(int)));

    // the action for switching look: gradients
    look_action_gradients = actionCollection()->add<KSelectAction>(QStringLiteral("view_look_onlygradients"));
    look_action_gradients->setText(i18n("&Gradients"));
    look_action_gradients->setItems(gradients);
    look_action_gradients->setToolBarMode(KSelectAction::MenuMode);
    look_action_gradients->setToolButtonPopupMode(QToolButton::InstantPopup);
    connect(look_action_gradients, SIGNAL(triggered(int)), this, SLOT(slotSwitchtoLookGradient(int)));

    // the action for switching tables
    QStringList table_schemes = pseTables::instance()->tables();
    table_action =  actionCollection()->add<KSelectAction>(QStringLiteral("view_table"));
    table_action->setText(i18n("&Tables"));
    table_action->setItems(table_schemes);
    table_action->setCurrentItem(Prefs::table());
    connect(table_action, SIGNAL(triggered(int)), this, SLOT(slotSwitchtoTable(int)));

    // the actions for switching numeration
    numeration_action = actionCollection()->add<KSelectAction>(QStringLiteral("view_numerationtype"));
    numeration_action->setText(i18n("&Numeration"));
    numeration_action->setItems(KalziumNumerationTypeFactory::instance()->numerations());
    numeration_action->setCurrentItem(Prefs::numeration());
    connect(numeration_action, SIGNAL(triggered(int)), this, SLOT(slotSwitchtoNumeration(int)));

    // tools actions
    m_pPlotAction = actionCollection()->addAction(QStringLiteral("tools_plotdata"));
    m_pPlotAction->setText(i18n("&Plot Data..."));
    m_pPlotAction->setIcon(QIcon::fromTheme(QStringLiteral("plot")));
    connect(m_pPlotAction, &QAction::triggered, this, &Kalzium::slotPlotData);

    // calculator actions
    m_pcalculator = actionCollection()->addAction(QStringLiteral("tools_calculate"));
    m_pcalculator->setText(i18n("Perform &Calculations..."));
    m_pcalculator->setIcon(QIcon::fromTheme(QStringLiteral("calculate")));
    m_pcalculator->setWhatsThis(i18nc("WhatsThis Help", "This is the calculator, it performs basic chemical calculations."));
    connect(m_pcalculator, &QAction::triggered, this, &Kalzium::showCalculator);

    m_pIsotopeTableAction= actionCollection()->addAction(QStringLiteral("tools_isotopetable"));
    m_pIsotopeTableAction->setText(i18n("&Isotope Table..."));
    m_pIsotopeTableAction->setIcon(QIcon::fromTheme(QStringLiteral("isotopemap")));
    m_pIsotopeTableAction->setWhatsThis(i18nc("WhatsThis Help", "This table shows all of the known isotopes of the chemical elements."));
    connect(m_pIsotopeTableAction, &QAction::triggered, this, &Kalzium::slotIsotopeTable);

    m_pGlossaryAction = actionCollection()->addAction(QStringLiteral("tools_glossary"));
    m_pGlossaryAction->setText(i18n("&Glossary..."));
    m_pGlossaryAction->setIcon(QIcon::fromTheme(QStringLiteral("glossary")));
    connect(m_pGlossaryAction, &QAction::triggered, this, &Kalzium::slotGlossary);

    m_pRSAction = actionCollection()->addAction(QStringLiteral("tools_rs"));
    m_pRSAction->setText(i18n("&R/S Phrases..."));
    m_pRSAction->setIcon(QIcon::fromTheme(QStringLiteral("kalzium_rs")));
    connect(m_pRSAction, &QAction::triggered, this, &Kalzium::slotRS);

    m_pOBConverterAction = actionCollection()->addAction(QStringLiteral("tools_obconverter"));
    m_pOBConverterAction->setText(i18n("Convert chemical files..."));
    m_pOBConverterAction->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));
    m_pOBConverterAction->setWhatsThis(i18nc("WhatsThis Help", "With this tool, you can convert files containing chemical data between various file formats."));
    connect(m_pOBConverterAction, &QAction::triggered, this, &Kalzium::slotOBConverter);
#ifndef HAVE_OPENBABEL2
    m_pOBConverterAction->setEnabled(false);
#endif

    m_pMoleculesviewer = actionCollection()->addAction(QStringLiteral("tools_moleculeviewer"));
    m_pMoleculesviewer->setText(i18n("Molecular Editor..."));
    m_pMoleculesviewer->setIcon(QIcon::fromTheme(QStringLiteral("kalzium_molviewer")));
    m_pMoleculesviewer->setWhatsThis(i18nc("WhatsThis Help", "This tool allows you to view and edit 3D molecular structures."));
    connect(m_pMoleculesviewer, &QAction::triggered, this, &Kalzium::slotMoleculeviewer);
#if !defined(HAVE_OPENBABEL2) || !defined(HAVE_EIGEN) || !defined(HAVE_AVOGADRO)
    m_pMoleculesviewer->setEnabled(false);
#endif

    m_pTables = actionCollection()->addAction(QStringLiteral("tools_tables"));
    m_pTables->setText(i18n("&Tables..."));
    m_pTables->setIcon(QIcon::fromTheme(QStringLiteral("kalzium_tables")));
    m_pTables->setWhatsThis(i18nc("WhatsThis Help", "This will open a dialog with listings of symbols and numbers related to chemistry."));

    connect(m_pTables, &QAction::triggered, this, &Kalzium::slotTables);

    // other period view options
    m_pLegendAction = m_legendDock->toggleViewAction();
    actionCollection()->addAction(QStringLiteral("view_legend"), m_pLegendAction);
    m_pLegendAction->setIcon(QIcon::fromTheme(QStringLiteral("legend")));
    m_pLegendAction->setWhatsThis(i18nc("WhatsThis Help", "This will show or hide the legend for the periodic table."));

    m_SidebarAction = m_dockWin->toggleViewAction();
    actionCollection()->addAction(QStringLiteral("view_sidebar"), m_SidebarAction);
    m_SidebarAction->setIcon(QIcon::fromTheme(QStringLiteral("sidebar")));
    m_SidebarAction->setWhatsThis(i18nc("WhatsThis Help", "This will show or hide a sidebar with additional information and a set of tools."));

    m_SidebarAction = m_tableDock->toggleViewAction();
    actionCollection()->addAction(QStringLiteral("view_tablebar"), m_SidebarAction);
    m_SidebarAction->setIcon(QIcon::fromTheme(QStringLiteral("kalzium_tables")));
    m_SidebarAction->setWhatsThis(i18nc("WhatsThis Help", "This will show or hide a sidebar with additional information about the table."));

    // the standard actions
    actionCollection()->addAction(QStringLiteral("saveAs"), KStandardAction::saveAs(this, SLOT(slotExportTable()), actionCollection()));

    KStandardAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());

    actionCollection()->addAction(QStringLiteral("quit"), KStandardAction::quit(qApp, SLOT(quit()), actionCollection()));

    m_legendWidget->LockWidget();


    slotSwitchtoLookGradient(KalziumElementProperty::instance()->gradientId());
    slotSwitchtoLookScheme(KalziumElementProperty::instance()->schemeId());

    slotSwitchtoNumeration(Prefs::numeration());
    slotSwitchtoTable(Prefs::table());

    m_legendWidget->UnLockWidget();
    m_legendWidget->updateContent();

    // set the shell's ui resource file
    setXMLFile(QStringLiteral("kalziumui.rc"));
    setupGUI();
}

void Kalzium::setupSidebars()
{
    setDockNestingEnabled(true);

    m_legendWidget = new LegendWidget(this);
    m_legendDock = new QDockWidget(i18n("Legend"), this);
    m_legendDock->setObjectName(QStringLiteral("kalzium-legend"));
    m_legendDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    m_legendDock->setWidget(m_legendWidget);

    connect(m_legendDock, &QDockWidget::dockLocationChanged,
            m_legendWidget, &LegendWidget::setDockArea);
    connect(m_legendWidget, &LegendWidget::elementMatched,
            m_periodicTable, &PeriodicTableView::slotSelectAdditionalElement);
    connect(m_legendWidget, &LegendWidget::resetElementMatch,
            m_periodicTable, &PeriodicTableView::slotUnSelectElements);

    m_TableInfoWidget = new TableInfoWidget(this);
    m_tableDock = new QDockWidget(i18n("Table Information"), this);
    m_tableDock->setWidget(m_TableInfoWidget);
    m_tableDock->setObjectName(QStringLiteral("kalzium-tableinfo"));
    m_tableDock->setFeatures(QDockWidget::AllDockWidgetFeatures);


    m_dockWin = new QDockWidget(i18n("Information"), this);
    m_dockWin->setObjectName(QStringLiteral("kalzium-sidebar"));
    m_dockWin->setFeatures(QDockWidget::AllDockWidgetFeatures);
    m_dockWin->setMinimumWidth(250);

    m_toolbox = new QToolBox(m_dockWin);
    m_dockWin->setWidget(m_toolbox);

    m_detailWidget = new DetailedGraphicalOverview(m_toolbox);
    m_detailWidget->setObjectName(QStringLiteral("DetailedGraphicalOverview"));
    m_detailWidget->setMinimumSize(200, m_detailWidget->minimumSize().height());

    m_toolbox->addItem(m_detailWidget, QIcon::fromTheme(QStringLiteral("overview")), i18n("Overview"));

    m_gradientWidget = new GradientWidgetImpl(m_toolbox);
    m_gradientWidget->setObjectName(QStringLiteral("viewtWidget"));

    connect(m_gradientWidget, &GradientWidgetImpl::gradientValueChanged,
             KalziumElementProperty::instance(), &KalziumElementProperty::setSliderValue);
    connect(m_gradientWidget->scheme_combo, SIGNAL(currentIndexChanged(int)),
             this, SLOT(slotSwitchtoLookScheme(int)));
    connect(m_gradientWidget->gradient_combo, SIGNAL(currentIndexChanged(int)),
             this, SLOT(slotSwitchtoLookGradient(int)));

    m_toolbox->addItem(m_gradientWidget, QIcon::fromTheme(QStringLiteral("statematter")), i18n("View"));

    addDockWidget(Qt::LeftDockWidgetArea, m_dockWin);
    addDockWidget(Qt::BottomDockWidgetArea, m_tableDock, Qt::Horizontal);
    addDockWidget(Qt::BottomDockWidgetArea, m_legendDock, Qt::Horizontal);

    m_tableDock->setVisible(false);
}

void Kalzium::slotExportTable()
{
    QString fileName = QFileDialog::getSaveFileName(this, i18n("Save Kalzium's Table In"), QString(), i18n("Image files (*.png *.xpm *.jpg *.svg)"));

    if (fileName.endsWith(QLatin1String(".svg"))) {
        m_periodicTable->generateSvg(fileName);
    } else {
        QPixmap pix = QPixmap::grabWidget(m_periodicTable);
        pix.save(fileName);
    }
}

void Kalzium::slotGlossary()
{
    if (!m_glossarydlg) {
      // creating the glossary dialog and loading the glossaries we have
      m_glossarydlg = new GlossaryDialog(this);
      m_glossarydlg->setObjectName(QStringLiteral("glossary"));
      QString dir = QStandardPaths::locate(QStandardPaths::DataLocation, QStringLiteral("data/"), QStandardPaths::LocateDirectory);
      dir = QFileInfo(dir).absolutePath();
      QString picturepath = dir + "/bg.jpg";
      QUrl u = QUrl::fromLocalFile(dir + "/knowledge.xml");
      Glossary *g = new Glossary(u);
      g->setName(i18n("Knowledge"));
      g->setBackgroundPicture(picturepath);
      m_glossarydlg->addGlossary(g, true);
      u = QUrl::fromLocalFile(dir + "/tools.xml");
      g = new Glossary(u, dir + "/toolpics/");
      g->setName(i18n("Tools"));
      g->setBackgroundPicture(picturepath);
      m_glossarydlg->addGlossary(g, true);
    }

    m_glossarydlg->show();
}

void Kalzium::slotRS()
{
    if (!m_rsDialog) {
      m_rsDialog = new RSDialog(this);
    }
    m_rsDialog->show();
}

void Kalzium::slotOBConverter()
{
#ifdef HAVE_OPENBABEL2
    KOpenBabel * d = new KOpenBabel(this);
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->show();
#endif
}

MoleculeDialog *Kalzium::slotMoleculeviewer()
{
#if defined(HAVE_OPENBABEL2) && defined(HAVE_EIGEN) && defined(HAVE_AVOGADRO)

    if (!QGLFormat::hasOpenGL()) {
        QMessageBox::critical(Q_NULLPTR, i18n("Kalzium Error"), i18n("This system does not support OpenGL."));
        return nullptr;
    }

    MoleculeDialog * d = new MoleculeDialog(this);
    d->show();
    return d;

#if 0
    KPluginLoader loader("libkalziumglpart");
    KPluginFactory* factory = loader.factory();

    if (factory) {
        KParts::ReadOnlyPart *part = 0;
        part = static_cast<KParts::ReadOnlyPart*>(factory->create(this, "KalziumGLPart"));

        part->widget()->show();
    }
#endif
#endif
    return nullptr;
}

void Kalzium::slotTables()
{
    if (!m_tablesDialog) {
      m_tablesDialog = new TablesDialog(this);
    }
    m_tablesDialog->show();
}

void Kalzium::slotIsotopeTable()
{
   if (!m_isotopeDialog) {
     m_isotopeDialog = new IsotopeTableDialog(this);
   }
   m_isotopeDialog->show();
}

void Kalzium::slotPlotData()
{
    if (!m_elementDataPlotter) {
      m_elementDataPlotter = new ElementDataViewer(this);
    }
    m_elementDataPlotter->show();
}

void Kalzium::showCalculator()
{
    if (!m_calculator) {
      m_calculator = new calculator(this);
    }
    m_calculator -> show();
}

void Kalzium::slotSwitchtoTable(int index)
{
    m_periodicTable->slotChangeTable(index);
    m_TableInfoWidget->setTableType(index);

    if (m_infoDialog) {
        m_infoDialog->setTableType(m_periodicTable->table());
    }
    Prefs::setTable(index);
    Prefs::self()->save();
}

void Kalzium::slotSwitchtoNumeration(int index)
{
    emit numerationChanged(index);
    Prefs::setNumeration(index);
    Prefs::self()->save();
}

void Kalzium::slotSwitchtoLookGradient(int which)
{
    qDebug() << "slotSwitchtoLookGradient Kalzium";

    KalziumElementProperty::instance()->setGradient(which);

    look_action_gradients->blockSignals(true);
    m_gradientWidget->gradient_combo->blockSignals(true);

    look_action_gradients->setCurrentItem(which);
    m_gradientWidget->gradient_combo->setCurrentIndex(which);

    look_action_gradients->blockSignals(false);
    m_gradientWidget->gradient_combo->blockSignals(false);

    m_gradientWidget->slotGradientChanged();

    m_legendWidget->updateContent();
}

void Kalzium::slotSwitchtoLookScheme(int which)
{
    qDebug() << "slotSwitchtoLookScheme Kalzium";

    KalziumElementProperty::instance()->setScheme(which);

    m_gradientWidget->scheme_combo->blockSignals(true);
    look_action_schemes->blockSignals(true);

    look_action_schemes->setCurrentItem(which);
    m_gradientWidget->scheme_combo->setCurrentIndex(which);

    look_action_schemes->blockSignals(false);
    m_gradientWidget->scheme_combo->blockSignals(false);

    m_legendWidget->updateContent();
}

void Kalzium::showSettingsDialog()
{
    if (KConfigDialog::showDialog(QStringLiteral("settings"))) {
        return;
    }

    //KConfigDialog didn't find an instance of this dialog, so lets create it :
    KConfigDialog *dialog = new KConfigDialog(this,QStringLiteral("settings"), Prefs::self());

    // colors page
    Ui_setupColors ui_colors;
    QWidget *w_colors = new QWidget(this);
    w_colors->setObjectName(QStringLiteral("colors_page"));
    ui_colors.setupUi(w_colors);
    dialog->addPage(w_colors, i18n("Schemes"), QStringLiteral("preferences-desktop-color"));

    // gradients page
    Ui_setupGradients ui_gradients;
    QWidget *w_gradients = new QWidget(this);
    w_gradients->setObjectName(QStringLiteral("gradients_page"));
    ui_gradients.setupUi(w_gradients);
    dialog->addPage(w_gradients, i18n("Gradients"), QStringLiteral("preferences-desktop-color"));

    // units page
    m_unitsDialog = new UnitSettingsDialog(this);
    m_unitsDialog->setObjectName(QStringLiteral("units_page"));
    dialog->addPage(m_unitsDialog, i18n("Units"), QStringLiteral("system-run"));

    Ui_setupCalc ui_calc;
    QWidget *w_calc = new QWidget(this);
    ui_calc.setupUi(w_calc);
    dialog->addPage(w_calc, i18n("Calculator"), QStringLiteral("accessories-calculator"));

    connect(dialog, &KConfigDialog::settingsChanged, this, &Kalzium::slotUpdateSettings);
    connect(dialog, &KConfigDialog::settingsChanged, m_gradientWidget, &GradientWidgetImpl::slotGradientChanged);

    dialog->show();
}

void Kalzium::slotUpdateSettings()
{
    Prefs::setLengthUnit(m_unitsDialog->getLenghtUnitId());

    Prefs::setEnergiesUnit(m_unitsDialog->getEnergyUnitId());

    Prefs::setTemperatureUnit(m_unitsDialog->getTemperatureUnitId());

    Prefs::self()->save();

    /*This slot function calls change the color of pse elements immediately after prefs change*/
    slotSwitchtoLookGradient(Prefs::colorgradientbox());
    slotSwitchtoLookScheme(Prefs::colorschemebox());
}

void Kalzium::slotShowExportDialog()
{
    if (!m_exportDialog) {
        m_exportDialog = new ExportDialog(this);
    }
    m_exportDialog->show();
}

void Kalzium::setupStatusBar()
{
    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    m_elementInfo = new QLabel(QLatin1String(""));
    m_elementInfo->setAlignment(Qt::AlignRight);
    statusBar->addWidget(m_elementInfo, 1);
    statusBar->show();
}

void Kalzium::elementHover(int num)
{
    //     extractIconicInformationAboutElement(num);
    Element *e = KalziumDataObject::instance()->element(num);
    m_elementInfo->setText(i18nc("For example: \"Carbon (6), Mass: 12.0107 u\"", "%1 (%2), Mass: %3 u",
                                 e->dataAsString(ChemicalDataObject::name),
                                 e->dataAsString(ChemicalDataObject::atomicNumber),
                                 e->dataAsString(ChemicalDataObject::mass)));
    qDebug() << "change item in status bar";

    m_detailWidget->setElement(num);
}

// FIXME What is that function for? Does not seem to do anything useful... yet?
void Kalzium::extractIconicInformationAboutElement(int elementNumber)
{
    QString setname = QStringLiteral("school");
    QString pathname = QStandardPaths::locate(QStandardPaths::DataLocation, QStringLiteral("data/iconsets/"), QStandardPaths::LocateDirectory);
    pathname = QFileInfo(pathname).absolutePath();
    QString filename = pathname + setname + '/' + "iconinformation.txt";

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QString infoline;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString tmp = in.readLine();
        if (tmp.startsWith(QString::number(elementNumber))) {
            infoline = tmp;
        }
    }

    QString realText = QStringLiteral("Moin dies ist ein test!");
//X         QString realText = infoline.remove(QRegExp("\\d+ "));
}

void Kalzium::openInformationDialog(int number)
{
    if (m_infoDialog) {
        m_infoDialog->setElement(number);
    } else {
        m_infoDialog = new DetailedInfoDlg(number, this);

        // Remove the selection when this dialog finishes or hides.
        connect(m_infoDialog, &DetailedInfoDlg::elementChanged,
                m_periodicTable, &PeriodicTableView::slotSelectOneElement);
        connect(m_infoDialog, &DetailedInfoDlg::elementChanged,
                this, &Kalzium::elementHover);
    }

    m_infoDialog->setTableType(m_periodicTable->table());
    m_infoDialog->show();
}


Kalzium::~Kalzium()
{
    delete m_periodicTable;
    delete m_infoDialog;
    delete m_TableInfoWidget;
    delete m_legendWidget;
    delete m_gradientWidget;

    delete m_dockWin;
    delete m_legendDock;
    delete m_tableDock;
}

void Kalzium::loadMolecule(const QString &moleculeFile)
{
#if defined(HAVE_OPENBABEL2) && defined(HAVE_EIGEN) && defined(HAVE_AVOGADRO)
    MoleculeDialog *d = slotMoleculeviewer();
    if (d) {
        d->loadMolecule(moleculeFile);
    }
#endif
}

QSize Kalzium::sizeHint() const
{
    return QSize(700, 500);
}

