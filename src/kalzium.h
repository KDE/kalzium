/*
 * Copyright (C) 2003, 2004 Carsten Niehaus <cniehaus@kde.org>
 */

#ifndef _KALZIUM_H_
#define _KALZIUM_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kmainwindow.h>

#include "pse.h"

/**
 * @short Application Main Window
 * @author Carsten Niehaus <cniehaus@kde.org>
 * @version 1.0
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

		/*
		 *as of now Kalzium supports three different PSE-types
		 */
		SimplifiedPSE* m_pSimplePSE;
		RegularPSE* m_pRegularPSE;
		MendeljevPSE* m_pMendeljevPSE;

		PSE* currentPSE() const;

	private:
		/*
		 * this Pointer points to the current PSE. The pointer
		 * is updated every time the users chooses a new 
		 * PSE
		 **/
		PSE* m_pCurrentPSE;
			
		void setupBlockLists( KalziumDataObject *d );
		
		/*
		 * all KActions Kalzium uses
		 **/
		KAction *m_pSimplePSEAction,
			*m_pRegularPSEAction,
			*m_pMendeljevPSEAction;
		

	private slots:
		/*
		 * the following three slots let Kalzium switch to the 
		 * corresponding PSE
		 **/
		void slotSwitchtoSimplePSE();
		void slotSwitchtoRegularPSE();
		void slotSwitchtoMendeljevPSE();

		/*
		 * These slots are for the standardactions
		 **/
		void showSettingsDialog();
		void optionsConfigureKeys();
		void optionsConfigureToolbars();
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
