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

/**
 * This class represents the answer of a quiz. A answer can 
 * be either true or false.
 * @param sentence is the text of the answer
 * @param type defines if the answer is @p true or @p false
 * @author Carsten Niehaus
 * @version 1.1
 */ 
class Answer
{
	public:
		Answer( QString sentence, bool type )
		{
			m_sentence = sentence;
			m_type = type;
		};

		/**
		 * @return the text of the answer
		 */
		QString answer(){ return m_sentence; }
		
		void setAnswer( QString answer ){ m_sentence = answer; }


		/**
		 * @return if the answer was true or false
		 */
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
		
/**
 * @short A task is a questions plus at least two answers
 * This class represents a task of a quiz. A task consists of
 * a question and at least two answers. 
 * @param question is the text of the question
 * @param grade defines how difficult the task is. Values from 1 to 5 or allowed.
 * @author Carsten Niehaus
 * @version 1.1
 * @see Answer
 */ 
class Task
{
	public:
		Task( QString question, 
				int grade );

		/**
		 * @return the question of the task
		 */
		QString question() {return m_question; }
		
		/**
		 * the grade of the Task
		 */
		int m_iGrade;
		
		/**
		 * this list contains the answers of this task
		 */
		answerList m_answers;
		
		/**
		 * @return the number of answers the task owns
		 */
		int numberOfAnswers();

		/**
		 * add the answer @p a to the list of answers
		 * @param a will be added the the list of answers
		 */
		void addAnswer( Answer *a );
		
		/**
		 * @return the grade of the Task
		 */
		int isGrade(){ return m_iGrade; }

		/**
		 * the question of the task
		 */
		QString m_question;

		/**
		 * @return the answer at position @p nr
		 * @param nr is the number of the answer, starting with 0
		 */
		Answer* answerAt( int nr );
};
		

/**
 * A TaskList contains tasks which can be used in the quiz
 */
class TaskList
{
	public:
		TaskList();

		void addTask( Task *t );

		void deleteTaskList(){ Tasks.clear(); }

		int numberOfTasks(){ return Tasks.count(); }

		/**
		 * @return a list of tasks with grade @param grade
		 */
		taskList tasks( int grade );
		
		/**
		 * @return all tasks
		 */
		taskList tasks(){ return Tasks; }

		/**
		 * @return the task at position @p pos
		 */
		Task* taskAt( int pos );
		
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

