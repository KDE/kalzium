#ifndef EXPORTER_H
#define EXPORTER_H

/***************************************************************************

                           The header for the exporter class of Kalzium
                             -------------------
    begin                : June 2005
    copyright            : (C) 2005, 2006 by Martin Pfeiffer
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

class QPixmap;
class QString;
class Element;

#include <QList>

/**
 * Small class that provides some exporting functions.
 *
 * @author Martin Pfeiffer
 */
class Exporter
{
public:
	Exporter();
	~Exporter();

	/**
	 * Export the image @p pixmap to the file @p fileName.
	 * If specified, @p x, @p y, @p width and @p height gives the region of
	 * @p pixmap we should export. Otherwise, the entire @p pixmap will be
	 * exported.
	 * @return whether the image was saved successfully
	 */
	bool saveAsImage( const QPixmap* pixmap, const QString& fileName, int x = 0, int y = 0, int width = 0, int height = 0 );

	/**
	 * Which image type can we export to?
	 * @return a string representing a filter string for file dialogs with
	 * the image type we can export to
	 */
	QString supportedImageFormats();

	/**
	 * Saves data as .cvs file
	 * @return whether the data has been saved successfully
	 * @todo Find the implementation of this method, and figure out
	 *       if 'CVS' is really intended, as opposed to CSV.
	 */
	bool saveAsCVS();
};

#endif

