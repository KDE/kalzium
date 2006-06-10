#ifndef TABLESDIALOG_H
#define TABLESDIALOG_H
/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
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

#include <kdialogbase.h>

/**
 * @author Carsten Niehaus
 */
class TablesDialog : public KDialogBase
{
	Q_OBJECT

	public:
		TablesDialog( QWidget *parent = 0 );
		virtual ~TablesDialog();

		void createNumbersTable();
		void createGreekSymbolTable();
 
};

#endif // TABLESDIALOG_H
