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
		 * Kalzium supports three different PSE-types. This is
		 * the object for one of this types
		 * @see PSE
		 */
		MendeljevPSE* m_pMendeljevPSE;

		/**
		 * this pointer returns the currently loaded PSE
		 * @return the currently used PDE
		 */
		PSE* currentPSE() const;

	private:
		/**
		 * this Pointer points to the current PSE. The pointer
		 * is updated every time the users chooses a new 
		 * PSE
		 */
		PSE* m_pCurrentPSE;
			
		void setupBlockLists( KalziumDataObject *d );
		
		/*
		 * all KActions Kalzium uses
		 **/
		KSelectAction *schema_action;
		KAction *m_pQuizStart,
			*m_pQuizSetup,
			*m_pQuizEditQuestions,
			*m_pQuizAddQuestions,

			*m_pBehAcidAction,
			*m_pBehNormalAction,
			*m_pBehBlocksAction,
			*m_pBehGroupAction,
			*m_pBehSOMAction,
			*m_pTimelineAction;
		
	private slots:
		/**
		 * this slot switches Kalzium to the selected colorscheme
		 */
		void slotShowScheme(void);

		/**
		 * this slot activates the timeline.
		 */
		void slotShowTimeline();

		void slotSaveConfig();
	
		/**
		 * This slot switches Kalzium to the selected PSE
		 */
		void slotSwitchtoPSE(int);

		/**
		 * start the quiz
		 */
		void slotStartQuiz();
		
		/**
		 * this slot starts the questionseditor.
		 * @see questionEditorImpl
		 */
		void slotEditQuestions();

		/**
		 * this slot starts a dialog in which you can add
		 * questions for the quiz
		 * @see questionAddDialogImpl
		 * @see Quiz
		 */
		void slotAddQuestions();

		/**
		 * These slots are for the standardactions
		 */
		void showSettingsDialog();
		void optionsConfigureKeys();
		void newToolbarConfig();
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
