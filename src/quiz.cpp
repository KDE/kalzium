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

void TaskList::addTask( Task *t )
{
	Tasks.append( t );
}

Task* TaskList::taskAt( int i )
{
	/*
	 * XXX this can probably be done with less code
	 */
	const taskList::iterator it = Tasks.at( i );
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
	
Quiz::Quiz()
{
}

QuizWidget::QuizWidget( Task *t, QWidget *parent, const char *name ) 
	: QWidget( parent, name )
{
	m_pTask = t;
}

void QuizWidget::paintEvent( QPaintEvent* /*e*/ )
{ 

}
