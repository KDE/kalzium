/***************************************************************************
                          kalzium.h  -  description
                             -------------------
    begin                : Die Dez  4 17:59:34 CET 2001
    copyright            : (C) 2001 by Carsten Niehaus
    email                : cniehaus@gmx.de
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kmainwindow.h>


#define KALZIUM_VERSION "0.4"

/** Kalzium is the base class of the project */


class QGridLayout;
class QLayout;
class QLabel;
class QLCDNumber;
class QSlider;
class QString;

class KConfig;
class KSelectAction;
class KToggleAction;

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
	PElementKP element[118];
        PQLabel labels[18];
        int quizresult;
        int numofquestions;
        int i;
	Colors_Config_Widget *colorsconfig;
        CalcDialog *calculationdialog;
	QGridLayout *maingrid;

    KSelectAction *numerationmenu, *psestylemenu, *colorschememenu, *weblookupmenu;
    KToggleAction *timelineToggleAction; 
   
    KConfig *main_config;
    
    StateOfMatterDlg *templookup;
    
    private:
    QSlider *dateS;
    QLCDNumber *dateLCD;
    SettingsDialog *setDlg;

    QString block, acidbeh;
    int ischecked;

    //the two different numerationschemes
    void showCAS();
    void showIUPAC();

    public slots:
    /**
    * this slot hides all elements which have not been known
    * when Mendelejew created the first PSE
    */
    void slotShowMendelejew();

    /**
    * this slot gives you information if the elements tends to
    *built acid, bases, doesn neither or is amphoter
    */
    void slotShowAcidBeh();
    
    //shows you the 4 different blocks
    void slotShowBlocks();
    
    /**
    * this slot gives you the information in which group
    * the elements is (1 to 8)
    */
    void slotShowGroups();
    
    /**
    * this slot shows the users in what state the element
    * is at 20 degree and 1013 h-pascal.
    */
    void slotShowStateOfMatter();
    
    /**
    * this slots shows all elements
    */
    void slotShowAll();
    
    /**
    * Which element has been known when? A slider will
    * appear and you can check it out.
    */
    void timeline();

    void changeNumeration(int);
    void changeColorScheme(int id=-1);

    //update the menues
    void updateNumMenu(int);
    void updateColourMenu(int);
    
    void pseStyleShow(int);
    void slotShowTimeline(bool);

    /**
    * the quiz will start
    */
    void slotKnowledge();
    
    void slotValues();

    /**
    * this slot will open a window in which you can do some calculations
    */
    void slotCalculations();
   
    void defineweights();
   
    private slots:
        void showSettingsDialog();
        void hideSettingsDialog();
    protected:
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
        * Called after setupActions , it sets the main window look 
        * according to the menu settings
        * @see Kalzium::setupActions
        */
        void updateMainWindow();
};
#endif
