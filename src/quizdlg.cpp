/***************************************************************************
                         quizdlg.cpp  -  description
                             -------------------
    begin                : Thu Jan 24 2002 
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

#include "config.h"

//KDE-Includes
#include <kdialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <ksimpleconfig.h>
#include <kstddirs.h>
#include <kprogress.h>

//QT-Includes
#include <qbuttongroup.h>
#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qradiobutton.h>
#include <qstring.h>
#include <qtable.h>
#include <qwhatsthis.h>
#include <qwidget.h>

// Standard C++ includes
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <time.h>

#include "quizdlg.h"
#include "quizdlgui.h"

QuizDlg::QuizDlg (QWidget *parent, const char *name, int numofquestions )  
    : QWidget (parent,name)
{
    qnum = numofquestions;

    srandom( time( 0 ) );
    for (int e = 0 ; e < 61 ; e++ )
    	order[e] = e+1;

    int a, b, temp;

    for (int r = 0 ; r < 50 ; r++ )
    {
    	b = rand()%61;
    	do {
    	    a = rand()%61;
    	} while (b == a);

    	temp = order[a];
    	order[a] = order[b];
    	order[b] = temp;
    }

    quizresult = 0;
    currentnr = 0;
    i = 0;

    quizdlg = new QuizDlgUI();
    quizdlg->show();
    quizdlg->KProgress1->setTotalSteps( qnum );
    quizdlg->KProgress1->setProgress(0);

    connect( quizdlg->bgroup, SIGNAL(clicked(int)), SLOT(updateIt()) );

    connect( quizdlg->confirm, SIGNAL(clicked() ), this, SLOT( slotCheck() ) );
    
    setTexts();

}


void QuizDlg::increaseIfCorrect( int i )
{
    switch (i)
    {
	case 1:
	    if (quizdlg->one->isChecked() == true)
	    {
		quizresult++; //increase if correct
		QuestioniWasCorrect[currentnr] = true;
	    }
	    break;
	case 2:
	    if (quizdlg->two->isChecked() == true) 
	    {
    		quizresult++; //increase if correct
    		QuestioniWasCorrect[currentnr] = true;
	    }
	    break;
	case 3:
	    if (quizdlg->three->isChecked() == true) 
	    {
    		quizresult++; //increase if correct
    		QuestioniWasCorrect[currentnr] = true;
	    }
	    break;
    } 

}

void QuizDlg::setTexts()
{
    //ok, read out the data
    KSimpleConfig quizconfig (locate("data", "kalzium/kalziumrc"));
    quizconfig.setGroup("q"+QString::number(order[i]));
    question=quizconfig.readEntry("Q", "Unknown");
    answer=quizconfig.readEntry("A", "Unknown");
    alternative1=quizconfig.readEntry("Alternative1", "Unknown");
    alternative2=quizconfig.readEntry("Alternative2", "Unknown");
    quizdlg->titleText->setText(i18n(question.utf8()));
    
    //now we need to know where to put the correct answer
    correctis = (rand()%3)+1;
    //now we put it
    
    switch (correctis)
    {
        case 1:
            quizdlg->one->setText(i18n(answer.utf8()));
            quizdlg->two->setText(i18n(alternative1.utf8()));
            quizdlg->three->setText(i18n(alternative2.utf8()));
            break;
        case 2:
            quizdlg->two->setText(i18n(answer.utf8()));
            quizdlg->one->setText(i18n(alternative1.utf8()));
            quizdlg->three->setText(i18n(alternative2.utf8()));
            break;
        case 3:
            quizdlg->three->setText(i18n(answer.utf8()));
            quizdlg->two->setText(i18n(alternative1.utf8()));
            quizdlg->one->setText(i18n(alternative2.utf8()));
            break;
    }
}



bool QuizDlg::wasCorrect( int i ) const
{
    return QuestioniWasCorrect[i];
}

//******* Slots ******************************************************

void QuizDlg::slotCheck()
{
    increaseIfCorrect( correctis );

    //if nothing at all is checked
    if (quizdlg->one->isChecked() == false && quizdlg->two->isChecked()== false  && quizdlg->three->isChecked()== false )
    {
        KMessageBox::error ( this, i18n("You haven't selected a button") );
    }
    //ok, something is checked
    else
    {
        currentnr++;
        quizdlg->KProgress1->setProgress(currentnr);
        //if it has not been the last question
        if (currentnr != qnum)
        {
            i++;              //increase the current number
            quizdlg->one->setChecked( FALSE );
            quizdlg->two->setChecked( FALSE );
            quizdlg->three->setChecked( FALSE );
            setTexts();       //set the next questions
        }

        //ok, the quiz is over, show the results
        else
        {
            this->close();
            KDialog *result = new KDialog ( this, "result" );
            result->setCaption(i18n("See your results!"));

            QGridLayout *grid = new QGridLayout(result , 2 , 0 , 8 );

            KPushButton *exit = new KPushButton( i18n("&Close"), result );
            QObject::connect (exit, SIGNAL(clicked()), result, SLOT(hide()));
            QWhatsThis::add(exit, i18n("If you click on this button this dialog will close."));
            grid->addWidget(exit,2,0);

            QLabel *resultlabel = new QLabel (result);
            QString resulttext;
            resulttext = i18n("You answered %1 of %2 questions correctly.").arg(QString::number(quizresult)).arg(qnum);
            resultlabel->setText(i18n(resulttext.utf8()));
            grid->addWidget( resultlabel, 0 , 0 );

            QTable *resultTable = new QTable( qnum+1 , 3 , result , "resultTable" );
            resultTable->setReadOnly( true );
            resultTable->setColumnStretchable( 0 , true );
            resultTable->setColumnStretchable( 1 , true );
            resultTable->setTopMargin( 0 );
            resultTable->setLeftMargin( 0 );
            grid->addWidget( resultTable, 1 , 0 );

            QPixmap good = SmallIcon("apply");
            QPixmap bad  = SmallIcon("cancel");

            resultTable->setText( 0 , 0 , i18n("Questions:") );
            resultTable->setText( 0 , 1 , i18n("The correct answer was:") );
            resultTable->setText( 0 , 2 , i18n("You have been:") );

            resultTable->horizontalHeader()->hide();
            resultTable->verticalHeader()->hide();
            resultTable->setShowGrid( false );

            KSimpleConfig quizconfig (locate("data", "kalzium/kalziumrc"));
            for ( int i = 0 ; i < qnum ; i++ )
            {
                resultTable->setRowStretchable( i+1 , true );
                quizconfig.setGroup("q"+QString::number(order[i]));

                QString ques, answ;
                ques=quizconfig.readEntry("Q", "Unknown");
                answ=quizconfig.readEntry("A", "Unknown");

                QTableItem *item = new QTableItem(resultTable, QTableItem::Never, i18n(ques.utf8()));
                item->setWordWrap(true);
                resultTable->setItem( i+1 , 0 , item);
                item = new QTableItem(resultTable, QTableItem::Never, i18n(answ.utf8()));
                item->setWordWrap(true);
                resultTable->setItem( i+1 , 1 , item);

                if (wasCorrect( i ) == true)
                    resultTable->setPixmap( i+1 , 2 , good );
                else 
                    resultTable->setPixmap( i+1 , 2 , bad  );
            }	    
            result->show();
        }
    }
}

void QuizDlg::updateIt()
{
    update();
}

#include "quizdlg.moc"
