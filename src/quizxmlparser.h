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
class QFile;
class QTextStream;

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
		 * read in all tasks defined in the questions-file which have
		 * a minimumgrade @p min and a maximumgrade @p max
		 * @param min the minimum grade
		 * @param max the minimum grade
		 * @param questionDocument the QDomDocument which is being parsed
		 */
		TaskList readTasks( int min, int max, QDomDocument &questionDocument );
		
		/**
		 * read in all tasks defined in the questions-file 
		 * @param questionDocument the QDomDocument which is being parsed
		 */
		TaskList readTasks( QDomDocument &questionDocument );
		
		/**
		 * checks if the file is well-formed XML
		 * @return if the XML is well-formed XML
		 */
		bool loadLayout( QDomDocument &questionDocument );

	private:
		TaskList m_tasklist;

};

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 * @version 1.1
 */
class QuizXMLWriter
{
	public:
		QuizXMLWriter( QFile *file );
		~QuizXMLWriter();

		/**
		 * add an the tags for the grade, the questions
		 * and the answers of the @p task
		 * @param task the Task which will be add to the xml-file
		 */
		void addItem(Task* task);
		void addHeader();

	private:
		QFile *outputFile;
		QTextStream outputStream;
};
	

#endif // QUIZXML_H

