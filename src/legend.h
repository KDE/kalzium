/***************************************************************************

                           legend.h  -  description
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

#ifndef _LEGEND_H
#define _LEGEND_H

#include <qwidget.h>

class KConfig;
class QPushButton;

class KalziumLegend : public QWidget
{
    Q_OBJECT

    public:
	KalziumLegend(QWidget *parent = 0, const char *name=0);

	void changeLegend(int);
	
	QPushButton *one, 
	*two, 
	*three, 
	*four, 
	*five, 
	*six, 
	*seven, 
	*eight;

    private:
	KConfig *main_config;
};
#endif
