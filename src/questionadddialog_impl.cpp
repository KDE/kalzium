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

#include <kpushbutton.h>
#include <knuminput.h>
#include <ktextedit.h>
#include <klocale.h>

#include <qlayout.h>
#include <qlabel.h>
#include <qbuttongroup.h>

#include <kdebug.h>

questionAddDialogImpl::questionAddDialogImpl( QWidget* parent, const char* name)
 : questionAddDialog(parent,name)
{
	connect( KPB_addQuestions, SIGNAL( clicked() ), this, SLOT( slotAddAnswers() ) );
}

void questionAddDialogImpl::slotAddAnswers()
{
	kdDebug() << "questionAddDialogImpl::slotAddAnswers()" << endl;
	Task *task = new Task(question->text(), KINI_grade->value() );
	if ( answer_1->isModified() &&
			type_1->selected() )
	{
		task->addAnswer( new Answer( answer_1->text(), type_1->id( type_1->selected() ) == 0 ? true : false ) );
	}
	if ( answer_2->isModified() &&
			type_2->selected() )
	{
		task->addAnswer( new Answer( answer_2->text(), type_2->id( type_2->selected() ) == 0 ? true : false ) );
	}
	if ( answer_3->isModified() &&
			type_3->selected() )
	{
		kdDebug() << "Frage 3: " << answer_3->text() << endl;
		task->addAnswer( new Answer( answer_3->text(), type_3->id( type_3->selected() ) == 0 ? true : false ) );
	}
	if ( answer_4->isModified() 
			&& type_4->selected() )
	{
		kdDebug() << "Frage 4: " << answer_4->text() << endl;
		task->addAnswer( new Answer( answer_4->text(), type_4->id( type_4->selected() ) == 0 ? true : false ) );
	}
	
	emit taskAdded( task );

	close();
}

#include "questionadddialog_impl.moc"
