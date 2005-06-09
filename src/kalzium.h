/***************************************************************************
    copyright            : (C) 2003, 2004 by Carsten Niehaus
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
#ifndef _KALZIUM_H_
#define _KALZIUM_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kmainwindow.h>

class KSelectAction;
class KToggleAction;
class QuizsettingsDlg;
class SliderWidget;
class QVBoxLayout;
class QDockWindow;
class TempSlider;
class KalziumDataObject;
class DetailedInfoDlg;
class PSE;
class MolcalcWidget;
class DetailedGraphicalOverview;
class timeWidget;
class SOMWidgetIMPL;

/**
 * @short Application Main Window
 * @author Carsten Niehaus <cniehaus@kde.org>
 * @version 1.1
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

		KalziumDataObject* data() const;

	private:
		class privatedata;
		friend class privatedata;
		privatedata* pd;

/*
		///the KComboBoxes for the statusbar (temperature and energie)
		KComboBox *e_box,
				  *t_box,
				  *u_box;
*/

		DetailedInfoDlg   *m_infoDialog;

		/**
		 * this Pointer points to the current PSE. The pointer
		 * is updated every time the users chooses a new 
		 * PSE
		 */
		PSE *m_PSE;
			
		/**
		 *initialize the Statusbar
		 */		
		void setupStatusBar();

		/**
		 *initialize actions
		 */		
		void setupActions();
		
		/**
		 *initialize the sidebars
		 */		
		void setupSidebars();
		
		/**
		 *diplay the energy unit in the Statusbar
		 */
		void updateStatusbar();	
		
		/**
		 * Signal the PSE that it should do a full draw operation
		 */
		void setFullDraw();
		
		/**
		 * all KActions Kalzium uses
		 */
		KSelectAction *gradient_action;
		KSelectAction *numeration_action;
		KAction *m_pPlotAction,
				*m_pLegendAction,
				*m_pGlossaryAction,
				*m_SidebarAction;

		KToggleAction *m_actionNoScheme;
		KToggleAction *m_actionGroups;
		KToggleAction *m_actionBlocks;
		KToggleAction *m_actionAcid;
		KToggleAction *m_actionFamily;
		KToggleAction *m_actionCrystal;
		
		/**
		 * the layout of the central Widget ( CentralWidget )
		 */
		QVBoxLayout *m_pCentralLayout;

		MolcalcWidget *m_calcWidget;

		timeWidget *m_timeWidget;
		
		SOMWidgetIMPL *m_somWidget;

		DetailedGraphicalOverview *m_detailWidget;

		bool m_showSidebar;

		QDockWindow *m_dockWin;
	
	private slots:
		void slotStatusbar(int);
		
		/**
		 * opens the information dialog for the element @p number
		 */
		void openInformationDialog( int number );

		/**
		 * called when the user changed a KComboBox in the statusbar
		 */
		void adjustUnits();
	

		/**
		 * this slot switches Kalzium to the selected colorscheme
		 */
		void slotShowScheme(int);

		void slotShowLegend();
		
		void slotGlossary();
		
		void slotSwitchtoNumeration(int);
		
		/**
		 * this will make the periodic table display the gradient
		 * defined by @p int gradient
		 */
		void slotSwitchtoGradient(int gradient);

		void slotPlotData();

		void slotUpdateSettings();

		void slotNoLook();

		void slotLookGroups();
		void slotLookBlocks();
		void slotLookAcidBehavior();
		void slotLookFamily();
		void slotLookCrystal();
	
		/**
		 * These slots are for the standardactions
		 */
		void showSettingsDialog();

		void slotToolboxCurrentChanged(int);

		void slotSelectedNumber(int);

		void slotShowHideSidebar();

	signals:
		/**
		 * this signal is emited when the user activated a dialog.
		 * For example, no tooltips will be displayed until
		 * tableLocked(false) is emited
		 */
		void tableLocked(bool);
};

/**
 * @short A private data class Kalzium uses
 * @author Carsten Niehaus <cniehaus@kde.org>
 * @version 1.0
 */
class Kalzium::privatedata
{
	public:
		Kalzium *k;
		KalziumDataObject *kalziumData;

		privatedata( Kalzium *parent ) : k( parent ){};
};
	

#endif // _KALZIUM_H_
