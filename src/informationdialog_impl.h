#ifndef INFORMATIONDIALOG_IMPL_H
#define INFORMATIONDIALOG_IMPL_H

#include "informationdialog.h"

#include <qpoint.h>
#include <qdom.h>
#include <qstring.h>

class InformationWidget : public InformationDialog
{
	Q_OBJECT

	public:
		InformationWidget( QWidget *parent );

	private:
		///returns the text formated as html
		QString getDesc(QPoint);

	public slots:
		void slotUpdate( QPoint point );
};

class QuizXMLParser
{
	public: 
		QuizXMLParser();

		QString readTasks( QDomDocument &questionDocument, int number );
		bool loadLayout( QDomDocument &questionDocument, const QString& filename );
};

#endif // INFORMATIONDIALOG_IMPL_H
