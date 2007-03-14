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
#ifndef KALZIUM_H
#define KALZIUM_H

#include <kmainwindow.h>

class KAction;
class KSelectAction;
class QToolBox;
class QVBoxLayout;
class QDockWidget;
class DetailedInfoDlg;
class PeriodicTableView;
class MolcalcWidget;
class DetailedGraphicalOverview;
class SOMWidgetIMPL;
class TimeWidgetImpl;
class GlossaryDialog;
class IsotopeTable;
class SearchWidget;
class QKeyEvent;

#include "kalziumpainter.h"

/**
 * @brief Application Main Window
 * @author Carsten Niehaus <cniehaus@kde.org>
 * @version 1.2
 *
 * This class connects the parts of kalzium. All actions will
 * be set, the menu generated and so on.
 */
class Kalzium : public KMainWindow
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

		/**
		 * this pointer points to the PeriodicTableView.
		 */
		PeriodicTableView *m_PeriodicTableView;
		
		/**
		 *initialize actions
		 */
		void setupActions();
		
		/**
		 *initialize the sidebars
		 */
		void setupSidebars();
		
		/**
		 * all KActions Kalzium uses
		 */
		KSelectAction *look_action;
		KSelectAction *look_action_schemes;
		KSelectAction *numeration_action;
		// settings
		QAction *m_pLegendAction;
		QAction *m_SidebarAction;
		// tools
		QAction *m_pPlotAction;
		QAction *m_pIsotopeTableAction;
		QAction *m_pGlossaryAction;
		QAction *m_pRSAction;
		QAction *m_pTables;
		QAction *m_pMoleculesviewer;
		QAction *m_pOBConverterAction;
		QAction *m_EQSolverAction;

		/**
		 * the layout of the central Widget ( CentralWidget )
		 */
		QVBoxLayout *m_pCentralLayout;

		MolcalcWidget *m_calcWidget;

		SOMWidgetIMPL *m_somWidget;
		TimeWidgetImpl *m_timeWidget;

		DetailedGraphicalOverview *m_detailWidget;

		QDockWidget *m_dockWin;
		QToolBox *m_toolbox;
		int m_toolboxCurrent;

		GlossaryDialog *m_glossarydlg;

		SearchWidget *m_searchWidget;

		KalziumPainter::MODE m_prevNormalMode;

	public slots:
		void slotSearchElements();
	
	private slots:
		/**
		 * opens the information dialog for the element @p number
		 */
		void openInformationDialog( int number );

		void elementHover( int num );
		void setupStatusBar();

		void slotShowEQSolver();
		
    void slotSwitchtoStyle( int index );
		
		void slotShowLegend();
		
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
		
		void slotSwitchtoLook( int );
		void slotSwitchtoNumeration(int);
		
		/**
		 *start the isotopetable
		 */
		 void slotIsotopeTable();

		void slotPlotData();

		void slotUpdateSettings();

		/**
		 * These slots are for the standard actions
		 */
		void showSettingsDialog();

		void slotToolboxCurrentChanged(int);

		void slotShowHideSidebar( bool checked, bool changeconfig = true );

	protected:
		virtual void keyPressEvent( QKeyEvent * e);
};

#endif // _KALZIUM_H_
