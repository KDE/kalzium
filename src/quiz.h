/* **************************************************************************
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
#ifndef QUIZ_H
#define QUIZ_H

#include <qwidget.h>

class Task;
class Answer;

typedef QValueList<Task*> taskList;
typedef QValueList<Answer*> answerList;

/**
 * This class represents the answer of a quiz. A answer can 
 * be either true or false.
 * @author Carsten Niehaus
 * @short An answers is one of several possible answers of a task.
 * @version 1.1
 */ 
class Answer
{
	public:
		/**
		 * Default Contructor. Creates an Answer. The answer
		 * has the text @p sentence. @p type defines if the 
		 * the answer is true or false.
		 * @param sentence is the text of the answer
		 * @param type defines if the answer is @p true or @p false
		 */
		Answer( QString sentence, bool type )
		{
			m_sentence = sentence;
			m_type = type;
		};

		/**
		 * @return the text of the answer
		 */
		QString answer(){ 
			return m_sentence; 
		}
		
		/**
		 * sets the answers text of to @p answer
		 */
		void setAnswer( QString &answer ){ 
			m_sentence = answer; 
		}

		/**
		 * @return if the answer was true or false
		 */
		bool isTrue(){
			return m_type;
		}

	private:
		/**
		 * whether or not the sentence is true
		 */
		bool m_type;

		/**
		 * The text of the answer
		 */
		QString m_sentence;
};
		
/**
 * This class represents a task of a quiz. A task consists of
 * a question and at least two answers. 
 * @author Carsten Niehaus
 * @short A task is a questions plus at least two answers
 * @version 1.1
 */ 
class Task
{
	public:
		/**
		 * The Contructor of this class. Creates a task for the quiz with
		 * the difficulty @p grade. A Task consists of one @p question and at least two answers.
		 * @param question is the text of the question
		 * @param grade defines how difficult the task is. Values from 1 to 3 are allowed.
		 *
		 * @see Answer
		 */
		Task( QString question, 
				int grade );

		/**
		 * @return the question of the task
		 */
		QString question() const{
			return m_question;
		}
		
		/**
		 * @return the number of answers the task owns
		 */
		int numberOfAnswers();

		/**
		 * add the answer @p a to the list of answers
		 * @param a The Answer will be added the the list of answers
		 */
		void addAnswer( Answer *a );
		
		/**
		 * @return the grade of the Task
		 */
		int isGrade() const{ 
			return m_iGrade;
		}

		/**
		 * @return the answer at position @p nr
		 * @param nr is the number of the answer, starting with 0
		 */
		Answer* answerAt( int nr );
	
	private:	
		/**
		 * the grade of the Task
		 */
		int m_iGrade;
		
		/**
		 * this list contains the answers of this task
		 */
		answerList m_answers;
		
		/**
		 * the question of the task
		 */
		QString m_question;
};
		

/**
 * @short A TaskList contains tasks which can be used in the quiz
 * @author Carsten Niehaus
 */
class TaskList
{
	public:
		TaskList();

		/**
		 * add the Task @p t to the tasklist
		 * @param t the Task which will be addet the the TaskList
		 */
		void addTask( Task *t ){
			Tasks.append( t );
		}

		/**
		 * removes all tasks for the internal list
		 */
		void deleteTaskList(){
			Tasks.clear();
		}

		/**
		 * @return the number of tasks
		 */
		int numberOfTasks(){
			return Tasks.count(); 
		}

		/**
		 * @param grade The grade which is queried
		 * @return a list of tasks with grade @p grade
		 */
		taskList tasks( int grade );
		
		/**
		 * @return all tasks
		 */
		taskList tasks(){ 
			return Tasks;
		}

		/**
		 * @param pos The position of the queried task
		 * @return the task at position @p pos
		 */
		Task* taskAt( int pos );

	private:
		
		/**
		 * the internal list of tasks
		 */
		taskList Tasks;
};

/**
 * A quiz represents the rules and the set of tasks. It can be 
 * compared with a manual of a game plus the cards of it.
 * @author Carsten Niehaus
 * @version 1.1
 * @short A quiz represents the rules and the set of tasks.
 */ 
class Quiz
{
	public:
		/**
		 * Default Contructor. Create a Quiz with @p numOfTasks Tasks 
		 * @see Task
		 * @see Answer
		 */
		Quiz( int numOfTasks );

		/**
		 * @return the number of points the player get per grade
		 * of the task if he answers it correctly
		 */
		int pointsPerTask() const {
			return m_pointsPerTask;
		}
		
		/**
		 * @return the number of seconds the player has to
		 * answer the task
		 */
		int secPerTask() const {
			return m_secPerTask;
		}

		/**
		 * sets the number of seconds @p sec for the quiz
		 */
		void setNumberOfSeconds( int sec );
		
		/**
		 * sets the number of points per grade to @p points
		 */
		void setPointsPerTask( int points );

		/**
		 * @return the number of tasks in the quiz
		 */
		int numOfTasks() const{
			return m_numTasks;
		}
	private:
		/**
		 * number of seconds the player has per task
		 */
		int m_secPerTask;

		/**
		 * creates the list of tasks for the quiz
		 */
		void generateTasks();

		/**
		 * number of points per correctly answered task and grade
		 * of the task. 15 would mean that a Grade-3 task would
		 * give the player 15*3 = 45 points
		 */
		int m_pointsPerTask;
		
		int m_numTasks;
};

/**
 * The QuizMaster controls the quiz. This class takes care
 * of the points, when the quiz stars and ends, how many
 * tasks have been answered correctly and so on.
 * @author Carsten Niehaus
 * @short A QuizMaster controls a quiz
 * @version 1.0
 */
class QuizMaster
{
	public:
		/**
		 * Default Contructor. Create a QuizMaster-Object which controls a Quiz
		 * @param q the Quiz with all settings
		 */
		QuizMaster( Quiz *q );

		/**
		 * this methods gets all the settings for the quiz, 
		 * generates the GUI and finally starts the quiz
		 */
		void startQuiz();

		/**
		 * generate the TaskList with @p num tasks for the quiz
		 */
		void generateTaskList( int num );
		
	private:

		Quiz *m_quiz;
		
		/**
		 * the number of points the player currently has
		 */
		int m_points;

		TaskList m_tasklist;
		
};

#endif // QUIZ_H

