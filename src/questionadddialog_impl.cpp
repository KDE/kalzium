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
#include "questiontextwidget.h"

#include <kpushbutton.h>
#include <knuminput.h>

#include <qlayout.h>

#include <kdebug.h>

questionAddDialogImpl::questionAddDialogImpl(QWidget* parent, const char* name)
 : questionAddDialog(parent,name)
{
	connect( KPB_addQuestions, SIGNAL( clicked() ), this, SLOT( slotAddAnswers() ) );
}

void questionAddDialogImpl::slotAddAnswers()
{
	kdDebug() << "questionAddDialogImpl::slotAddAnswers()" << endl;
	
	int num = KINI_number->value( );
	
	QuestionsWidget *qw = new QuestionsWidget( num );
	qw->show();
}

QuestionsWidget::QuestionsWidget( int num, QWidget* parent, const char* name )
	: QWidget( parent, name )
{
	vlay = new QVBoxLayout( this );
	for ( int i = 0; i < num; ++i )
	{
		QuestionTextWidget *qtw = new QuestionTextWidget( this );
		vlay->addWidget( qtw );
	}
}

#include "questionadddialog_impl.moc"
