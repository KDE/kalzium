#ifndef EXPORTER_H
#define EXPORTER_H

/***************************************************************************

                           The header for the exporter class of Kalzium
                             -------------------
    begin                : June 2003
    copyright            : (C) 2003, 2004, 2005 by Martin Pfeiffer
    email                : hubipete@gmx.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

class QWidget;
class QString;

class Exporter
{
public:
	Exporter();
	~Exporter();

	bool saveAsPNG( const QWidget* widget, QString fileName, int x = 0, int y = 0, int width = 0, int height = 0 );

};

#endif

