#include "informationdialog_impl.h"

#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qtextstream.h>
#include <qfile.h>

#include <klocale.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kfiledialog.h>
#include <kmessagebox.h>

#include <kdebug.h>


InformationWidget::InformationWidget( QWidget *parent )
	: InformationDialog( parent )
{
}

void InformationWidget::slotSelectionChanged( int number )
{
}


void InformationWidget::slotUpdate( QPoint point )
{
	m_topic->setText( getTopic(point) );
	m_explanation->setText( getDesc(point) );
}

QString InformationWidget::getDesc( QPoint point )
{
	QString information;
	if ( buttonGroup->selectedId() == 0 ) //the users looks for groups
		information = "foo bar bar foo groups";
	if ( buttonGroup->selectedId() == 1) //the users looks for groups
		information = "julia julia julia period";

	QuizXMLParser parser;
	QDomDocument doc( "periods" );
	QString fn;

	if ( buttonGroup->selectedId() == 1 ) fn = "periods.xml";
	else fn = "groups.xml";
	
	if ( parser.loadLayout( doc, fn ) )
		information = parser.readTasks( doc, point.x() );

	return information;
}

QString InformationWidget::getTopic( QPoint point )
{
	QString information;
	if ( buttonGroup->selectedId() == 0 ) //the users looks for groups
		information = "foo bar bar foo groups";
	if ( buttonGroup->selectedId() == 1) //the users looks for groups
		information = "julia julia julia period";

	return information;
}

QuizXMLParser::QuizXMLParser()
{
}

bool QuizXMLParser::loadLayout( QDomDocument &questionDocument, const QString& filename )
{
        KURL url;
        url.setPath( locate("data", "kalzium/data/"));
		url.setFileName( filename );

        QFile layoutFile( url.path() );
        
        if (!layoutFile.exists())
        {
                QString mString=i18n("The file was not found in\n"
                                "$KDEDIR/share/apps/kalzium/data/\n\n"
                                "Please install this file and start Kalzium again.\n\n");
                KMessageBox::information( 0, mString, i18n( "Loading File - Error" ) );
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

        return true;
}

QString QuizXMLParser::readTasks( QDomDocument &questionDocument, int number )
{
        QDomNodeList taskList,             //the list of all tasks
                                        questionList,
                                        answerList;

		QString html;

        QDomAttr textAttibute, 
                                gradeAttribute, 
                                trueAttribute;  //whether or not the answer is true

        QDomElement taskElement, //a single task
                                questionElement,
                                aElement;

        //read in all tasks
        taskList = questionDocument.elementsByTagName( "unit" );

        for ( uint i = 0; i < taskList.count(); ++i )
        {//iterate through all tasks
                taskElement = ( const QDomElement& ) taskList.item( i ).toElement();
                 if ( taskElement.attributeNode( "number" ).value().toInt() != number )
 					continue;

				QDomNode contentNode = taskElement.namedItem( "content" );
				QDomNode nameNode = taskElement.namedItem( "name" );
				html = contentNode.toElement().text();
				html.append(  nameNode.toElement().text() );
        }
        
	return html;
}
