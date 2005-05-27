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
class InformationWidget;

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

	private:
		/**
		 * the date which the pSliderWidget will start with
		 */
		int date;

		InformationWidget *m_info;
		
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
		 *display text in the statusbar item at index int
		 */
		void slotStatusBar(const QString& text, int id);

		/**
		 *diplay the energy unit in the Statusbar
		 */
		void displayEnergie();	
		
    /**Signal the PSE that it should do a full draw operation
      */
    void setFullDraw() { m_PSE->setFullDraw(); }
		
    /**
		 * all KActions Kalzium uses
		 */
		KSelectAction *gradient_action;
		KSelectAction *look_action;
		KSelectAction *numeration_action;
		KAction *m_pPlotAction,
				*m_pCalcAction,
				*m_pLegendAction,
				*m_pCrystalAction,
				*m_pGlossaryAction,
				*m_pLearningmodeAction;
		
		/**
		 * the layout of the central Widget ( CentralWidget )
		 */
		QVBoxLayout *m_pCentralLayout;
	
	private slots:
		/**
		 * opens the information dialog for the element @p number
		 */
		void openInformationDialog( int number );

		/**
		 * this slot switches Kalzium to the selected colorscheme
		 */
		void slotShowScheme(int);

		void slotShowCrystal();
		
		void slotShowLegend();
		
		void slotGlossary();
		
		void slotSwitchtoNumeration(int);
		
		/**
		 * this will make the periodic table display the gradient
		 * defined by @p int gradient
		 */
		void slotSwitchtoGradient(int gradient);

		void slotPlotData();

		void slotCalculate();
		
		void slotLearningmode();

		void slotUpdateSettings();
	
		/**
		 * These slots are for the standardactions
		 */
		void showSettingsDialog();

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
