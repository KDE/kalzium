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
#ifndef KALZIUM_H
#define KALZIUM_H

#include <kxmlguiwindow.h>
#include "periodictableview.h"
#include "unitsettingsdialog.h"

class KTabWidget;
class KAction;
class KSelectAction;
class QToolBox;
class QVBoxLayout;
class QDockWidget;
class DetailedInfoDlg;
class PeriodicTableView;
class MolcalcWidget;
class DetailedGraphicalOverview;
class GradientWidgetImpl;
class GlossaryDialog;
class IsotopeTable;
class SearchWidget;
class QKeyEvent;
class LegendWidget;
class TableInfoWidget;
class ExportDialog;

/**
 * @brief Application Main Window
 * @author Carsten Niehaus <cniehaus@kde.org>
 * @version 1.2
 *
 * This class connects the parts of kalzium. All actions will
 * be set, the menu generated and so on.
 */
class Kalzium : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    Kalzium();

    /**
     * Default Destructor
     */
    virtual ~Kalzium();

private:
    DetailedInfoDlg   *m_infoDialog;

    LegendWidget *m_legendWidget;

    TableInfoWidget *m_TableInfoWidget;

    PeriodicTableView *m_periodicTable;

    /**
     *initialize actions
     */
    void setupActions();

    /**
     *initialize the sidebars
     */
    void setupSidebars();

    void extractIconicInformationAboutElement( int elementNumber );

    /**
     * all KActions Kalzium uses
     */
    KSelectAction *look_action_schemes;
    KSelectAction *look_action_gradients;

    KSelectAction *table_action;
    KSelectAction *table_action_schemes;
    KSelectAction *numeration_action;
    // settings
    QAction *m_pLegendAction;
    QAction *m_SidebarAction;
    // tools
    QAction *m_pPlotAction;
    QAction *m_pcalculator;
    QAction *m_pIsotopeTableAction;
    QAction *m_pGlossaryAction;
    QAction *m_pRSAction;
    QAction *m_pTables;
    QAction *m_pMoleculesviewer;
    QAction *m_pOBConverterAction;
    QAction *m_EQSolverAction;
    QAction *export_action;

    QAction *m_legendAction;

    MolcalcWidget *m_calcWidget;

    GradientWidgetImpl *m_gradientWidget;

    DetailedGraphicalOverview *m_detailWidget;

    /**
     * the dock of the central pse Widget
     */
    QDockWidget *m_dockWin;
    QDockWidget *m_legendDock;
    QDockWidget *m_tableDock;

    QToolBox *m_toolbox;

    int m_toolboxCurrent;

    GlossaryDialog *m_glossarydlg;

    SearchWidget *m_searchWidget;

    ExportDialog *m_exportDialog;

    UnitSettingsDialog *m_unitsDialog;

Q_SIGNALS:
    void numerationChanged(int);

private slots:
    /**
     * opens the information dialog for the element @p number
     */
    void openInformationDialog( int number );

    void elementHover( int num );
    void setupStatusBar();

    void slotShowEQSolver();

    /**
     * switched to the table with the index @p index
     */
    void slotSwitchtoTable( int index );

    /**
     * start the glossary
     */
    void slotGlossary();

    /**
     * show the R/S-phrases
     */
    void slotRS();

    void slotMoleculeviewer();

    void slotOBConverter();

    void slotTables();

    void slotSwitchtoLookGradient( int );
    void slotSwitchtoLookScheme( int );
    void slotSwitchtoNumeration(int);

    /**
     *start the isotopetable
     */
    void slotIsotopeTable();

    void slotPlotData();
    /**
     * Start the calculator
     */
    void showCalculator();

    void slotExportTable();

    void slotShowExportDialog();

    /**
     * These slots are for the standard actions
     */
    void showSettingsDialog();

    void slotUpdateSettings();

protected:
    virtual QSize sizeHint() const;
};

#endif // _KALZIUM_H_
