#ifndef INFORMATIONDIALOG_IMPL_H
#define INFORMATIONDIALOG_IMPL_H

#include "informationdialog.h"

#include <qpoint.h>

class InformationWidget : public InformationDialog
{
	Q_OBJECT

	public:
		InformationWidget( QWidget *parent );

	private:
		QString getTopic(QPoint);
		QString getDesc(QPoint);

	public slots:
		void slotUpdate( QPoint point );
};
#endif // INFORMATIONDIALOG_IMPL_H
