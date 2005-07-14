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
class QToolBox;
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
class GlossaryDialog;

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

		/**
		 * @return the data of Kalzium
		 */
		KalziumDataObject* data() const;

	private:
		class privatedata;
		friend class privatedata;
		privatedata* pd;

		DetailedInfoDlg   *m_infoDialog;

		/**
		 * this pointer points to the PSE.
		 */
		PSE *m_PSE;
		
		/**
		 *initialize actions
		 */
		void setupActions();
		
		/**
		 *initialize the sidebars
		 */
		void setupSidebars();
		
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
				*m_pGlossaryAction,
				*m_SidebarAction;
		
		KToggleAction *m_pTooltipAction;
		KToggleAction *m_pLegendAction;

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

		QDockWindow *m_dockWin;
		QToolBox *m_toolbox;
		int m_toolboxCurrent;

		GlossaryDialog *m_glossarydlg;
	
	private slots:
		/**
		 * opens the information dialog for the element @p number
		 */
		void openInformationDialog( int number );

		void slotStatusbar( int num );
		void setupStatusBar();


		/**
		 * this slot switches Kalzium to the selected color scheme
		 */
		void slotShowScheme(int);

		void slotShowLegend();
		
		/**
		 * Hide or show the tooltips
		 */
		void slotEnableTooltips();
		
		/**
		 * start the glossary
		 */
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
		 * These slots are for the standard actions
		 */
		void showSettingsDialog();

		void slotToolboxCurrentChanged(int);

		void slotSelectedNumber(int);

		void slotShowHideSidebar();

		void slotSidebarVisibilityChanged(bool);

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
