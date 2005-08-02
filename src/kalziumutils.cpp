#include "kalziumutils.h"

#include <kdebug.h>
#include <qfont.h>
#include <qrect.h>
#include <qpainter.h>

#include <math.h>

int KalziumUtils::maxSize( const QString& string, const QRect& rect, QFont font, QPainter* p, int minFontSize, int maxFontSize )
{
	bool goodSizeFound = false;
	int size = maxFontSize;
	QRect r;

	do
	{
		font.setPointSize( size );
		p->setFont( font );
		r = p->boundingRect( QRect(), Qt::AlignAuto, string );
		r.moveBy( rect.left(), rect.top() );
		
		if ( rect.contains( r ) )
			goodSizeFound = true;
		else
			size--;
	}
	while ( !goodSizeFound && ( size > minFontSize ) );

	return size;
}

int KalziumUtils::StringHeight( const QString& string, const QFont& font, QPainter* p )
{
	return p->boundingRect( QRect(), Qt::AlignAuto, string ).height();
}

int KalziumUtils::StringWidth( const QString& string, const QFont& font, QPainter* p )
{
	return p->boundingRect( QRect(), Qt::AlignAuto, string ).width();
}

double KalziumUtils::strippedValue( double num )
{
	if ( !finite( num ) )
		return num;

	double power;
	power = 1e-6;
	while ( power < num )
		power *= 10;

	num = num / power * 10000;
	num = round( num );

	return num * power / 10000;
}
	
