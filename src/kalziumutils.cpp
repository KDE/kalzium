/***************************************************************************
    copyright            : (C) 2005, 2006, 2007 by Carsten Niehaus
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

#include <QFont>
#include <QRect>
#include <QPainter>

#include <kdebug.h>
#include <klocale.h>

#include <tempunit.h>
#include <element.h>

#include "prefs.h"

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
    if (!el) return "error";

    QString result;
    double val = 0.0; //the value to convert

    switch ( kind )
    {
        //FIXME I just copied the code from "boilingpoint", no clue 
        //if that really works
        case ChemicalDataObject::meltingpoint: // a temperature
            {
                val = el->dataAsVariant( kind ).toDouble();
                if ( val <= 0.0 )
                    result = i18n( "Unknown Value" );
                else
                {
                    val = TempUnit::convert( val, (int)TempUnit::Kelvin, Prefs::temperature() );
                    result = i18nc( "%1 is the temperature, %2 is the unit, like \"300 K\"", "%1 %2", val, TempUnit::unitListSymbol( Prefs::temperature() ) );
                }
                break;
            }
        case ChemicalDataObject::boilingpoint:
            {
                val = el->dataAsVariant( kind ).toDouble();
                if ( val <= 0.0 )
                    result = i18n( "Unknown Value" );
                else
                {
                    val = TempUnit::convert( val, (int)TempUnit::Kelvin, Prefs::temperature() );
                    result = i18nc( "%1 is the temperature, %2 is the unit, like \"300 K\"", "%1 %2", val, TempUnit::unitListSymbol( Prefs::temperature() ) );
                }
                break;
            }
        case ChemicalDataObject::electronegativityPauling: // electronegativity
            {
                val = el->dataAsVariant( kind ).toDouble();
                if ( val <= 0.0 )
                    result = i18n( "Value not defined" );
                else
                    result = i18nc("Just a number", "%1", val );
                break;
            }
            //FIXME nothing is done here
        case ChemicalDataObject::electronAffinity: // an energy
            {
            }
            //FIXME I don't check for the real unit here, that needs to be fixed
        case ChemicalDataObject::density: // an energy
            {
                val = el->dataAsVariant( kind ).toDouble();
                if ( val <= 0.0 )
                    result = i18n( "Unknown Value" );
                else
                {
                    result = i18nc( "x gramm per cubic centimeter. This is the density.", "%1 g/cm3", val ); 
                }
                break;
            }
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
                                result = i18nc( "kilo joule per mol", "%1 kJ/mol", val );
                                break;
                            }
                        case 1: // use electronvolt
                            {
                                val /= 96.6;
                                result = i18nc( "electron volt", "%1 eV", val );
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
                    result = i18nc( "x u (units). The atomic mass.", "%1 u", val );
                break;
            }
        case ChemicalDataObject::date: // a date
            {
                val = el->dataAsVariant( kind ).toInt();
                if ( val > 1600 ) {
                    result = i18n( "This element was discovered in the year %1.", val );
                }else if( val == 0 ){
                    result = i18n( "Unknown." );
                } else {
                    result = i18n( "This element was known to ancient cultures." );
                }
                break;
            }
            //FIXME nothing done here...
        case ChemicalDataObject::radiusCovalent: // a length
        case ChemicalDataObject::radiusVDW:
            {
                val = el->dataAsVariant( kind ).toDouble();
                if ( val <= 0.0 )
                    result = i18n( "Unknown Value" );
                else
                    result = i18nc( "%1 is a length, eg: 12.3 pm", "%1 pm", val );
                break;
            }
        default:
            {
            }
    }

    return result;
}
