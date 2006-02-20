/***************************************************************************
    copyright            : (C) 2005, 2006 by Carsten Niehaus
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

#include <QLabel>
#include <QPair>
#include <QSlider>
#include <QTextEdit>
#include <QList>
#include <QVariant>

#include <kdebug.h>
#include <knuminput.h>
#include <klocale.h>

#include <math.h>

#include <element.h>
#include <tempunit.h>

#include "kalziumdataobject.h"
#include "prefs.h"

SOMWidgetIMPL::SOMWidgetIMPL( QWidget *parent )
	: QWidget( parent )
{
	setupUi( this );

	m_list = KalziumDataObject::instance()->ElementList;

	m_htmlBegin = "";
	m_htmlEnd = "";
	m_prevUnit = Prefs::temperature();

	connect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( spinValueChanged( double ) ) );
	connect( temp_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( sliderValueChanged( int ) ) );
	connect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( setNewTemp( double ) ) );

	reloadUnits();
}

int SOMWidgetIMPL::temperature() const
{
	return temp_slider->value();
}

void SOMWidgetIMPL::reloadUnits()
{
	disconnect( Number1, SIGNAL( valueChanged( double ) ),
	            this, SLOT( spinValueChanged( double ) ) );
	disconnect( temp_slider, SIGNAL( valueChanged( int ) ),
	            this, SLOT( sliderValueChanged( int ) ) );
	disconnect( Number1, SIGNAL( valueChanged( double ) ),
	            this, SLOT( setNewTemp( double ) ) );
	lblUnit->setText( TempUnit::unitListSymbol( Prefs::temperature() ) );
	QPair<double, double> range = TempUnit::rangeForUnit( Prefs::temperature() );

	double newvalue = TempUnit::convert( Number1->value(), m_prevUnit, Prefs::temperature() );
	Number1->setRange( range.first, range.second, 0.1, 1 );
	Number1->setValue( newvalue );
	setNewTemp( newvalue );
kDebug() << "min: " << Number1->minimum() << " - max: " << Number1->maximum() << endl;
	m_prevUnit = Prefs::temperature();
	connect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( spinValueChanged( double ) ) );
	connect( temp_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( sliderValueChanged( int ) ) );
	connect( Number1, SIGNAL( valueChanged( double ) ),
	         this, SLOT( setNewTemp( double ) ) );
}

void SOMWidgetIMPL::sliderValueChanged( int temp )
{
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

	QList<Element*>::ConstIterator it = m_list.begin();
	const QList<Element*>::ConstIterator itEnd = m_list.end();

	QStringList listMeltingPoint;
	QStringList listBoilingPoint;
	QStringList listBoilingPointValue;
	QStringList listMeltingPointValue;
	for ( ; it != itEnd; ++it )
	{
		double melting = ( *it )->dataAsVariant( ChemicalDataObject::meltingpoint ).toDouble();
		if ( ( melting > 0.0 ) && fabs( melting - temp ) <= threshold )
		{
			listMeltingPoint << ( *it )->dataAsString( ChemicalDataObject::name );
			listMeltingPointValue << ( *it )->dataAsString( ChemicalDataObject::meltingpoint );
		}
		double boiling = ( *it )->dataAsVariant( ChemicalDataObject::boilingpoint ).toDouble();
		if ( ( boiling > 0.0 ) && fabs( boiling - temp ) <= threshold )
		{
			listBoilingPoint << ( *it )->dataAsString( ChemicalDataObject::name );
			listBoilingPointValue << ( *it )->dataAsString( ChemicalDataObject::boilingpoint );
		}
	}
	QString htmlcode;
	if ( listMeltingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with melting point around this temperature:" ) + "\n";
		for ( int i = 0; i < listMeltingPoint.count(); i++ )
		{
			htmlcode += " - " + i18n( "For example: Carbon (300K)", "%1 (%2)" ).arg( listMeltingPoint.at( i ) ).arg( listMeltingPointValue.at( i ) ) + "\n";
		}
		htmlcode += "\n";
	}
	else
	{
		htmlcode += i18n( "No elements with a melting point around this temperature" );
		htmlcode += "\n\n";
	}
	if ( listBoilingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with boiling point around this temperature:" ) + "\n";
		for ( int i = 0; i < listBoilingPoint.count(); i++ )
		{
			htmlcode += " - " + i18n( "For example: Carbon (300K)", "%1 (%2)" ).arg( listBoilingPoint.at( i ) ).arg( listBoilingPointValue.at( i ) ) + "\n";
		}
		htmlcode += "\n";
	}
	else
	{
		htmlcode += i18n( "No elements with a boiling point around this temperature" );
		htmlcode += "\n";
	}

//	kDebug() << m_htmlBegin + htmlcode + m_htmlEnd << endl;

	text->setText( m_htmlBegin + htmlcode + m_htmlEnd );

	emit temperatureChanged( temperature() );
}

#include "somwidget_impl.moc"
