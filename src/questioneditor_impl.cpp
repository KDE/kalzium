/***************************************************************************
 *   Copyright (C) 2003 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "questioneditor_impl.h"
#include "questionadddialog_impl.h"
#include "quiz.h"

#include <qfile.h>
#include <qstring.h>

#include <klistview.h>

#include <kmessagebox.h>
#include <kdebug.h>
#include <klocale.h>

questionEditorImpl::questionEditorImpl(QWidget* parent, const char* name)
 : questionEditor(parent,name)
{
	KLV_qa->setSorting( -1 );
	
	bool ok = loadLayout( m_questionDocument );
	if ( ok )
		ok = readTasks( m_questionDocument );
}

void questionEditorImpl::setupListView()
{
	TaskList t = m_tasklist; //just to be safe
	
	for ( int i = 0; i < t.numberOfTasks(); ++i )
	{
		kdDebug() << "Run # " << i << endl;
	 
		Task *task = t.taskAt( i );
		
		KListViewItem *item = new KListViewItem( KLV_qa, 
												QString::number( i+1 ),
												i18n( "Task" ),
												QString::number( task->isGrade()) );

		for ( int i = 0; i < task->numberOfAnswers(); ++i )
		{//iterate through all answers and determin if they are true or false
			KListViewItem *child = new KListViewItem( item );
			child->setText( 0, i18n( "Answer" ) );
			child->setText( 1, task->answerAt( i )->answer() );
			child->setText( 2, task->answerAt( i )->isTrue() ? i18n( "True" ) : i18n( "False" ) );
		}
		
		//add the question
		KListViewItem *childItemQuestion = new KListViewItem( item );
		childItemQuestion->setText( 0, i18n( "Question" ));
		childItemQuestion->setText( 1, task->question() );
		
	}
}	

void questionEditorImpl::addTaskToListView( Task * /*t*/ ){}

bool questionEditorImpl::loadLayout( QDomDocument &layoutDocument )
{
	QFile layoutFile( "/home/carsten/cvs/kdeedu/kalzium33/src/questions.xml" );

	if (!layoutFile.exists())
	{
		kdWarning() << "sounds.xml file not found in $KDEDIR/share/apps/klettres/data/" << endl;
		QString mString=i18n("The file questions.xml was not found in\n"
				"$KDEDIR/share/apps/klettres/data/\n\n"
				"Please install this file and start Kalzium again.\n\n");
		KMessageBox::information( this, mString,"Kalzium - Error" );
	}

	if (!layoutFile.open(IO_ReadOnly))
		return false;

	///Check if document is well-formed
	if (!layoutDocument.setContent(&layoutFile))
	{
		layoutFile.close();
		return false;
	}
	layoutFile.close();

	return true;
}

bool questionEditorImpl::readTasks( QDomDocument &questionDocument )
{
	QDomNodeList taskList,             //the list of all tasks
					questionList,
					answerList;

	QDomAttr textAttibute, 
				gradeAttribute, 
				trueAttribute;  //whether or not the answer is true

	QDomElement taskElement, //a single task
				questionElement,
				aElement;

	/*
	 * read in all tasks
	 */
	taskList = questionDocument.elementsByTagName( "task" );
	
	for ( uint i = 0; i < taskList.count(); ++i )
	{//iterate through all tasks

		taskElement = ( const QDomElement& ) taskList.item( i ).toElement();
		gradeAttribute = taskElement.attributeNode( "grade" );

		/*
		 * get the question
		 */
		questionList = taskElement.elementsByTagName( "question" );
		questionElement = ( const QDomElement& ) questionList.item( 0 ).toElement();
		const QString q = questionElement.attributeNode( "text" ).value();
		
		/*
		 * get the grade of the task
		 */
		const int g = gradeAttribute.value().toInt();

		/*
		 * get the answers. In most cases you will get three or for answers.
		 * Probably one will be true, the rest false
		 */
		answerList = taskElement.elementsByTagName( "answer" );

		/*
		 * get the number of answers
		 */
		int count = answerList.count();
		
		/*
		 * generate the task, the answers will be added inside the next loop
		 */
		Task *t = new Task( q, g );
		
		for ( int i = 0; i < count; ++i )
		{//iterate though all answers and generate the tasks
			bool answerisTrue;
			aElement = ( const QDomElement& ) answerList.item( i ).toElement();
			
			QString answerString = aElement.attributeNode( "text" ).value();

			/*
			 * check if the answer is true or not
			 */
			aElement.attributeNode( "type" ).value() == "true" ? answerisTrue = true : answerisTrue = false;
			
			Answer *answer = new Answer( answerString, answerisTrue );
			
			t->addAnswer( answer );
		}
		
		/*
		 * the task has now all answers, the grade and the question.
		 * Now we can add the Task to the list of tasks
		 */
		m_tasklist.addTask( t );
	}
		
	setupListView();		
	
	return true;
}

void questionEditorImpl::slotAddQuestion()
{
	questionAddDialogImpl *q = new questionAddDialogImpl(this);
	q->show();
}


#include "questioneditor_impl.moc"
