/*   Copyright (C) 2005 by Martin Pfeiffer                                 *
 *   hubipete@gmx.net                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include "exporter.h"

#include <qpixmap.h>

#include <kdebug.h>
#include <kimageio.h>

Exporter::Exporter()
{
	static bool kimageioRegistered = false;
	if ( ! kimageioRegistered )
	{
		KImageIO::registerFormats();
		kimageioRegistered = true;
	}

}

Exporter::~Exporter()
{
}

bool Exporter::saveAsImage( const QPixmap* pixmap, const QString& fileName, int x, int y, int width, int height )
{
	if ( x != 0 || y != 0 || width != 0 || height != 0 )
	{
		QPixmap* tmpPixmap = new QPixmap();

		copyBlt( tmpPixmap, 0, 0, pixmap, x, y, width, height );
	
		if ( tmpPixmap->isNull() )
			kdDebug() << "empty pixmap" << endl;
		return tmpPixmap->save( fileName, "PNG" );
	}

	return pixmap->save( fileName, "PNG" );
}

QString Exporter::supportedImageFormats()
{
	return KImageIO::pattern( KImageIO::Writing );
}

