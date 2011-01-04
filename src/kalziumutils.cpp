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
#include <tqfont.h>
#include <tqrect.h>
#include <tqpainter.h>
#include <tqglobal.h>

#include <math.h>
#if defined(Q_OS_SOLARIS)
#include <ieeefp.h>
#endif

int KalziumUtils::maxSize( const TQString& string, const TQRect& rect, TQFont font, TQPainter* p, int minFontSize, int maxFontSize )
{
	bool goodSizeFound = false;
	int size = maxFontSize;
	TQRect r;

	do
	{
		font.setPointSize( size );
		p->setFont( font );
		r = p->boundingRect( TQRect(), Qt::AlignAuto, string );
		r.moveBy( rect.left(), rect.top() );
		
		if ( rect.tqcontains( r ) )
			goodSizeFound = true;
		else
			size--;
	}
	while ( !goodSizeFound && ( size > minFontSize ) );

	return size;
}

int KalziumUtils::StringHeight( const TQString& string, const TQFont& font, TQPainter* p )
{
	return p->boundingRect( TQRect(), Qt::AlignAuto, string ).height();
}

int KalziumUtils::StringWidth( const TQString& string, const TQFont& font, TQPainter* p )
{
	return p->boundingRect( TQRect(), Qt::AlignAuto, string ).width();
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
	num = tqRound( num );

	return num * power / 10000;
}
	
TQString KalziumUtils::localizedValue( double val, int precision, unsigned long options  )
{
	TQString str = KGlobal::locale()->formatNumber( val, precision ); 
	while( str.endsWith("0") )
		str.truncate( str.length()-1);
	if ( str.endsWith( KGlobal::locale()->decimalSymbol()  ) )
	{
		// we do not want trailing ',' values so readd trailing 0 
		str.append( '0' ); // 
	}
	return str;
}
