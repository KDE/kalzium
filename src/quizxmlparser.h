#ifndef QUIZXML_H
#define QUIZXML_H
/***************************************************************************
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

class TaskList;
#include <qdom.h>
#include "quiz.h"

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 * @version 1.1
 */
class QuizXMLParser
{
	public:
		/**
		 * Default Constructor
		 */
		QuizXMLParser();

		/**
		 * read in all tasks defined in the questions-file
		 * @param min the minimum grade
		 * @param max the minimum grade
		 * @param questionDocument the QDomDocument which is being parsed
		 */
		TaskList readTasks( int min, int max, QDomDocument &questionDocument );
		
		/**
		 * checks if the file is well-formed XML
		 * @return if the XML is well-formed XML
		 */
		bool loadLayout( QDomDocument &questionDocument );

	private:
		TaskList m_tasklist;

};
	

#endif // QUIZXML_H

