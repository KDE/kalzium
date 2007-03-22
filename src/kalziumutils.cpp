/***************************************************************************
    copyright            : (C) 2005 by Carsten Niehaus
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
#include "kalziumutils.h"

#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <qfont.h>
#include <qrect.h>
#include <qpainter.h>
#include <qglobal.h>

#include <math.h>
#if defined(Q_OS_SOLARIS)
#include <ieeefp.h>
#endif

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
	num = qRound( num );

	return num * power / 10000;
}
	
QString KalziumUtils::localizedValue( double val, int precision, unsigned long options  )
{
	QString str = KGlobal::locale()->formatNumber( val, precision ); 
	while( str.endsWith("0") )
		str.truncate( str.length()-1);
	if ( str.endsWith( KGlobal::locale()->decimalSymbol()  ) )
	{
		// we do not want trailing ',' values so readd trailing 0 
		str.append( '0' ); // 
	}
	return str;
}
