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
#include <kpushbutton.h>
#include <ksimpleconfig.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstddirs.h>

//QT-Includes
#include <qlabel.h>
#include <qstring.h>
#include <qradiobutton.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qtable.h>
#include <qimage.h>
#include <qwidget.h>
#include <qpixmap.h>
#include <qbuttongroup.h>

// Standard C++ includes
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <time.h>

#include "quizdlg.h"
#include "quizdlg.moc"

QuizDlg::QuizDlg (QWidget *parent, const char *name, int numofquestions )  : KDialog (parent,name)
{
    qnum = numofquestions;

    srandom( time( 0 ) );
    for (int e = 0 ; e < 61 ; e++ )
    {
	order[e] = e+1;
    }

    int a, b, temp;

    for (int r = 0 ; r < 50 ; r++ )
    {
	b = rand()%61;
	do
	{
	    a = rand()%61;
	}while (b == a);

	temp = order[a];
	order[a] = order[b];
	order[b] = temp;
    }

    quizresult = 0;
    currentnr = 0;
    i = 0;

    //a grid lets resize nicely the dialog
    QGridLayout *grid = new QGridLayout( this);
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    //put the label and buttongroup vertically
    QVBoxLayout *vert = new QVBoxLayout;
    vert->setSpacing( 6 );
    vert->setMargin( 0 );

    titleText = new QLabel ( this );
    titleText->setFont(QFont("helvetica", 20, QFont::Bold));
    vert->addWidget(titleText);

    QButtonGroup *bgroup = new QButtonGroup ( this );
    QWhatsThis::add(bgroup, i18n("Only one of these answers is correct."));
    bgroup->setColumnLayout(0, Qt::Vertical );
    bgroup->layout()->setSpacing( 0 );
    bgroup->layout()->setMargin( 0 );

    //vertical layout for the radiobuttons
    QVBoxLayout *gbLayout = new QVBoxLayout( bgroup->layout() );
    gbLayout->setAlignment( Qt::AlignTop );
    gbLayout->setSpacing( 6 );
    gbLayout->setMargin( 11 );

    connect( bgroup, SIGNAL(clicked(int)), SLOT(updateIt(int)) );

    one = new QRadioButton ( bgroup );
    one->setChecked( FALSE );

    two = new QRadioButton (  bgroup );
    two->setChecked( FALSE );

    three = new QRadioButton ( bgroup );
    three->setChecked( FALSE );

    gbLayout->addWidget( one );
    gbLayout->addWidget( two );
    gbLayout->addWidget( three );

    vert->addWidget(bgroup);

    grid->addLayout( vert, 0, 0 );

    bgroup->show();

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setSpacing( 6 );
    hlayout->setMargin( 0 );
    KPushButton *confirm = new KPushButton (i18n("&Accept") , this );
    confirm->setFocus();
    QWhatsThis::add(confirm, i18n("If you click on this button the next question will be asked."));
    connect( confirm, SIGNAL(clicked() ), this, SLOT( slotCheck() ) );
    hlayout->addWidget( confirm );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    hlayout->addItem( spacer );
    vert->addLayout( hlayout );   
    setTexts();
    this->show();
}


void QuizDlg::updateIt( int index )
{
    update();
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
            KDialog *result = new KDialog ( this, "result" );
            result->setCaption(i18n("See your results!"));

            QGridLayout *grid = new QGridLayout(result , 2 , 0 , 8 );

            KPushButton *exit = new KPushButton( i18n("&Close"), result );
            QObject::connect (exit, SIGNAL(clicked()), result, SLOT(hide()));
            QWhatsThis::add(exit, i18n("If you click on this button this dialog will close."));
            grid->addWidget(exit,2,0);

            QLabel *resultlabel = new QLabel (result);
            QString resulttext;
            resulttext = i18n("You answered %1 of %2 questions correctly.").arg(QString::number(quizresult)).arg(qnum); //FIXME This text does not appear
            resultlabel->setText(i18n(resulttext.utf8()));
            grid->addWidget( resultlabel, 0 , 0 );


//new Table - BEGIN

	    QTable *resultTable = new QTable( qnum+1 , 3 , result , "resultTable" );
	    resultTable->setReadOnly( true );
	    resultTable->setColumnStretchable( 0 , true );
	    resultTable->setColumnStretchable( 1 , true );
	    resultTable->adjustColumn( 0 );
	    resultTable->adjustColumn( 1 );
            grid->addWidget( resultTable, 1 , 0 );

	    QPixmap good = SmallIcon("apply.png");
	    QPixmap bad  = SmallIcon("cancel.png");

	    resultTable->setText( 0 , 0 , i18n("Questions:") );
	    resultTable->setText( 0 , 1 , i18n("The correct answer was:") );
	    resultTable->setText( 0 , 2 , i18n("You have been:") );

	    resultTable->horizontalHeader()->hide();
	    resultTable->verticalHeader()->hide();
	    resultTable->setShowGrid( false );

	    KSimpleConfig quizconfig (locate("data", "kalzium/kalziumrc"));
	    for ( int i = 0 ; i < qnum ; i++ )
	    {
		quizconfig.setGroup("q"+QString::number(order[i]));
		
		QString ques, answ;
		ques=quizconfig.readEntry("Q", "Unknown");
		answ=quizconfig.readEntry("A", "Unknown");
		
		resultTable->setItem( i+1 , 0 , new QTableItem( resultTable, QTableItem::Never , ques ) );
		resultTable->setItem( i+1 , 1 , new QTableItem( resultTable, QTableItem::Never , answ ) );
		
		if (wasCorrect( i ) == true)
		{
		    resultTable->setPixmap( i+1 , 2 , good );
		}
		else resultTable->setPixmap( i+1 , 2 , bad );
	    }
	    

//new Table - END
	    
            result->show();
        }
    }
}

bool QuizDlg::wasCorrect( int i )
{
    if (QuestioniWasCorrect[i] == true) return TRUE;
    return FALSE;
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
            one->setText(i18n(answer.utf8()));
            two->setText(i18n(alternative1.utf8()));
            three->setText(i18n(alternative2.utf8()));
            break;
        case 2:
            two->setText(i18n(answer.utf8()));
            one->setText(i18n(alternative1.utf8()));
            three->setText(i18n(alternative2.utf8()));
            break;
        case 3:
            three->setText(i18n(answer.utf8()));
            two->setText(i18n(alternative1.utf8()));
            one->setText(i18n(alternative2.utf8()));
            break;
    }
}
