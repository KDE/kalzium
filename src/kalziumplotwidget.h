#ifndef KALZIUMPLOTWIDGET_H
#define KALZIUMPLOTWIDGET_H
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

#include <qwidget.h>

class KalziumPlotWidget : public QWidget
{
	Q_OBJECT

	public:
		KalziumPlotWidget( QWidget *parent=0, const char *name =0 );

	protected slots:
		virtual void paintEvent( QPaintEvent* );
};

#endif // KALZIUMPLOTWIDGET_H

