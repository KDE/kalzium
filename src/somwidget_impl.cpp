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

#include "somwidget_impl.h"

#include <qlabel.h>
#include <qpair.h>
#include <qslider.h>
#include <qtextedit.h>
#include <qvaluelist.h>

#include <kdebug.h>
#include <knuminput.h>
#include <klocale.h>

#include <math.h>

#include "element.h"
#include "kalziumdataobject.h"
#include "prefs.h"
#include "tempunit.h"

SOMWidgetIMPL::SOMWidgetIMPL( QWidget *parent, const char* name )
	: SOMWidget( parent,name )
{
	m_list = KalziumDataObject::instance()->ElementList;

	text->setAlignment( text->alignment() | Qt::WordBreak );
	text->setTextFormat( Qt::RichText );
	text->setReadOnly( true );
	text->setPaletteBackgroundColor( paletteBackgroundColor() );
	text->setFrameStyle( QFrame::NoFrame );

	m_htmlBegin = "<qt>";
	m_htmlEnd = "</qt>";
	m_prevUnit = Prefs::temperature();

	connect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( spinValueChanged( double ) ) );
	connect( temp_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( sliderValueChanged( int ) ) );
	connect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( setNewTemp( double ) ) );

	reloadUnits();
}

void SOMWidgetIMPL::reloadUnits()
{
	lblUnit->setText( TempUnit::unitListSymbol( Prefs::temperature() ) );
	QPair<double, double> range = TempUnit::rangeForUnit( Prefs::temperature() );

	double newvalue = TempUnit::convert( Number1->value(), m_prevUnit, Prefs::temperature() );
	Number1->setRange( range.first, range.second, 0.1, 1 );
	Number1->setValue( newvalue );
	setNewTemp( newvalue );
kdDebug() << "min: " << Number1->minValue() << " - max: " << Number1->maxValue() << endl;
	m_prevUnit = Prefs::temperature();
}

void SOMWidgetIMPL::sliderValueChanged( int temp )
{
// TODO check if in Qt4 the RangeControl emits the signal again
	disconnect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( spinValueChanged( double ) ) );
	disconnect( temp_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( sliderValueChanged( int ) ) );
	disconnect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( setNewTemp( double ) ) );
	double newvalue = TempUnit::convert( (double)temp, (int)TempUnit::Kelvin, Prefs::temperature() );
	Number1->setValue( newvalue );
	setNewTemp( newvalue );
	connect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( spinValueChanged( double ) ) );
	connect( temp_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( sliderValueChanged( int ) ) );
	connect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( setNewTemp( double ) ) );
}

void SOMWidgetIMPL::spinValueChanged( double temp )
{
	disconnect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( spinValueChanged( double ) ) );
	disconnect( temp_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( sliderValueChanged( int ) ) );
	disconnect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( setNewTemp( double ) ) );
	int newvalue = (int)TempUnit::convert( temp, Prefs::temperature(), (int)TempUnit::Kelvin );
	temp_slider->setValue( newvalue );
	setNewTemp( temp );
	connect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( spinValueChanged( double ) ) );
	connect( temp_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( sliderValueChanged( int ) ) );
	connect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( setNewTemp( double ) ) );
}

void SOMWidgetIMPL::setNewTemp( double newtemp )
{
	static const int threshold = 25;

	double temp = TempUnit::convert( newtemp, Prefs::temperature(), (int)TempUnit::Kelvin );

	QValueList<Element*>::ConstIterator it = m_list.begin();
	const QValueList<Element*>::ConstIterator itEnd = m_list.end();

	QStringList listMeltingPoint;
	QStringList listBoilingPoint;
	QStringList listBoilingPointValue;
	QStringList listMeltingPointValue;
	for ( ; it != itEnd; ++it )
	{
		if ( ( ( *it )->melting() > 0.0 ) && fabs( ( *it )->melting() - temp ) <= threshold )
		{
			listMeltingPoint << ( *it )->elname();
			listMeltingPointValue << ( *it )->adjustUnits( Element::MELTINGPOINT );
		}
		if ( ( ( *it )->boiling() > 0.0 ) && fabs( ( *it )->boiling() - temp ) <= threshold )
		{
			listBoilingPoint << ( *it )->elname();
			listBoilingPointValue << ( *it )->adjustUnits( Element::BOILINGPOINT );
		}
	}
	QString htmlcode;
	if ( listMeltingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with melting point around this temperature:" ) + "<br>";
		for ( uint i = 0; i < listMeltingPoint.count(); i++ )
		{
			htmlcode += "&nbsp;<b>&middot;</b>&nbsp;" + i18n( "For example: Carbon (300K)", "%1 (%2)" ).arg( listMeltingPoint[i] ).arg( listMeltingPointValue[i] ) + "<br>";
		}
		htmlcode += "<br>";
	}
	else
	{
		htmlcode += i18n( "No elements with a melting point around this temperature" );
		htmlcode += "<br><br>";
	}
	if ( listBoilingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with boiling point around this temperature:" ) + "<br>";
		for ( uint i = 0; i < listBoilingPoint.count(); i++ )
		{
			htmlcode += "&nbsp;<b>&middot;</b>&nbsp;" + i18n( "For example: Carbon (300K)", "%1 (%2)" ).arg( listBoilingPoint[i] ).arg( listBoilingPointValue[i] ) + "<br>";
		}
		htmlcode += "<br>";
	}
	else
	{
		htmlcode += i18n( "No elements with a boiling point around this temperature" );
		htmlcode += "<br>";
	}

//	kdDebug() << m_htmlBegin + htmlcode + m_htmlEnd << endl;

	text->setText( m_htmlBegin + htmlcode + m_htmlEnd );

}

#include "somwidget_impl.moc"
