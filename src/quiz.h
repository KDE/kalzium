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

//#include <qwidget.h>

class Task;
class Answer;
class QuizWidgetImpl;

#include <qvaluelist.h>
#include <qdom.h>
#include <qobject.h>

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
		 * the difficulty @p grade. A Task consists of one @p question and at least two
		 * and a maximum of four answers.
		 * @param question is the text of the question
		 * @param grade defines how difficult the task is. Values from 1 to 3 are allowed.
		 *
		 * @see Answer
		 */
		Task( QString question, 
				int grade );

		Task(){};

		/**
		 * @return the question of the task
		 */
		QString question() const{
			return m_question;
		}
		
		/**
		 * @return the number of answers the task owns
		 */
		int numberOfAnswers(){
			return m_answers.count();
		}

		/**
		 * add the answer @p a to the list of answers
		 * @param a The Answer will be added the the list of answers
		 */
		void addAnswer( Answer *a ){
			m_answers.append( a );
		}
		
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

		Answer* correctAnswer();
	
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
		 * cuts the list down to @p num tasks
		 * @param num the number of tasks for the quiz
		 */
		void cutList( int num );

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

		/**
		 * removes the Task at position @p pos
		 * @param pos the position of the Task which will be removed
		 */
		void removeTaskAt( int pos );

		/**
		 * randomizes the list of tasks.
		 * @see KRandomSequence
		 */
		void randomize();

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
		 * Default Contructor. Creates a Quiz.
		 * @param numOfTasks the number of tasks
		 * @param min the easiest grade
		 * @param max the most difficult grade
		 * @param secPerTask number of seconds per task 
		 * @param points the number of points per grade and task
		 * @see Task
		 * @see Answer
		 */
		Quiz( int numOfTasks , int min, int max, int secPerTask, int points ){
			m_numTasks = numOfTasks;
			m_minGrade = min;
			m_maxGrade = max;
			m_secPerTask = secPerTask;
			m_pointsPerTask = points;
		}

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
		void setNumberOfSeconds( int sec ){
			m_secPerTask = sec;
		}
		
		/**
		 * sets the number of points per grade to @p points
		 */
		void setPointsPerTask( int points ){
			m_pointsPerTask = points;
		}

		/**
		 * @return the number of tasks in the quiz
		 */
		int numOfTasks() const{
			return m_numTasks;
		}

		/**
		 * @return the minimum grade of the quiz
		 */
		int minGrade() const{
			return m_minGrade;
		}
		
		/**
		 * @return the minimum grade of the quiz
		 */
		int maxGrade() const{
			return m_maxGrade;
		}
	private:
		/**
		 * number of seconds the player has per task
		 */
		int m_secPerTask;

		/**
		 * the easiest grade of the quiz
		 */
		int m_minGrade;
		
		/**
		 * the most difficult grade of the quiz
		 */
		int m_maxGrade;

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
		
		/**
		 * num of tasks in the quiz
		 */
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
class QuizMaster : public QObject
{
	Q_OBJECT

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
		QDomDocument m_questionDocument;

		
		Quiz *m_quiz;
		Task *m_currentTask;

		QuizWidgetImpl *m_quizwidget;
		
		/**
		 * the number of points the player currently has
		 */
		int m_points;

		TaskList m_tasklist;

		taskList t;

		/**
		 * the number of the task which is the current Task. It
		 * starts with 1 for the first Task and ends with numTotal
		 */
		int numberOfTask;
		
		/**
		 * the number of tasks in the Quiz
		 */
		int numTotal;
		
		/**
		 * the number of correctly answered tasks
		 */
		int numCorrect;
		
		/**
		 * the number of wrong answered tasks
		 */
		int numWrong;

		/**
		 * updates the number at the bottom of QuizWidgetImpl
		 * @param correct is true when the answer was correct
		 */
		void updateNumbers( bool correct );

		bool m_answerWasCorrect;

		/**
		 * if true the values will be displayed as percentage and not as absolute values
		 */
		bool m_percentage;

		/**
		 * stop the quiz
		 */
		void endQuiz();

	private slots:
		/**
		 * updates the points of the user
		 */
		void slotUpdatePoints(bool correct,int grade);

		/**
		 * this starts several things:
		 * - update the points: updatePoints(bool);
		 * - display the "now previous" task and the correct answer: displayPrevious()
		 * - update the numbers on the bottom of the QuizWidgetImpl
		 */
		void slotNextTask();
		
};

#endif // QUIZ_H

