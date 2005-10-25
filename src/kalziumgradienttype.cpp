/***************************************************************************
 *   Copyright (C) 2005      by Pino Toscano, toscano.pino@tiscali.it      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "kalziumgradienttype.h"

#include "element.h"
#include "kalziumdataobject.h"

#include <QVariant>

#include <klocale.h>

KalziumGradientTypeFactory::KalziumGradientTypeFactory()
{
	m_gradients << KalziumMassGradientType::instance();
	m_gradients << KalziumBoilingPointGradientType::instance();
	m_gradients << KalziumMeltingPointGradientType::instance();
}

KalziumGradientTypeFactory* KalziumGradientTypeFactory::instance()
{
	static KalziumGradientTypeFactory kttf;
	return &kttf;
}

KalziumGradientType* KalziumGradientTypeFactory::build( int id ) const
{
	if ( ( id < 0 ) || ( id >= m_gradients.count() ) )
		return 0;

	return m_gradients.at( id );
}

KalziumGradientType* KalziumGradientTypeFactory::build( const QByteArray& id ) const
{
	for ( int i = 0; i < m_gradients.count(); i++ )
	{
		if ( m_gradients.at( i )->name() == id )
			return m_gradients.at( i );
	}
	
	// not found
	return 0;
}

QStringList KalziumGradientTypeFactory::gradients() const
{
	QStringList l;
	for ( int i = 0; i < m_gradients.count(); i++ )
	{
		l << m_gradients.at( i )->description();
	}
	return l;
}


KalziumGradientType::KalziumGradientType()
{
}

KalziumGradientType::~KalziumGradientType()
{
}

KalziumGradientType* KalziumGradientType::instance()
{
	return 0;
}

double KalziumGradientType::elementCoeff( int el ) const
{
	double val = value( el );
	return val > .0 ? val / maxValue() : -1;
}

QColor KalziumGradientType::firstColor() const
{
	return Qt::white;
}

QColor KalziumGradientType::secondColor() const
{
	return Qt::red;
}

QColor KalziumGradientType::notAvailableColor() const
{
	return Qt::lightGray;
}

QColor KalziumGradientType::calculateColor( const double coeff ) const
{
	if ( ( coeff < 0.0 ) || ( coeff > 1.0 ) ) return notAvailableColor();

	QColor color2 = secondColor();
	QColor color1 = firstColor();

	int red = static_cast<int>( (color2.red() - color1.red()) * coeff + color1.red() );
	int green = static_cast<int>( (color2.green() - color1.green()) * coeff + color1.green() );
	int blue = static_cast<int>( (color2.blue() - color1.blue()) * coeff + color1.blue() );

	return QColor( red, green, blue );
}

KalziumMassGradientType* KalziumMassGradientType::instance()
{
	static KalziumMassGradientType kargt;
	return &kargt;
}

KalziumMassGradientType::KalziumMassGradientType()
  : KalziumGradientType()
{
}

QByteArray KalziumMassGradientType::name() const
{
	return "AtomicMass";
}

QString KalziumMassGradientType::description() const
{
	return i18n( "Atomic Mass" );
}

double KalziumMassGradientType::value( int el ) const
{
	QVariant v = KalziumDataObject::instance()->element( el )->dataAsVariant( ChemicalDataObject::mass );
	if ( v.type() != QVariant::Double ) return -1;
	return v.toDouble();
}

double KalziumMassGradientType::minValue() const
{
	return 1.00794;
}

double KalziumMassGradientType::maxValue() const
{
	return 292.0;
}


KalziumBoilingPointGradientType* KalziumBoilingPointGradientType::instance()
{
	static KalziumBoilingPointGradientType kbpgt;
	return &kbpgt;
}

KalziumBoilingPointGradientType::KalziumBoilingPointGradientType()
  : KalziumGradientType()
{
}

QByteArray KalziumBoilingPointGradientType::name() const
{
	return "BoilingPoint";
}

QString KalziumBoilingPointGradientType::description() const
{
	return i18n( "Boiling Point" );
}

double KalziumBoilingPointGradientType::value( int el ) const
{
	QVariant v = KalziumDataObject::instance()->element( el )->dataAsVariant( ChemicalDataObject::boilingpoint );
	if ( v.type() != QVariant::Double ) return -1;
	return v.toDouble();
}

double KalziumBoilingPointGradientType::minValue() const
{
	return 4.216;
}

double KalziumBoilingPointGradientType::maxValue() const
{
	return 5870.0;
}


KalziumMeltingPointGradientType* KalziumMeltingPointGradientType::instance()
{
	static KalziumMeltingPointGradientType kmpgt;
	return &kmpgt;
}

KalziumMeltingPointGradientType::KalziumMeltingPointGradientType()
  : KalziumGradientType()
{
}

QByteArray KalziumMeltingPointGradientType::name() const
{
	return "MeltingPoint";
}

QString KalziumMeltingPointGradientType::description() const
{
	return i18n( "Melting Point" );
}

double KalziumMeltingPointGradientType::value( int el ) const
{
	QVariant v = KalziumDataObject::instance()->element( el )->dataAsVariant( ChemicalDataObject::meltingpoint );
	if ( v.type() != QVariant::Double ) return -1;
	return v.toDouble();
}

double KalziumMeltingPointGradientType::minValue() const
{
	return 0.94;
}

double KalziumMeltingPointGradientType::maxValue() const
{
	return 3825.0;
}
