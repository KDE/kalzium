/***************************************************************************
    copyright            : (C) 2004 by Carsten Niehaus
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
#include "quizwidget_impl.h"
#include "quiz.h"

#include <qlabel.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qlayout.h>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>

QuizWidgetImpl::QuizWidgetImpl( QWidget *parent, const char* name )
	: QuizWidget( parent, name )
{
	kdDebug() << "QuizWidget()" << endl;
	
	connect( QuestionGroup, SIGNAL( clicked(int )), this, SLOT( slotAnswered( int )) );
	m_currentTask = new Task();
	
	buttonList << btn1 << btn2 << btn3 << btn4;
	setupPixmaps();
}

void QuizWidgetImpl::setupPixmaps()
{
	picQuestion->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picAnswer->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picPrevious->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picYourAnswer->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picCorrectAnswer->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/correct.png" ) );
	picCorrect->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/correct.png" ) );
	picCount->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picAnswered->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picError->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/error.png" ) );
}

//TODO
void QuizWidgetImpl::displayPrevious()
{
	kdDebug() << "QuizWidgetImpl::displayPrevious()" << endl;

	lblPreviousQuestion->setText( m_OldQuestion );
	lblYourAnswer->setText( m_UsersAnswer );
	lblCorrect->setText( m_CorrectAnswer );
}

void QuizWidgetImpl::slotAnswered(int q )
{
	kdDebug() << "QuizWidgetImpl::slotAnswered()" << endl;
	kdDebug() << "button " << q << "  has been pressed!" << endl;

	Answer *a =  m_currentTask->answerAt( q );
	m_UsersAnswer = a->answer();
	m_CorrectAnswer = m_currentTask->correctAnswer()->answer();
	m_OldQuestion = m_currentTask->question();
	displayPrevious();

	if ( a->isTrue() )
	{
		kdDebug() << "The answer was CORRECT" << endl;
		emit( AnswerIs( true, m_currentTask->isGrade() ) );
	}
	else
	{
		kdDebug() << "The answer was WRONG" << endl;
		emit( AnswerIs( false, m_currentTask->isGrade() ) );
	}
}
	
void QuizWidgetImpl::setRadiobuttons()
{
	kdDebug() << "QuizWidgetImpl::setRadiobuttons()" << endl;
	RadioList::iterator it = buttonList.begin();
	for ( ; it != buttonList.end(); ++it )
	{//first, hide all buttons
		(*it)->hide();
		(*it)->setChecked(false);
		kdDebug() << ( *it )->text() << endl;
	}
	
	int numOfAnswers = m_currentTask->numberOfAnswers();
	
	it = buttonList.begin();

	kdDebug() << "num of answers: " << numOfAnswers << endl;
	
	for ( int i = 0 ; i < numOfAnswers ; ++i )
	{//now diplay the needed buttons (one per answer) and set the texts

		kdDebug() << "answer " << i << " text: " << ( *it )->text() << endl;
		(*it)->show();
		(*it)->setText(m_currentTask->answerAt( i )->answer());
		++it;
	}
	kdDebug() << "End of QuizWidgetImpl::setRadiobuttons()" << endl;
}

void QuizWidgetImpl::setTask( Task *t )
{
	kdDebug() << "QuizWidgetImpl::setTask() 1" << endl;
	m_currentTask = t;

	kdDebug() << "QuizWidgetImpl::setTask() 2" << endl;
	lblQuestion->setText( m_currentTask->question() );
	kdDebug() << "QuizWidgetImpl::setTask() 3" << endl;
	setRadiobuttons();
}

void QuizWidgetImpl::slotSetupWidgets()
{
	kdDebug() << "QuizWidgetImpl::slotSetupWidgets()" << endl;

}

#include "quizwidget_impl.moc"
