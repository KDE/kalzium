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

#include <qpainter.h>
#include <qwidget.h>

Task::Task( QString question, 
			int grade)
{
	m_question = question;
	m_iGrade = grade;
}

void Task::addAnswer( Answer *a )
{
	m_answers.append( a );
}	

int Task::numberOfAnswers()
{
	return m_answers.count();
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
	/*
	 * XXX this can probably be done with less code
	 */
	const taskList::iterator it = Tasks.at( pos );
	return *it;
}

/*
 * returns a list of tasks with grade @param grade
 */
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
	
Quiz::Quiz( int numOfTasks )
{
}

