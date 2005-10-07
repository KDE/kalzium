#ifndef SMPRINTDIALOGPAGE_H
#define SMPRINTDIALOGPAGE_H
class Q3ButtonGroup;
class QRadioButton;


#include <kdeprint/kprintdialogpage.h>

/**
 * Print dialog page to provide specific print options for SpectrumEditor
 * @author Carsten Niehaus
*/
class SMPrintDialogPage : public KPrintDialogPage
{
	Q_OBJECT
	public:
		/**
		 * public constructor
		 */
		SMPrintDialogPage( QWidget *parent = 0 );

		~SMPrintDialogPage();
		//reimplement virtual functions
		void getOptions(QMap<QString,QString>& opts, bool incldef = false);
		void setOptions(const QMap<QString,QString>& opts);
		bool isValid(const QString& msg );

	private:
		Q3ButtonGroup * g;
		QRadioButton * rb0;
		QRadioButton * rb1;
		QRadioButton * rb2;
};
#endif // SMPRINTDIALOGPAGE_H
