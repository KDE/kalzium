/***************************************************************************

                           KalziumGraph.h  -  description
                             -------------------
    begin                : Wed Oct 23 2002
    copyright            : (C) 2002 by Carsten Niehaus
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

#ifndef _KALZIUMGRAPH_H
#define _KALZIUMGRAPH_H

class KComboBox;
class QGridLayout;

#include <qwidget.h>
#include <kdialog.h>

class KalziumGraphDialog : public KDialog
{
	Q_OBJECT

	public:
		KalziumGraphDialog( QWidget *parent=0, const char *name =0 );
		QGridLayout *grid;
	public slots:
		void slotokClicked();
	private:
		KComboBox *kcb;
};

class KalziumGraphDisplay : public QWidget
{
	public:
		KalziumGraphDisplay( int, int, int, QWidget *parent=0, const char *name=0 );
};

class KalziumGraphDataContainer
{
	public:
		KalziumGraphDataContainer( int, int, int);
		QString Data[ 118 ];
};

//Datempräsentation
class KalziumGraph : public QWidget
{
    Q_OBJECT

    public:
	KalziumGraph(QWidget *parent = 0, const char *name=0, KalziumGraphDataContainer *data=0);
};

#endif
