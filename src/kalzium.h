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
#ifndef KALZIUM_H
#define KALZIUM_H

#include <kmainwindow.h>

class KSelectAction;
class QToolBox;
class QVBoxLayout;
class QDockWidget;
class DetailedInfoDlg;
class PeriodicTableView;
class MolcalcWidget;
class DetailedGraphicalOverview;
class SOMWidgetIMPL;
class GlossaryDialog;
class IsotopeTable;
class PrintWidget;
class DoubleSliderWidget;
class QKeyEvent;
class QTimer;

#include "kalziumpainter.h"

/**
 * @short Application Main Window
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
		KSelectAction *numeration_action;
		// settings
		KAction *m_pLegendAction;
		KAction *m_SidebarAction;
		// tools
		KAction *m_pPlotAction;
		KAction *m_pIsotopeTableAction;
		KAction *m_pGlossaryAction;
		KAction *m_pCrystalViewer;
//		KAction *m_EQSolverAction;

		/**
		 * the layout of the central Widget ( CentralWidget )
		 */
		QVBoxLayout *m_pCentralLayout;

		MolcalcWidget *m_calcWidget;

		DoubleSliderWidget *m_doubleWidget;
		
		SOMWidgetIMPL *m_somWidget;

		DetailedGraphicalOverview *m_detailWidget;

		PrintWidget* m_printWidget;

		QDockWidget *m_dockWin;
		QToolBox *m_toolbox;
		int m_toolboxCurrent;

		GlossaryDialog *m_glossarydlg;

		KalziumPainter::MODE m_prevNormalMode;

	public slots:
		void slotSearchElements();
	
	private slots:
		/**
		 * opens the information dialog for the element @p number
		 */
		void openInformationDialog( int number );

		void slotStatusbar( int num );
		void setupStatusBar();

		void slotCrystalViewer();

		void slotShowEQSolver();
		
		void slotShowLegend();
		
		/**
		 * start the glossary
		 */
		void slotGlossary();
		
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

		void slotSelectedNumber(int);

		void slotShowHideSidebar();

		void slotSidebarVisibilityChanged(bool);

	private:
		bool m_activeTypeSearch;
		QString m_typeAheadString;

		void findAheadStop();

		QTimer *m_searchTimer;

	protected:
		virtual void keyPressEvent( QKeyEvent * e);
};

#endif // _KALZIUM_H_
