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
#include "quizxmlparser.h"
#include "quiz.h"

#include <qtextstream.h>
#include <qfile.h>

#include <klocale.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kfiledialog.h>
#include <kmessagebox.h>

#include <stdlib.h>

QuizXMLParser::QuizXMLParser()
{
}

bool QuizXMLParser::loadLayout( QDomDocument &questionDocument )
{
	kdDebug() << "questionEditorImpl::loadLayout()" << endl;

	KURL url;
	url.setPath( locate("data", "kalzium/data/"));
	url = KFileDialog::getOpenURL( url.path(),
		QString("*xml") );
	
	//	i18n("Open Kalzium Data file...") );
	
	QFile layoutFile( url.path() );
	
	if (!layoutFile.exists())
	{
		kdDebug() << "questionEditorImpl::loadLayout(): file does not exist" << endl;
		QString mString=i18n("The file was not found in\n"
				"$KDEDIR/share/apps/kalzium/data/\n\n"
				"Please install this file and start Kalzium again.\n\n");
		KMessageBox::information( 0, mString, i18n( "Loading File - Error" ) );
		exit(1);
	}

	if (!layoutFile.open(IO_ReadOnly))
		return false;

	///Check if document is well-formed
	if (!questionDocument.setContent(&layoutFile))
	{
		kdDebug() << "wrong xml" << endl;
		layoutFile.close();
		return false;
	}
	layoutFile.close();

	kdDebug() << "good xml" << endl;
	return true;
}
TaskList QuizXMLParser::readTasks( QDomDocument &questionDocument )
{
	kdDebug() << "questionEditorImpl::readTasks()" << endl;
	QDomNodeList taskList,             //the list of all tasks
					questionList,
					answerList;

	QDomAttr textAttibute, 
				gradeAttribute, 
				trueAttribute;  //whether or not the answer is true

	QDomElement taskElement, //a single task
				questionElement,
				aElement;

	//read in all tasks
	taskList = questionDocument.elementsByTagName( "task" );
	
	for ( uint i = 0; i < taskList.count(); ++i )
	{//iterate through all tasks

		taskElement = ( const QDomElement& ) taskList.item( i ).toElement();
		gradeAttribute = taskElement.attributeNode( "grade" );

		//get the question
		questionList = taskElement.elementsByTagName( "question" );
		questionElement = ( const QDomElement& ) questionList.item( 0 ).toElement();
		const QString q = questionElement.attributeNode( "text" ).value();
		
		//get the grade of the task
		const int g = gradeAttribute.value().toInt();

		//get the answers. In most cases you will get three or four answers.
		//Probably one will be true, the rest false
		answerList = taskElement.elementsByTagName( "answer" );

		 //get the number of answers
		int count = answerList.count();
		
		//generate the task, the answers will be added inside the next loop
		Task *t = new Task( q, g );
		
		for ( int i = 0; i < count; ++i )
		{//iterate through all answers and generate the tasks
			bool answerisTrue;
			aElement = ( const QDomElement& ) answerList.item( i ).toElement();
			
			QString answerString = aElement.attributeNode( "text" ).value();

			// check if the answer is true or not
			aElement.attributeNode( "type" ).value() == "true" ? 
						answerisTrue = true 
						: answerisTrue = false;
			
			Answer *answer = new Answer( answerString, answerisTrue );
			
			t->addAnswer( answer );
		}
		
		//
		//the task has now all answers, the grade and the question.
		//Now we can add the Task to the list of tasks
		//
		m_tasklist.addTask( t );
	}
	
	return m_tasklist;
}

TaskList QuizXMLParser::readTasks( int min, int max, QDomDocument &questionDocument )
{
	kdDebug() << "questionEditorImpl::readTasks()" << endl;
	QDomNodeList taskList,             //the list of all tasks
					questionList,
					answerList;

	QDomAttr textAttibute, 
				gradeAttribute, 
				trueAttribute;  //whether or not the answer is true

	QDomElement taskElement, //a single task
				questionElement,
				aElement;

	//read in all tasks
	taskList = questionDocument.elementsByTagName( "task" );
	
	for ( uint i = 0; i < taskList.count(); ++i )
	{//iterate through all tasks

		taskElement = ( const QDomElement& ) taskList.item( i ).toElement();
		gradeAttribute = taskElement.attributeNode( "grade" );

		//get the question
		questionList = taskElement.elementsByTagName( "question" );
		questionElement = ( const QDomElement& ) questionList.item( 0 ).toElement();
		const QString q = questionElement.attributeNode( "text" ).value();
		
		//get the grade of the task
		const int g = gradeAttribute.value().toInt();

		if ( g < min || g > max ) //this task is out of the grade-scope
			continue;
		
		//get the answers. In most cases you will get three or four answers.
		//Probably one will be true, the rest false
		answerList = taskElement.elementsByTagName( "answer" );

		 //get the number of answers
		int count = answerList.count();
		
		//generate the task, the answers will be added inside the next loop
		Task *t = new Task( q, g );
		
		for ( int i = 0; i < count; ++i )
		{//iterate through all answers and generate the tasks
			bool answerisTrue;
			aElement = ( const QDomElement& ) answerList.item( i ).toElement();
			
			QString answerString = aElement.attributeNode( "text" ).value();

			// check if the answer is true or not
			aElement.attributeNode( "type" ).value() == "true" ? 
						answerisTrue = true 
						: answerisTrue = false;
			
			Answer *answer = new Answer( answerString, answerisTrue );
			
			t->addAnswer( answer );
		}
		
		//
		//the task has now all answers, the grade and the question.
		//Now we can add the Task to the list of tasks
		//
		m_tasklist.addTask( t );
	}
	
	return m_tasklist;
}

QuizXMLWriter::QuizXMLWriter( QFile *file )
{
	kdDebug() << "QuizXMLWriter::QuizXMLWriter()" << endl;
	outputFile = file;
	if ( outputFile->open( IO_WriteOnly ) )
	{
		outputStream.setDevice( outputFile );
		outputStream.setEncoding( QTextStream::UnicodeUTF8 );
		outputStream << "<kalzium>" << endl;
	}
}

QuizXMLWriter::~QuizXMLWriter()
{
	outputStream << "</kalzium>" << endl;
	outputFile->close();
}

void QuizXMLWriter::addItem(Task* task)
{
	//the next line writes eg: <task grade="2">
	outputStream << "   <task grade=\"" << task->isGrade() << "\">" << endl;

	//the next line writes eg: <question text="blah" />
	outputStream << "       <question text=\"" << task->question() << "\" />" << endl;

	for ( int i = 0 ; i < task->numberOfAnswers() ; ++i )
	{//write all answers
		QString type = "\"true\""; //task->answerAt(i)->isTrue ? "true" : "false";
		QString answertext = task->answerAt( i )->answer();
		outputStream << "       <answer type=" << type << " text=\"" << answertext << "\" />" << endl;
	}

	//close the task
	outputStream << "   </task>" << endl;
}

void QuizXMLWriter::addHeader()
{}

