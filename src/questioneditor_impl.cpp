/***************************************************************************
 *   Copyright (C) 2003, 2004 by Carsten Niehaus                           *
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
#include "quizxmlparser.h"

#include <klistview.h>
#include <kurl.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <klocale.h>
#include <kstandarddirs.h>

#include <qfile.h>

questionEditorImpl::questionEditorImpl(QWidget* parent, const char* name)
 : questionEditor(parent,name)
{
	kdDebug() << "questionEditor()" << endl;
	KLV_qa->setSorting( -1 );
}

void questionEditorImpl::slotLoadFile()
{
	kdDebug() << "questionEditorImpl::slotLoadFile()" << endl;
	QuizXMLParser *parser = new QuizXMLParser();
	QDomDocument questionDocument;
	bool ok = parser->loadLayout( questionDocument );
	if ( ok )
		m_tasklist = parser->readTasks( questionDocument );
	setupListView();
}

void questionEditorImpl::slotRemoveQuestion()
{
	kdDebug() << "questionEditorImpl::slotRemoveQuestion()" << endl;
	QListViewItem *i = KLV_qa->selectedItem();

	if ( i )
	{
		if ( i->parent() )
			i = i->parent();
		m_tasklist.removeTaskAt( i->text( 0 ).toInt() );
	}
	setupListView();
}

void questionEditorImpl::setupListView()
{
	kdDebug() << "questionEditorImpl::setupListView()" << endl;
	TaskList t = m_tasklist; //just to be safe

	KLV_qa->clear();
	
	for ( int i = 0; i < t.numberOfTasks(); ++i )
	{
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

void questionEditorImpl::slotAddQuestion()
{
	questionAddDialogImpl *q = new questionAddDialogImpl( this);
	q->show();
	connect( q, SIGNAL(taskAdded(Task*) ), this, SLOT( slotAddTask(Task*) ) );
}

void questionEditorImpl::slotAddTask(Task* t)
{
	kdDebug() << "questionEditorImpl::slotAddTask(Task*)" << endl;
	
	m_tasklist.addTask( t );

	setupListView();
}

void questionEditorImpl::slotSaveFile()
{
	kdDebug() << "questionEditorImpl::slotSaveFile()" << endl;
	KURL url;
	url.setPath( locate("data", "kalzium/data/"));
	url = KFileDialog::getOpenURL( url.path(),
		QString("*xml") );
	
	QFile *layoutFile = new QFile( url.path() );
	QuizXMLWriter *writer = new QuizXMLWriter( layoutFile );

	for ( int i = 0; i < m_tasklist.numberOfTasks() ; ++i )
	{
		writer->addItem( m_tasklist.taskAt( i ) );
	}
}

#include "questioneditor_impl.moc"
