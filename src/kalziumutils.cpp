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

#include <qfont.h>
#include <qrect.h>
#include <qpainter.h>

#include <kdebug.h>
#include <klocale.h>

#include <tempunit.h>
#include <element.h>

#include "prefs.h"

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
		r = p->boundingRect( QRect(), Qt::AlignTop | Qt::AlignLeft, string );
		r.translate( rect.left(), rect.top() );
		
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
	Q_UNUSED( font );
	return p->boundingRect( QRect(), Qt::AlignTop | Qt::AlignLeft, string ).height();
}

int KalziumUtils::StringWidth( const QString& string, const QFont& font, QPainter* p )
{
	Q_UNUSED( font );
	return p->boundingRect( QRect(), Qt::AlignTop | Qt::AlignLeft, string ).width();
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

QString KalziumUtils::prettyUnit( const Element* el, ChemicalDataObject::BlueObelisk kind )
{
	QString result;
	double val = 0.0; //the value to convert

	switch ( kind )
	{
		case ChemicalDataObject::meltingpoint: // a temperatures
		case ChemicalDataObject::boilingpoint:
		{
			val = el->dataAsVariant( kind ).toDouble();
			if ( val <= 0.0 )
				result = i18n( "Unknown Value" );
			else
			{
				val = TempUnit::convert( val, (int)TempUnit::Kelvin, Prefs::temperature() );
				switch ( Prefs::temperature() )
				{
					case 0: //Kelvin
						result = i18n( "%1 is the temperature in Kelvin", "%1 K" ).arg( val );
						break;
					case 1://Kelvin to Celsius
						result = i18n( "%1 is the temperature in Celsius", "%1 %2C" ).arg( val ).arg( QChar(0xB0) );
						break;
					case 2: // Kelvin to Fahrenheit
						result = i18n( "%1 is the temperature in Fahrenheit", "%1 %2F" ).arg( val ).arg( QChar(0xB0) );
						break;
					case 3: // Kelvin to Rankine
						result = i18n( "%1 is the temperature in Rankine", "%1 %2Ra" ).arg( val ).arg( QChar(0xB0) );
						break;
					case 4: // Kelvin to Reamur
						result = i18n( "%1 is the temperature in Reamur", "%1 %2R" ).arg( val ).arg( QChar(0xB0) );
						break;
				}
			}
			break;
		}
		case ChemicalDataObject::electronegativityPauling: // electronegativity
		{
			val = el->dataAsVariant( kind ).toDouble();
			if ( val <= 0.0 )
				result = i18n( "Value not defined" );
			else
				result = QString::number( val );
			break;
		}
		case ChemicalDataObject::electronAffinity: // an energy
		case ChemicalDataObject::ionization:
		{
			val = el->dataAsVariant( kind ).toDouble();
			if ( val <= 0.0 )
				result = i18n( "Value not defined" );
			else
			{
				switch ( Prefs::energies() )
				{
					case 0:
					{
						result = i18n( "%1 kJ/mol" ).arg( val );
						break;
					}
					case 1: // use electronvolt
					{
						val /= 96.6;
						result = i18n( "%1 eV" ).arg( val );
						break;
					}
				}
			}
			break;
		}
		case ChemicalDataObject::mass: // a mass
		{
			val = el->dataAsVariant( kind ).toDouble();
			if ( val <= 0.0 )
				result = i18n( "Unknown Value" );
			else
				result = i18n( "%1 u" ).arg( val );
			break;
		}
		case ChemicalDataObject::date: // a date
		{
			val = el->dataAsVariant( kind ).toInt();
			result = val < 1600
			         ? i18n( "This element was known to ancient cultures." )
			         : i18n( "This element was discovered in the year %1." ).arg( val );
			break;
		}
		case ChemicalDataObject::radiusCovalent: // a length
		case ChemicalDataObject::radiusVDW:
		{
			val = el->dataAsVariant( kind ).toDouble();
			if ( val <= 0.0 )
				result = i18n( "Unknown Value" );
			else
				result = i18n( "%1 is a length, eg: 12.3 pm", "%1 pm" ).arg( val );
			break;
		}
		default:
		{
		}
	}

	return result;
}
