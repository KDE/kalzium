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
#include <kstandarddirs.h>
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

// Standard C++ includes
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <time.h>

#include "quizdlg.h"
#include "quiz_result.h"

QuizDlg::QuizDlg (QWidget *parent, const char *name, int numofquestions )  
    : QuizDlgUI (parent,name)
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

    KProgress1->setTotalSteps( qnum );
    KProgress1->setProgress(0);
    
    
    connect( bgroup, SIGNAL(clicked(int)), SLOT(updateIt()) );

    connect( confirm, SIGNAL(clicked() ), this, SLOT( slotCheck() ) );
    
    setTexts();

}


void QuizDlg::increaseIfCorrect( int i )
{
    switch (i)
    {
	case 1:
	    if (one->isChecked() == true)
	    {
		quizresult++; //increase if correct
		QuestioniWasCorrect[currentnr] = true;
	    }
	    break;
	case 2:
	    if (two->isChecked() == true) 
	    {
    		quizresult++; //increase if correct
    		QuestioniWasCorrect[currentnr] = true;
	    }
	    break;
	case 3:
	    if (three->isChecked() == true) 
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
    titleText->setText(i18n(question.utf8()));
    
    //now we need to know where to put the correct answer
    correctis = (rand()%3)+1;
    //now we put it
    
    switch (correctis)
    {
        case 1:
            one_label->setText(i18n(answer.utf8()));
            two_label->setText(i18n(alternative1.utf8()));
            three_label->setText(i18n(alternative2.utf8()));
            break;
        case 2:
            two_label->setText(i18n(answer.utf8()));
            one_label->setText(i18n(alternative1.utf8()));
            three_label->setText(i18n(alternative2.utf8()));
            break;
        case 3:
            three_label->setText(i18n(answer.utf8()));
            two_label->setText(i18n(alternative1.utf8()));
            one_label->setText(i18n(alternative2.utf8()));
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
    if (one->isChecked() == false && two->isChecked()== false  && three->isChecked()== false )
    {
        KMessageBox::error ( this, i18n("You haven't selected a button") );
    }
    //ok, something is checked
    else
    {
        currentnr++;
        KProgress1->setProgress(currentnr);
        //if it has not been the last question
        if (currentnr != qnum)
        {
            i++;              //increase the current number
            one->setChecked( FALSE );
            two->setChecked( FALSE );
            three->setChecked( FALSE );
            setTexts();       //set the next questions
        }

        //ok, the quiz is over, show the results
        else
        {
            this->close();
            Quiz_Result *finalresults = new Quiz_Result();
            finalresults->setCaption(i18n("See your results!"));
            QObject::connect (finalresults->exit, SIGNAL(clicked()), finalresults, SLOT(hide()));
            QString resulttext;
            resulttext = i18n("You answered %1 of %2 questions correctly.").arg(QString::number(quizresult)).arg(qnum);
            finalresults->resultlabel->setText(i18n(resulttext.utf8()));
            finalresults->result_table->setNumRows( qnum+1 );
            finalresults->result_table->setNumCols( 3 );
            finalresults->result_table->setReadOnly( true );
            finalresults->result_table->setColumnStretchable( 0 , true );
            finalresults->result_table->setColumnStretchable( 1 , true );
            finalresults->result_table->setTopMargin( 0 );
            finalresults->result_table->setLeftMargin( 0 );
            QPixmap good = SmallIcon("button_ok");
            QPixmap bad  = SmallIcon("button_cancel");

            finalresults->result_table->setText( 0 , 0 , i18n("Questions:") );
            finalresults->result_table->setText( 0 , 1 , i18n("The correct answer was:") );
            finalresults->result_table->setText( 0 , 2 , i18n("You were:") );
            KSimpleConfig quizconfig (locate("data", "kalzium/kalziumrc"));
            for ( int i = 0 ; i < qnum ; i++ )
            {
                finalresults->result_table->setRowStretchable( i+1 , true );
                quizconfig.setGroup("q"+QString::number(order[i]));

                QString ques, answ;
                ques=quizconfig.readEntry("Q", "Unknown");
                answ=quizconfig.readEntry("A", "Unknown");

                QTableItem *item = new QTableItem(finalresults->result_table, QTableItem::Never, i18n(ques.utf8()));
                item->setWordWrap(true);
                finalresults->result_table->setItem( i+1 , 0 , item);
                item = new QTableItem(finalresults->result_table, QTableItem::Never, i18n(answ.utf8()));
                item->setWordWrap(true);
                finalresults->result_table->setItem( i+1 , 1 , item);

                if (wasCorrect( i ) == true)
                    finalresults->result_table->setPixmap( i+1 , 2 , good );
                else 
                    finalresults-> result_table->setPixmap( i+1 , 2 , bad  );
            }	    
        finalresults->show();
        }
    }
}

void QuizDlg::updateIt()
{
    update();
}

#include "quizdlg.moc"
