/***************************************************************************
                          quizdlg.h  -  description
                             -------------------
    begin                : Thu Dec 24 2002
    copyright            : (C) 2002, 2003 by Carsten Niehaus
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

#ifndef QUIZDLG_H
#define QUIZDLG_H

#include "quizdlgui.h"

class QLabel;
class QRadioButton;
class QWidget;

/**
* This class provides the dialog for the knowledge test. 
* It is a simple KDialog.
* @author Carsten Niehaus
*/

class QuizDlg : public QuizDlgUI 
{
    Q_OBJECT

    public: 
	    /**
        * construtor 
        */
        QuizDlg(QWidget *parent, const char *name, int numofquestions, bool modal=true );
    
    private:
        int i, quizresult, currentnr, qnum, correctis;
        int order[69];
         
        /*
        * the three QRadioButton you select when answering
        */
        QString question, alternative1, alternative2, answer;
        
        void increaseIfCorrect( int );
    	bool QuestioniWasCorrect[20]; //FIXME s/10/numofquestions
	                              //but I don't know, how...
        void setTexts();
    	bool wasCorrect( int ) const;
	
    private slots:
        
        /**
        * check if the answer is correct and do the rest 
        * of the quiz 
        */
        void slotCheck();
        
        /**
        * update the buttons
        */
        void updateIt();
};

#endif
