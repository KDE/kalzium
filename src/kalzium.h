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

#include "pse.h"
#include <kmainwindow.h>

class KSelectAction;
class QuizsettingsDlg;
class SliderWidget;
class QVBoxLayout;
class TempSlider;
class Legend;

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

		class privatedata;
		friend class privatedata;
		privatedata* pd;

		/**
		 * Kalzium supports three different PSE-types. This is
		 * the object for one of this types
		 * @see PSE
		 */
		SimplifiedPSE* m_pSimplePSE;
		/**
		 * Kalzium supports three different PSE-types. This is
		 * the object for one of this types
		 * @see PSE
		 */
		RegularPSE* m_pRegularPSE;
		
		/**
		 * this pointer returns the currently loaded PSE
		 * @return the currently used PDE
		 */
		PSE* currentPSE() const;

	private:
		/**
		 * the date which the pSliderWidget will start with
		 */
		int date;

		/**
		 * the slider used for the State of Matter
		 */
		TempSlider *m_pSOMSlider;

		/**
		 * the slider used for the timeline
		 */
		SliderWidget *m_pTimeSlider;

		/**
		 * the legend for the periodic tables
		 */
		Legend *m_pLegend;

		/**
		 * if true, the legend will be shown
		 */
		bool m_bShowLegend;
		
		/**
		 * if true, the state of matter will be shown
		 */
		bool m_bShowSOM;
		
		/**
		 * if true, the timeline will be shown
		 */
		bool m_bShowTimeline;
		
		/**
		 * this Pointer points to the current PSE. The pointer
		 * is updated every time the users chooses a new 
		 * PSE
		 */
		PSE* m_pCurrentPSE;
			
		/**
		 *initialize the Statusbar
		 */		
		void setupStatusBar();

		/**
		 *initialize actions
		 */		
		void setupActions();
		
		/**
		 *display text in the statusbar item at index int
		 */
		void slotStatusBar(QString text, int id);

		/**
		 *diplay the Temperature unit in the Statusbar
		 */
		void displayTemperature();
		
		/**
		 *diplay the energy unit in the Statusbar
		 */
		void displayEnergie();	
		
		/**
		 * all KActions Kalzium uses
		 */
		KSelectAction *schema_action;
		KSelectAction *look_action;
		KSelectAction *numeration_action;
		KAction *m_pTimelineAction,
			   *m_pPlotAction,
			    *m_pSOMAction,
				*m_pLengendAction;
		
		/**
		 * the layout of the central Widget ( CentralWidget )
		 */
		QVBoxLayout *m_pCentralLayout;
	
	private slots:
		/**
		 * this slot switches Kalzium to the selected colorscheme
		 */
		void slotShowScheme(int);
		
		void slotShowLegend();
		
		void slotStateOfMatter();

		void slotSwitchtoNumeration(int);

		void slotPlotData();

		/**
		 * this slot activates the timeline.
		 */
		void slotShowTimeline();

		void slotUpdateSettings();
	
		/**
		 * This slot switches Kalzium to the selected PSE
		 */
		void slotSwitchtoPSE(int);
		
		/**
		 * These slots are for the standardactions
		 */
		void showSettingsDialog();
		
		/**
		 * This slot passes the value of the SOM-slider to the PSE
		 */
		void slotTempChanged( int );
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
