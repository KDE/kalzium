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
#include "quiz.h"
#include "quizxmlparser.h"
#include "quizwidget_impl.h"

#include "krandomsequence.h"
#include <kdebug.h>
#include <qptrlist.h>
#include <qlabel.h>


Task::Task( QString question, 
			int grade)
{
	m_question = question;
	m_iGrade = grade;
}

Answer* Task::answerAt( int nr )
{
	const answerList::iterator it = m_answers.at( nr );
	return *it;
}


TaskList::TaskList()
{
}

Task* TaskList::taskAt( int pos )
{
	kdDebug() << "TaskList::taskAt()" << endl;
	
	const taskList::iterator it = Tasks.at( pos );
	return *it;
}

void TaskList::randomize()
{
	kdDebug() << "TaskList::randomize()" << endl;
	KRandomSequence seq;
	QPtrList<Task> tList;

	taskList::iterator it = Tasks.begin();
	for ( ; it != Tasks.end() ; ++it )
	{
		tList.append( *it );
	}

	seq.randomize( &tList );

	Tasks.clear();

	Task* task;
	for ( task=tList.first() ; task; task = tList.next() )
	{
		Tasks.append( task );
	}
}

void TaskList::cutList( int num )
{
	kdDebug() << "TaskList::cutList()" << endl;

	taskList t;
	taskList::iterator it = Tasks.begin();
	for ( ; it != Tasks.at( num ) ; ++it )
	{
		t.append( ( *it ) );
	}
	Tasks = t;
}
	
taskList TaskList::tasks( int grade )
{
	taskList newList;

	taskList::iterator it;
	for ( it = Tasks.begin() ; it != Tasks.end(); ++it )
	{
		if ( (*it)->isGrade() == grade )
		{
			newList.append( *it );
		}
	}

	return newList;
}

/////////////////////////////////////////////////////////////////
	
QuizMaster::QuizMaster( Quiz *q )
	: QObject()
{
	m_quiz = q;

	m_currentTask = new Task();
}

void QuizMaster::generateTaskList( int num )
{
	kdDebug() << "QuizMaster::generateTaskList()" << endl;
	
	QuizXMLParser *parser = new QuizXMLParser();
	
	bool ok = parser->loadLayout( m_questionDocument );
	if ( ok )
		m_tasklist = parser->readTasks( m_quiz->minGrade(), m_quiz->maxGrade(), m_questionDocument );

	m_tasklist.randomize();

	m_tasklist.cutList( num );

	//I copy the list to have the complete list at the end of the game
	m_completeListOfTasks = m_tasklist;
}

void QuizMaster::startQuiz()
{
	kdDebug() << "QuizMaster::startQuiz()" << endl;
	generateTaskList( m_quiz->numOfTasks() );
	
	//The list is now completly ready: random and correct
	//number of tasks with the correct grade

	m_quizwidget = new QuizWidgetImpl( 0L , "quizwidgetimpl" );
	m_quizwidget->show();
	connect( m_quizwidget, SIGNAL( AnswerIs(bool,int) ), this, SLOT( slotUpdatePoints(bool,int) ) );

	taskList::iterator it = m_tasklist.tasks().begin();
	m_quizwidget->setTask( *it );


//X 	for ( ; it != m_tasklist.tasks().end() ; ++it )
//X 	{
//X 		m_currentTask = ( *it );
//X 		m_quizwidget->setTask( (*it) );
//X 	}
	
}

void QuizMaster::slotUpdateNumbers()
{
	kdDebug() << "QuizMaster::slotUpdateNumbers()" << endl;
	int totalnumber = numCorrect + numWrong;
	
	QString total;
	QString correct;
	QString wrong;
	
	if ( m_percentage )
	{
		total = QString::number( (int)( numTotal/totalnumber ) );
		correct = QString::number( (int)( numTotal/numCorrect ) );
		wrong = QString::number( ( int )numTotal/numWrong );
	}else
	{
		total = QString::number( totalnumber );
		correct = QString::number( numCorrect );
		wrong = QString::number( numWrong );
	}
	
	m_quizwidget->lblScoreCount->setText( QString::number(numTotal) );
	m_quizwidget->lblScoreAnswered->setText( total );
	m_quizwidget->lblScoreCorrect->setText( correct );
	m_quizwidget->lblScoreError->setText( wrong );
}

void QuizMaster::displayPrevious( Task *t )
{
	kdDebug() << "QuizWidgetImpl::displayPrevious()" << endl;

	m_quizwidget->lblPreviousQuestion->setText( t->question() );
}

void QuizMaster::slotNextTask()
{
	kdDebug() << "QuizMaster::slotNextTask()" << endl;

	numberOfTask++;

//TODO
//	displayPrevious(m_currentTask);
		
	taskList::iterator it = m_tasklist.tasks().at( numberOfTask );

	kdDebug() << "hier bin ich noch" << endl;
	m_quizwidget->setTask( *it );
	kdDebug() << "nach dem Task hier bin ich noch" << endl;
}

bool QuizMaster::checkAnswer()
{
	//todo
	return true;
}
