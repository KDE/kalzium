/***************************************************************************
    copyright            : (C) 2003, 2004 by Carsten Niehaus
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
#ifndef QUIZ_H
#define QUIZ_H

#include <qvaluelist.h>
#include <qwidget.h>

class Task;
class Answer;

typedef QValueList<Task*> taskList;
typedef QValueList<Answer*> answerList;

class Answer
{
	public:
		Answer( QString sentence, bool type )
		{
			m_sentence = sentence;
			m_type = type;
		};

		QString answer(){ return m_sentence; }

		bool isTrue(){ return m_type; }

	private:
		/*
		 * whether or not the sentence is true
		 */
		bool m_type;

		/*
		 * The text of the answer
		 */
		QString m_sentence;
};
		
class Task
{
	public:
		Task( QString question, 
				int grade );

		QString question() {return m_question; }
		
		/*
		 * the grade of the Task
		 */
		int m_iGrade;
		
		/*
		 * the true and false answers
		 */
		answerList m_answers;
		
		int numberOfAnswers();

		/*
		 * add the answer @param a to the list of answers
		 */
		void addAnswer( Answer *a );
		
		/*
		 * return the grade of the Task
		 */
		int isGrade(){ return m_iGrade; }

		/*
		 * the question of the task
		 */
		QString m_question;

		Answer* answerAt( int nr );
};
		

/*
 * A TaskList contains tasks which can be used in the quiz
 */
class TaskList
{
	public:
		TaskList();

		void addTask( Task *t );

		void deleteTaskList(){ Tasks.clear(); }

		int numberOfTasks(){ return Tasks.count(); }

		/*
		 * returns a list of tasks with grade @param grade
		 */
		taskList tasks( int grade );
		
		taskList tasks(){return Tasks;}

		Task* taskAt( int );
		
		taskList Tasks;
};

class Quiz
{
	public:
		/**
		 * Default Constructor
		 */
		Quiz();

};

class QuizWidget : public QWidget
{
	public:
		QuizWidget( Task *t, QWidget *parent = 0, const char *name = 0 );

	private:
		Task *m_pTask;

	protected:
		virtual void paintEvent( QPaintEvent* );
};
#endif // QUIZ_H

