#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H
/***************************************************************************

    copyright            : (C) 2005 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kdialogbase.h>

#include "element.h"

class QChar;
class QDomDocument;
class QListViewItem;
class KListView;
class KListViewSearchLine;
class KActionCollection;


class ExportDialog : public KDialogBase
{
	Q_OBJECT

	public:
		ExportDialog( KalziumDataObject *data, QWidget *parent=0, const char *name=0);
		~ExportDialog();

	private:
		KalziumDataObject *m_data;

		/**
		 * generate the csv-code which will be exported
		 */
		QString generateCSV();

	private slots:
		virtual void slotUser1();
};

#endif // EXPORTDIALOG_H

