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
#include "questionadddialog_impl.h"
#include "quiz.h"

#include <kpushbutton.h>
#include <kcombobox.h>
#include <ktextedit.h>
#include <klocale.h>
#include <knuminput.h>

#include <qtable.h>
#include <qbutton.h>
#include <qbuttongroup.h>

#include <kdebug.h>

questionAddDialogImpl::questionAddDialogImpl(QWidget* parent, const char* name)
 : questionAddDialog(parent,name)
{
	connect( KPB_test, SIGNAL( clicked() ), this, SLOT( slotTestQuestion() ) );
	connect( KPB_addAnswer, SIGNAL( clicked() ), this, SLOT( slotAddAnswer() ) );
	connect( table, SIGNAL( valueChanged( int, int ) ), this, SLOT( slotAdjustAnswer(int,int) ) );
}

void questionAddDialogImpl::slotAdjustAnswer( int row, int col )
{
	kdDebug() << "questionAddDialogImpl::slotAdjustAnswer()" << endl;

	QString text = table->text( row, col );

	const answerList::iterator it = m_answers.at( row );

	kdDebug() << "Old answer: " << ( *it )->answer() << endl;
	
	(*it)->setAnswer( text );

	kdDebug() << "New answer: " << ( *it )->answer() << endl;
}

void questionAddDialogImpl::slotAddAnswer()
{
	kdDebug() << "questionAddDialogImpl::slotAddAnswer()" << endl;
	bool type;
	QString questiontext = KTE_answers->text();

	QBG_type->id( QBG_type->selected() ) ? type = true : type = false;

	Answer *a = new Answer( questiontext, type );
	m_answers.append( a );

	KTE_answers->clear();
	updateTable();
}

void questionAddDialogImpl::updateTable()
{
	kdDebug() << "questionAddDialogImpl::updateTable()" << endl;

	kdDebug() << "numrows: " << table->numRows() << endl;
	kdDebug() << "num of answers: " << m_answers.count() << endl;

	for ( int i = 0; i < m_answers.count() ; )
	{//clear the table
	 //how the **** do you clear a table?! This is not working and
	 //I don't get why. there has to be sth like table->clear()...
		table->removeRow( i );
		kdDebug() << "removing row " << i << endl;
		++i;
	}

	table->insertRows( 0 , m_answers.count() );

	answerList::iterator it = m_answers.begin();
	for ( int i = 0 ; it != m_answers.end() ; ++it )
	{
		table->setText( i, 0, (*it)->answer() );
		table->setText( i, 1, (*it)->isTrue() ? i18n( "false" ) : i18n( "true" ) );
		i++;
	}
}

void questionAddDialogImpl::slotTestQuestion()
{
	kdDebug() << "questionAddDialog::slotTestQuestion()" << endl;

	QString q = KLE_q->text();
	int grade = KSB_level->value();
	
	Task *t = new Task( q,grade );
	
	answerList::iterator it = m_answers.begin();
	for ( int i = 0 ; it != m_answers.end() ; ++it )
	{//add all answers to the task
		t->addAnswer( *it );

		kdDebug() << "Answer " << i << " is " << ( *it )->answer() << endl;

		++i;
	}
	QuizWidget *qw = new QuizWidget( t );

	//TODO: Add this test
}


