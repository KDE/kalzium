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

#include <klistview.h>

#include <kmessagebox.h>
#include <kdebug.h>
#include <klocale.h>
#include <kstandarddirs.h>

questionEditorImpl::questionEditorImpl(QWidget* parent, const char* name)
 : questionEditor(parent,name)
{
	kdDebug() << "questionEditor()" << endl;
	KLV_qa->setSorting( -1 );
}

void questionEditorImpl::setupListView()
{
	TaskList t = m_tasklist; //just to be safe
	
	kdDebug() << "#: " << t.numberOfTasks() << endl;
	
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

void questionEditorImpl::slotAddQuestion()
{
	questionAddDialogImpl *q = new questionAddDialogImpl(this);
	q->show();
}


#include "questioneditor_impl.moc"
