/***************************************************************************
 *   Copyright (C) 2005, 2006      by Pino Toscano, toscano.pino@tiscali.it      *
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
#include <kdebug.h>

KalziumGradientTypeFactory::KalziumGradientTypeFactory()
{
	m_gradients << KalziumCovalentRadiusGradientType::instance();
	m_gradients << KalziumVanDerWaalsRadiusGradientType::instance();
	m_gradients << KalziumMassGradientType::instance();
	m_gradients << KalziumBoilingPointGradientType::instance();
	m_gradients << KalziumMeltingPointGradientType::instance();
	m_gradients << KalziumElectronegativityGradientType::instance();
	m_gradients << KalziumElectronaffinityGradientType::instance();
	m_gradients << KalziumDiscoverydateGradientType::instance();
	m_gradients << KalziumIonizationGradientType::instance();
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
	return val > .0 ? ( val - minValue() ) / ( maxValue() - minValue() ) : -1;
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

KalziumCovalentRadiusGradientType* KalziumCovalentRadiusGradientType::instance()
{
	static KalziumCovalentRadiusGradientType kcrgt;
	return &kcrgt;
}

KalziumCovalentRadiusGradientType::KalziumCovalentRadiusGradientType()
  : KalziumGradientType()
{
}

QByteArray KalziumCovalentRadiusGradientType::name() const
{
	return "CovalentRadius";
}

QString KalziumCovalentRadiusGradientType::description() const
{
	return i18n( "Covalent Radius" );
}

double KalziumCovalentRadiusGradientType::value( int el ) const
{
	QVariant v = KalziumDataObject::instance()->element( el )->dataAsVariant( ChemicalDataObject::radiusCovalent );
	if ( v.type() != QVariant::Double ) return -1;
	return v.toDouble();
}

double KalziumCovalentRadiusGradientType::minValue() const
{
	return 0.32;
}

double KalziumCovalentRadiusGradientType::maxValue() const
{
	return 2.25;
}


KalziumVanDerWaalsRadiusGradientType* KalziumVanDerWaalsRadiusGradientType::instance()
{
	static KalziumVanDerWaalsRadiusGradientType kvdwrgt;
	return &kvdwrgt;
}

KalziumVanDerWaalsRadiusGradientType::KalziumVanDerWaalsRadiusGradientType()
  : KalziumGradientType()
{
}

QByteArray KalziumVanDerWaalsRadiusGradientType::name() const
{
	return "KalziumVanDerWaalsRadiusGradientType";
}

QString KalziumVanDerWaalsRadiusGradientType::description() const
{
	return i18n( "van Der Waals" );
}

double KalziumVanDerWaalsRadiusGradientType::value( int el ) const
{
	QVariant v = KalziumDataObject::instance()->element( el )->dataAsVariant( ChemicalDataObject::radiusVDW );
	if ( v.type() != QVariant::Double ) return -1;
	return v.toDouble();
}

double KalziumVanDerWaalsRadiusGradientType::minValue() const
{
	return 1.2;
}

double KalziumVanDerWaalsRadiusGradientType::maxValue() const
{
	return 3.0;
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


KalziumElectronegativityGradientType* KalziumElectronegativityGradientType::instance()
{
	static KalziumElectronegativityGradientType kegt;
	return &kegt;
}

KalziumElectronegativityGradientType::KalziumElectronegativityGradientType()
  : KalziumGradientType()
{
}

QByteArray KalziumElectronegativityGradientType::name() const
{
	return "Electronegativity";
}

QString KalziumElectronegativityGradientType::description() const
{
	return i18n( "Electronegativity (Pauling)" );
}

double KalziumElectronegativityGradientType::value( int el ) const
{
	QVariant v = KalziumDataObject::instance()->element( el )->dataAsVariant( ChemicalDataObject::electronegativityPauling );
	if ( v.type() != QVariant::Double ) return -1;
	return v.toDouble();
}

double KalziumElectronegativityGradientType::minValue() const
{
	return 0.7;
}

double KalziumElectronegativityGradientType::maxValue() const
{
	return 3.98;
}

///DISCOVERYDATE///

KalziumDiscoverydateGradientType* KalziumDiscoverydateGradientType::instance()
{
	static KalziumDiscoverydateGradientType kegt;
	return &kegt;
}

KalziumDiscoverydateGradientType::KalziumDiscoverydateGradientType()
  : KalziumGradientType()
{
}

QByteArray KalziumDiscoverydateGradientType::name() const
{
	return "Discoverydate";
}

QString KalziumDiscoverydateGradientType::description() const
{
	return i18n( "Discoverydate" );
}

double KalziumDiscoverydateGradientType::value( int el ) const
{
	QVariant v = KalziumDataObject::instance()->element( el )->dataAsVariant( ChemicalDataObject::date );

	return v.toDouble();
}

double KalziumDiscoverydateGradientType::minValue() const
{
	return 1699.0;
}

double KalziumDiscoverydateGradientType::maxValue() const
{
	return 1994.0;
}


///ELECTRONAFFINITY///

KalziumElectronaffinityGradientType* KalziumElectronaffinityGradientType::instance()
{
	static KalziumElectronaffinityGradientType kegt;
	return &kegt;
}

KalziumElectronaffinityGradientType::KalziumElectronaffinityGradientType()
  : KalziumGradientType()
{
}

QByteArray KalziumElectronaffinityGradientType::name() const
{
	return "Electronaffinity";
}

QString KalziumElectronaffinityGradientType::description() const
{
	return i18n( "Electronaffinity" );
}

double KalziumElectronaffinityGradientType::value( int el ) const
{
	QVariant v = KalziumDataObject::instance()->element( el )->dataAsVariant( ChemicalDataObject::electronAffinity );

	return v.toDouble();
}

double KalziumElectronaffinityGradientType::minValue() const
{
	return 0.0;
}

double KalziumElectronaffinityGradientType::maxValue() const
{
	return 3.7;
}

///FIRST IONIZATINO///

KalziumIonizationGradientType* KalziumIonizationGradientType::instance()
{
	static KalziumIonizationGradientType kegt;
	return &kegt;
}

KalziumIonizationGradientType::KalziumIonizationGradientType()
  : KalziumGradientType()
{
}

QByteArray KalziumIonizationGradientType::name() const
{
	return "Ionization";
}

QString KalziumIonizationGradientType::description() const
{
	return i18n( "First Ionization" );
}

double KalziumIonizationGradientType::value( int el ) const
{
	QVariant v = KalziumDataObject::instance()->element( el )->dataAsVariant( ChemicalDataObject::ionization );

	return v.toDouble();
}

double KalziumIonizationGradientType::minValue() const
{
	return 0.0;
}

double KalziumIonizationGradientType::maxValue() const
{
	return 25.0;
}
