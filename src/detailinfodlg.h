/***************************************************************************

        detailedinfodlg.h  -  description
                             -------------------
    begin                : Tue Apr 2 20:43:44 2002 UTC
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

#ifndef _DETAILINFODLG_H_
#define _DETAILINFODLG_H_

#include <kdialogbase.h>

#include "elementkp.h"
class QFrame;
class DetailedTab;


class DetailedTab : public QWidget
{
	Q_OBJECT

	public:
		DetailedTab( ElementInfo Eleminfo , QWidget *parent, const char *name=0 );
		DetailedTab( QWidget *parent, const char *name=0 );
		ElementInfo EData;
		QColor PSEColor( const QString &Block ) const;
		void setData( ElementInfo Eleminfo );
	
	private:
		void drawBiologicalSymbol( QPainter *p );
	
		//calculation of the corners
		int x1,x2,y1,y2,h_t;


	protected:
		virtual void paintEvent( QPaintEvent* );
};

class DetailedInfoDlg : public KDialogBase
{
    Q_OBJECT
    
	public:
        DetailedInfoDlg( ElementInfo Eleminfo , QWidget *parent=0, const char *name=0);
		ElementInfo Data;

		QFrame *mainTab, *energyTab, *chemicalTab, *overviewTab, *miscTab, *orbitsTab;

		DetailedTab *dTab;

	protected slots:
		virtual void slotUser1();
};
#endif
