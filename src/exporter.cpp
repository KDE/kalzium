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

#include <QPainter>
#include <QPixmap>
#include <QFile>
#include <QTextStream>

#include <kdebug.h>
#include <kimageio.h>
#include <khtml_part.h>
#include <kglobalsettings.h>
#include <kprinter.h>


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
		QPixmap* tmpPixmap = new QPixmap( width, height );
		QPainter* paint = new QPainter();
		paint->begin( tmpPixmap );
		paint->drawPixmap( 0, 0, *pixmap, x, y, width, height );
		paint->end();

		return tmpPixmap->save( fileName, "PNG" );
	}

	return pixmap->save( fileName, "PNG" );
}

QString Exporter::supportedImageFormats()
{
	return KImageIO::pattern( KImageIO::Writing );
}
bool Exporter::printElements( const QList<Element*>& elements, const QString& path )
{
        if ( path.isEmpty() )
                return false;

	KPrinter printer;
	printer.setPageSize();
	QPainter painter;
	QString imgdir/* = locate(  "data" , "kalzium/elempics/" )*/;


	QFile file( path );                     // load the html template
	if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
		return false;

	QTextStream html_stream( &file  );
	QString html;
	KHTMLPart* part = new KHTMLPart();      // create a new KHTMLPart
	painter.begin( &printer );

	foreach( Element* element, elements )   // iterate through all elements and print each one on a page
	{
		html = html_stream.readAll();   // replace the tags with values
		html.replace( "$NAME", element->dataAsString( ChemicalDataObject::name ) );
		html.replace( "$BOILINGPOINT", element->dataAsString( ChemicalDataObject::boilingpoint ) );
		html.replace( "$IMGPATH", imgdir + element->dataAsString( ChemicalDataObject::symbol ) + ".jpg" );

		part->begin();
		part->write( html );            //write the html code into the KHTMLPart
		part->end();
		
		part->paint( painter, );        // let the KHtmlPart paint to the printer-painter
		
		printer.newPage();
	}
	
	painter.end();
	
	return true;
}

