#ifndef QUIZWIDGET_IMPL_H
#define QUIZWIDGET_IMPL_H
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

#include <quizwidget.h>
class Task;
class QRadioButton;

typedef QValueList<QRadioButton*> RadioList;


/**
 * This class represents the answer of a quiz. A answer can 
 * be either true or false.
 * @author Carsten Niehaus
 * @short An answers is one of several possible answers of a task.
 * @version 1.1
 */ 
class QuizWidgetImpl : public QuizWidget
{
	Q_OBJECT

	public:
		QuizWidgetImpl( QWidget *parent, const char* name = 0 );

		void setTask( Task *t );
		
		void setPreviousTask( Task *t );

		void setRadiobuttons();

	private:
		RadioList buttonList;

		Task *m_currentTask;

		void setupPixmaps();

	public slots:
		void slotSetupWidgets();

		void slotAnswered( int q );

	signals:
		void newTask();

		void AnswerIs( bool , int grade);
};
#endif // QUIZWIDGET_IMPL_H
