/***************************************************************************

        kalzium.h  -  description
                             -------------------
    begin                : Die Dez  4 17:59:34 CET 2001
    copyright            : (C) 2001 by Carsten Niehaus
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

#ifndef _KALZIUM_H
#define _KALZIUM_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kmainwindow.h>


#define KALZIUM_VERSION "0.5"

/** Kalzium is the base class of the project */


class QGridLayout;
class QLayout;
class QLabel;
class QLCDNumber;
class QSlider;
class QString;
class QButton;

class KConfig;
class KSelectAction;
class KColorButton;
class KToggleAction;
class KPushButton;

class CalcDialog;
class Colors_Config_Widget;
class ElementKP;
class SettingsDialog;
class StateOfMatterDlg;

typedef ElementKP* PElementKP;
typedef QLabel* PQLabel;

class Kalzium : public KMainWindow

{
	Q_OBJECT

	public:
		/** construtor */
		Kalzium(const char *name=0);
		/** destructor */
		~Kalzium();

		int numofquestions;
		PElementKP element[118];
		QString helpArray[9][18];
		void createhelpArray();
		CalcDialog *calculationdialog;
		Colors_Config_Widget *colorsconfig;
		KSelectAction *numerationmenu, *psestylemenu, *colorschememenu, *weblookupmenu;
		StateOfMatterDlg *templookup;
        KPushButton *one, *two, *three, *four, *five, *six, *seven, *eight;

	private:
		PQLabel labels[18];
		KConfig *main_config;
		KToggleAction *timelineToggleAction, *toolbarToggleAction;
		QGridLayout *maingrid;
		QLCDNumber *dateLCD;
		QSlider *dateS;
		SettingsDialog *setDlg;


		/**
		 * Called before the window is closed, will save the menusettings.
		 */
		bool queryClose();

		void setupActions();

		/**
		 * This method sets the menu for kalzium using XMLGUI. 
		 */
		void setupConfig();

		/**
		 * numeration theme
		 */
		void showCAS() const;

		/**
		 * numeration theme
		 */
		void showIUPAC() const;


		/**
		 * Called after setupActions , it sets the main window look 
		 * according to the menu settings
		 * @see Kalzium::setupActions
		 */
		void updateMainWindow();


		//******* Slots ******************************************************

		public slots:

			void changeColorScheme(int id=-1);
            void changeLegend(int id=-1);

		private slots:

			void changeNumeration(int) const;

		void defineweights() const;

		void showPseStyle(int);

		void showToolbar();

		/**
		 * this slot will open a window in which you can do some calculations
		 */
		void slotCalculations();

		/**
		 * the quiz will start
		 */
		void slotKnowledge();

		/**
		 * this slot gives you information if the elements tends to
		 *built acid, bases, doesn neither or is amphoter
		 */
		void slotShowAcidBeh();

		/**
		 * this slots shows all elements
		 */
		void slotShowAll();

		/*
		 * shows you the 4 different blocks
		 */
		void slotShowBlocks();

		/**
		 * this slot gives you the information in which group
		 * the elements is (1 to 8)
		 */
		void slotShowGroups();

		/**
		 * this slot hides all elements which have not been known
		 * when Mendelejew created the first PSE
		 */
		void slotShowMendelejew();

		/**
		 * this slot shows the users in what state the element
		 * is at 20 degree and 1013 h-pascal.
		 */
		void slotShowStateOfMatter();

		void updateColorMenu(int);
		
        void updateNumMenu(int);

        /**
         * connected to void KApplication::kdisplayFontChanged() 
         * so I can set for KDE 3.1 a fixed size for the 
         * element buttons
         * cause i have to play with kalzium's layout managment
         * to understand it
         **/
        void updateElementKPSize();
        
		void slotShowTimeline(bool);

		void slotValues();

		void hideSettingsDialog();

		void showSettingsDialog();

		/**
		 * Which element has been known when? A slider will
		 * appear and you can check it out.
		 */
		void timeline();

};
#endif
