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

Answer* Task::correctAnswer()
{
	answerList::iterator it = m_answers.begin();
	for ( ; it != m_answers.end() ; ++it )
	{
		if ( (*it)->isTrue() )
			return *it;
	}
}

Answer* Task::answerAt( int nr )
{
	kdDebug() << "Task::answerAt()" << endl;
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

void TaskList::removeTaskAt( int pos )
{
	pos--; //the list starts with 0 and not 1
	taskList::iterator ite = Tasks.at( pos );
	Tasks.remove( Tasks.at( pos ) );
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
	kdDebug() << "QObject()" << endl;
	m_quiz = q;

	m_currentTask = new Task();

	numCorrect = 0;
	numWrong = 0;
	numberOfTask = 1;

	kdDebug() << "Quiz:number of tasks: " << q->numOfTasks() << endl;
	kdDebug() << "Quiz:number of tasks:  " << m_quiz->numOfTasks() << endl;
	numTotal = q->numOfTasks();
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

	t = m_tasklist.tasks();
}

void QuizMaster::startQuiz()
{
	kdDebug() << "QuizMaster::startQuiz()" << endl;
	generateTaskList( m_quiz->numOfTasks() );
	
	//The list is now completly ready: random and correct
	//number of tasks with the correct grade

	m_quizwidget = new QuizWidgetImpl( 0L , "quizwidgetimpl" );
	m_quizwidget->lblScoreCount->setText( QString::number(numTotal) );
	m_quizwidget->show();
	connect( m_quizwidget, SIGNAL( AnswerIs(bool,int) ), this, SLOT( slotUpdatePoints(bool,int) ) );

	taskList::iterator it = t.begin();
	m_quizwidget->setTask( *it );
}
		

void QuizMaster::slotUpdatePoints(bool correct,int grade)
{
	kdDebug() << "QuizMaster::slotUpdatePoints()" << endl;
	if ( correct ){
		m_answerWasCorrect = true;
		int p = grade * m_quiz->pointsPerTask();
		m_points+=p;
	}else{
		m_answerWasCorrect = false;

	}
	updateNumbers( correct );
}

void QuizMaster::updateNumbers( bool c )
{
	kdDebug() << "QuizMaster::updateNumbers()" << endl;
	
	if ( c )
		numCorrect++;
	else
		numWrong++;
	
	QString total;
	QString correct;
	QString wrong;
	
	int tot = numCorrect + numWrong;
	
	total = QString::number( tot );
	correct = QString::number( numCorrect );
	wrong = QString::number( numWrong );
	
	m_quizwidget->lblScoreCount->setText( QString::number(numTotal) );
	m_quizwidget->lblScoreAnswered->setText( total );
	m_quizwidget->lblScoreCorrect->setText( correct );
	m_quizwidget->lblScoreError->setText( wrong );
		
	slotNextTask();
}

void QuizMaster::slotNextTask()
{
	kdDebug() << "QuizMaster::slotNextTask()" << endl;

	kdDebug() << "ANZAHL AN TASKS: " << t.count() << endl;
	kdDebug() << "numberOfTask:    " << numberOfTask << endl;
	if ( numTotal == numberOfTask )
	{
		endQuiz();
		return;
	}

	taskList::iterator it = t.at( numberOfTask );
	numberOfTask++;

	//TODO
	//	displayPrevious();

	kdDebug() << "NEUE FRAGE: " << ( *it )->question() << endl;
	m_quizwidget->setTask( *it );
}

//TODO
void QuizMaster::endQuiz()
{
	kdDebug() << "QuizMaster::endQuiz()" << endl;
	m_quizwidget->close();	
}

#include "quiz.moc"
