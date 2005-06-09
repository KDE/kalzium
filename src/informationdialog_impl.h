/***************************************************************************
    copyright            : (C) 2005 by Carsten Niehaus
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

	private:
		///returns the text formated as html
		QString getDesc(QPoint);

		PSE *m_pse;
		InformationDialog *m_infoDialog;

	protected slots:
		virtual void slotClose();

	public slots:
		void slotTemp( int );

	signals:
		void closed();
};

#endif // INFORMATIONDIALOG_IMPL_H
