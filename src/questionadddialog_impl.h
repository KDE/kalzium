#ifndef QUESTIONADDDIALOG_IMPL_H
#define QUESTIONADDDIALOG_IMPL_H
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

#include "questionadddialog.h"

#include "quiz.h"

#include <qlabel.h>

class KPushButton;

/**
This class enables the user to add questions to the quiz
@short Add userdefined questions to the quiz
@author Carsten Niehaus
*/
class questionAddDialogImpl : public questionAddDialog
{
	Q_OBJECT
	public:
		/**
		 * Constructor
		 */
		questionAddDialogImpl( QWidget* parent = 0, const char*  name = 0);

	private:
		answerList m_answers;

	public slots:
		void slotAddAnswers();

	signals:
		void taskAdded( Task* );
};

#endif // QUESTIONADDDIALOG_IMPL_H

