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
	Task *m_currentTask = new Task();
	
	buttonList << btn1 << btn2 << btn3 << btn4;
	setupPixmaps();
	setRadiobuttons();
}

void QuizWidgetImpl::setupPixmaps()
{
	//TODO I have no clue why this doesn't work. The file question.png
	//exists. So the path is wrong...
	picQuestion->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picAnswer->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picPrevious->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picYourAnswer->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picCorrectAnswer->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picCount->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picAnswered->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picCorrect->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
	picError->setPixmap( locate( "data", "kalzium/icons/crystalsvg/16x16/actions/question.png" ) );
}

void QuizWidgetImpl::slotAnswered(int q )
{
	//check if correct
	kdDebug() << "button " << q << "  has been pressed!" << endl;
	if ( m_currentTask->answerAt( q )->isTrue() )
		emit( AnswerIs( true, m_currentTask->isGrade() ) );
	else
		emit( AnswerIs( false, m_currentTask->isGrade() ) );
}
	
void QuizWidgetImpl::setRadiobuttons()
{
	kdDebug() << "QuizWidgetImpl::setRadiobuttons()" << endl;
	RadioList::iterator it = buttonList.begin();
	for ( ; it != buttonList.end(); ++it )
	{//first, hide all buttons
		(*it)->hide();
		kdDebug() << ( *it )->text() << endl;
	}
	
	int numOfAnswers = m_currentTask->numberOfAnswers();
	
	it = buttonList.begin();
	
	for ( int i = 0 ; i < numOfAnswers ; ++i )
	{//now diplay the needed buttons (one per answer) and set the texts

		kdDebug() << ( *it )->text() << endl;
		(*it)->show();
		(*it)->setText(m_currentTask->answerAt( i )->answer());
		++it;
	}
	kdDebug() << "End of QuizWidgetImpl::setRadiobuttons()" << endl;
}

void QuizWidgetImpl::setTask( Task *t )
{
	kdDebug() << "QuizWidgetImpl::setTask()" << endl;
	m_currentTask = t;

	lblQuestion->setText( m_currentTask->question() );
	setRadiobuttons();
}

void QuizWidgetImpl::slotSetupWidgets()
{
	kdDebug() << "QuizWidgetImpl::slotSetupWidgets()" << endl;

}

#include "quizwidget_impl.moc"
