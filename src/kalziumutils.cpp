#include "kalziumutils.h"

#include <kdebug.h>
#include <qfont.h>
#include <qrect.h>
#include <qpainter.h>

int KalziumUtils::maxSize( const QString& string, const QRect& rect, QFont font, QPainter* p )
{
	bool goodSizeFound = false;
	int size = 25;
	QRect r;

	kdDebug() << "At the beginning: " << rect << endl;

	do
	{
		font.setPointSize( size );
		p->setFont( font );
		r = p->boundingRect( QRect(), Qt::AlignAuto, string );
		r.moveBy( rect.left(), rect.top() );
		kdDebug() << "String: " << string << " size: " << font.pointSize() << " r: " << r << " rect: " << rect << endl;
		if ( rect.contains( r ) )
			goodSizeFound = true;
		else
			size--;
	}
	while ( !goodSizeFound && ( size > 4 ) );

	return size;
}
	
