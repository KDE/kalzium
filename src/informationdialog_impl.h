#ifndef INFORMATIONDIALOG_IMPL_H
#define INFORMATIONDIALOG_IMPL_H

#include <kdialogbase.h>

class QDomDocument;
class QPoint;
class InformationDialog;
class PSE;

class InformationWidget : public KDialogBase
{
	Q_OBJECT

	public:
		InformationWidget( PSE *pse );

		void showSOM();

		void showTimeline();

	private:
		///returns the text formated as html
		QString getDesc(QPoint);

		PSE *m_pse;
		InformationDialog *m_infoDialog;

	protected slots:
		virtual void slotClose();

	public slots:
		void slotDate( int );
		void slotTemp( int );

		void tabSelected( QWidget* );

	signals:
		void closed();
};

class QuizXMLParser
{
	public: 
		QuizXMLParser();

		QString readTasks( QDomDocument &questionDocument, int number );
		bool loadLayout( QDomDocument &questionDocument, const QString& filename );
};

#endif // INFORMATIONDIALOG_IMPL_H
