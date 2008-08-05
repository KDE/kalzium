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
	m_prevUnit = Prefs::temperatureUnit();

	connect( temp_spinbox, SIGNAL( valueChanged( int ) ),
	         this, SLOT( sliderValueChanged( int ) ) );
	connect( temp_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( sliderValueChanged( int ) ) );

	reloadUnits();
}

int SOMWidgetIMPL::temperature() const
{
	return TempUnit::convert( temp_slider->value(), Prefs::temperatureUnit(), TempUnit::Kelvin );
}

void SOMWidgetIMPL::reloadUnits()
{
	temp_spinbox->blockSignals( true );
	temp_slider->blockSignals( true );
	lblUnit->setText( TempUnit::unitListSymbol( Prefs::temperatureUnit() ) );
	QPair<double, double> range = TempUnit::rangeForUnit( Prefs::temperatureUnit() );

	int newvalue = TempUnit::convert( temperature(), m_prevUnit, Prefs::temperatureUnit() );
	temp_spinbox->setRange( range.first, range.second );
	temp_spinbox->setValue( newvalue );
        temp_slider->setRange( range.first, range.second );
        temp_slider->setValue( newvalue );
	setNewTemp( newvalue );
	m_prevUnit = Prefs::temperatureUnit();
	temp_spinbox->blockSignals( false );
	temp_slider->blockSignals( false );
}

void SOMWidgetIMPL::sliderValueChanged( int temp )
{
        //the signals need to be blocked as both will return to this slot. But no
        //matter which UI elements (slider oder spinbox) was changed, the other
        //has to be set to the same value
	temp_spinbox->blockSignals( true );
	temp_slider->blockSignals( true );
	temp_spinbox->setValue( temp );
	temp_slider->setValue( temp );
	setNewTemp( temp );
	temp_spinbox->blockSignals( false );
	temp_slider->blockSignals( false );
}

void SOMWidgetIMPL::setNewTemp( int newtemp )
{
	static const int threshold = 25;
	
        const QString unitSymbol = TempUnit::unitListSymbol( Prefs::temperatureUnit() );

	double temp = TempUnit::convert( newtemp, Prefs::temperatureUnit(), (int)TempUnit::Kelvin );

	QStringList listMeltingPoint;
	QStringList listBoilingPoint;
	QStringList listBoilingPointValue;
	QStringList listMeltingPointValue;

	foreach (Element * element, m_list)
	{
		double melting = element->dataAsVariant( ChemicalDataObject::meltingpoint ).toDouble();
		if ( ( melting > 0.0 ) && fabs( melting - temp ) <= threshold )
		{
			listMeltingPoint << element->dataAsString( ChemicalDataObject::name );
			listMeltingPointValue << QString::number(TempUnit::convert(melting,(int)TempUnit::Kelvin,Prefs::temperatureUnit())); 
		}
		double boiling = element->dataAsVariant( ChemicalDataObject::boilingpoint ).toDouble();
		if ( ( boiling > 0.0 ) && fabs( boiling - temp ) <= threshold )
		{
			listBoilingPoint << element->dataAsString( ChemicalDataObject::name );
			listBoilingPointValue << QString::number(TempUnit::convert(boiling,(int)TempUnit::Kelvin,Prefs::temperatureUnit()));
		}
	}
	QString htmlcode;
	if ( listMeltingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with melting point around this temperature:" ) + '\n';
		for ( int i = 0; i < listMeltingPoint.count(); i++ )
		{
			htmlcode += " - " + i18nc( "For example: Carbon (300K)", "%1 (%2%3)", 
					listMeltingPoint.at( i ), listMeltingPointValue.at( i ), unitSymbol ) + '\n';
		}
		htmlcode += '\n';
	}
	else
	{
		htmlcode += i18n( "No elements with a melting point around this temperature" );
		htmlcode += "\n\n";
	}
	if ( listBoilingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with boiling point around this temperature:" ) + '\n';
		for ( int i = 0; i < listBoilingPoint.count(); i++ )
		{
			htmlcode += " - " + i18nc( "For example: Carbon (300K)", "%1 (%2%3)", 
					listBoilingPoint.at( i ), listBoilingPointValue.at( i ), unitSymbol )  + '\n';
		}
		htmlcode += '\n';
	}
	else
	{
		htmlcode += i18n( "No elements with a boiling point around this temperature" );
		htmlcode += '\n';
	}

	text->setText( m_htmlBegin + htmlcode + m_htmlEnd );

	emit temperatureChanged( temperature() );
}

#include "somwidget_impl.moc"
