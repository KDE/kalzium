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
#include <ktextedit.h>

#include <kdebug.h>

questionAddDialogImpl::questionAddDialogImpl(QWidget* parent, const char* name)
 : questionAddDialog(parent,name)
{
	connect( KPB_test, SIGNAL( clicked() ), this, SLOT( slotTestQuestion() ) );
}

void questionAddDialogImpl::slotTestQuestion()
{
	kdDebug() << "questionAddDialog::slotTestQuestion()" << endl;

	QString q = KLE_q->text();
	int grade = 1;
	
	/*
	Task *t = new Task( q,a,wa1,wa2,grade );
	
	QuizWidget *qw = new QuizWidget( t );
	qw->show();
	*/
}


