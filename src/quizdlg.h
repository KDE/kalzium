/***************************************************************************
                          quizdlg.h  -  description
                             -------------------
    begin                : Thu Dec 24 2002
    copyright            : (C) 2002 by Carsten Niehaus
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

class KDialog;
class QLabel;
class QRadioButton;
class QWidget;

/*
*This class provides the dialog for the knowledge test. It is a simple
KDialog.
*@author Carsten Niehaus
*/

class QuizDlg : public KDialog  {
    Q_OBJECT

    public: 
	/** construtor */
        QuizDlg(QWidget *parent, const char *name, int numofquestions );
    
    private:
	bool wasCorrect( int );
	bool QuestioniWasCorrect[10]; //FIXME s/10/numofquestions
	                              //but I don't know, how...
				      
	void increaseIfCorrect( int );
	
        //the three QRadioButton you select when answering
        QRadioButton *one;
        QRadioButton *two;
        QRadioButton *three;
        QLabel *titleText;
        QString question;
        QString alternative1;
        QString alternative2;
        QString answer;
        int i, quizresult, currentnr, qnum, correctis;
        int order[69];

        void setTexts();

    private slots:
        //update the buttons
        void updateIt( int );

        //check if the answer is correct and do the rest
        //of the quiz
        void slotCheck();
};

#endif
