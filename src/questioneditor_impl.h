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
#ifndef QUESTIONSEDITOR_IMPL_H
#define QUESTIONSEDITOR_IMPL_H

#include "questioneditor.h"
#include "quiz.h"
#include <qdom.h>

/**
basic class for the specific PSEs
@author Carsten Niehaus
*/

class questionEditorImpl : public questionEditor
{
	Q_OBJECT
	public:
		/**
		 * Constructor
		 */
		questionEditorImpl( QWidget* parent = 0, const char*  name = 0);

		bool readTasks( QDomDocument &questionDocument );
		bool loadLayout( QDomDocument &layoutDocument );

	private:
		QDomDocument m_questionDocument;

		void addTaskToListView( Task *t );

		void setupListView();

		TaskList m_tasklist;
		
	private slots:
		void slotAddQuestion(); 
};


#endif
