#ifndef CSVDIALOGIMPG_H
#define CSVDIALOGIMPG_H
/***************************************************************************
                    csvdialogimpl.h  -  description
                             -------------------
    begin                : June 2003
	copyright            : (C) 2003 by Carsten Niehaus                     
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

class KURL;
#include "csvdialog.h"

class csvdialogImpl : public csvdialog
{
	Q_OBJECT

	public:
		csvdialogImpl( bool , int, QString, QChar del, QWidget *parent = 0 , const char *name = 0 );

		QChar m_delimiter;
		QString requestedFile;
		bool selectionOnly;
		int whatToExport;


	private slots: 
		void delimiterClicked( int id );
		void slotStartExport();
};

	

#endif // CSVDIALOGIMPG_H

