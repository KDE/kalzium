/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include "chemicaldataobject.h"
#include <kdebug.h>

class ChemicalDataObjectPrivate : public QSharedData
{
    public:
        ChemicalDataObjectPrivate();
        ~ChemicalDataObjectPrivate();

        QVariant m_value;
        QVariant m_errorValue;
        ChemicalDataObject::BlueObelisk m_type;
        ChemicalDataObject::BlueObeliskUnit m_unit;
};

//########################
ChemicalDataObjectPrivate::ChemicalDataObjectPrivate()
: QSharedData()
{
}

ChemicalDataObjectPrivate::~ChemicalDataObjectPrivate()
{
}
//##############

ChemicalDataObject::ChemicalDataObject( const QVariant& v, BlueObelisk type, const QVariant& errorValue ) 
  : d(new ChemicalDataObjectPrivate)
{
    d->m_value = v;
    d->m_errorValue = errorValue;
    d->m_type = type;
    d->m_unit = ChemicalDataObject::noUnit;
}

ChemicalDataObject::ChemicalDataObject() 
  : d(new ChemicalDataObjectPrivate)
{
    d->m_errorValue = QVariant();
    d->m_unit = ChemicalDataObject::noUnit;
}

ChemicalDataObject::ChemicalDataObject(const ChemicalDataObject &other)
  : d(other.d)
{
}

ChemicalDataObject::~ChemicalDataObject()
{
}

ChemicalDataObject& ChemicalDataObject::operator=(const ChemicalDataObject &other)
{
	d = other.d;
	return *this;
}

bool ChemicalDataObject::operator==( const int v ) const
{
	if ( d->m_value.type() != QVariant::Int )	
		return false;
	
	return d->m_value.toInt() == v;
}

bool ChemicalDataObject::operator==( const bool v ) const
{
	if ( d->m_value.type() != QVariant::Bool )	
		return false;
	
	return d->m_value.toBool() == v;
}

bool ChemicalDataObject::operator==( const double v ) const
{
	if ( d->m_value.type() != QVariant::Double )	
		return false;
	
	return d->m_value.toDouble() == v;
}

bool ChemicalDataObject::operator==( const QString& v ) const
{
	if ( d->m_value.type() != QVariant::String )	
		return false;

	return d->m_value.toString() == v;
}

bool ChemicalDataObject::operator==(const ChemicalDataObject &other) const
{
	return d == other.d;
}

bool ChemicalDataObject::operator!=(const ChemicalDataObject &other) const
{
	return d != other.d;
}

QString ChemicalDataObject::valueAsString() const
{
	return d->m_value.toString();
}

ChemicalDataObject::BlueObelisk ChemicalDataObject::type() const
{
	return d->m_type;
}

QVariant ChemicalDataObject::value() const
{
	return d->m_value;
}

QVariant ChemicalDataObject::errorValue() const
{
	return d->m_errorValue;
}

void ChemicalDataObject::setUnit( ChemicalDataObject::BlueObeliskUnit unit )
{
	d->m_unit = unit;
}

ChemicalDataObject::BlueObeliskUnit ChemicalDataObject::unit() const
{
	return d->m_unit;
}

void ChemicalDataObject::setData( const QVariant& v )
{
	d->m_value = v;
}

void ChemicalDataObject::setErrorValue( const QVariant& v )
{
	d->m_errorValue = v;
}

void ChemicalDataObject::setType( BlueObelisk type )
{
	d->m_type = type;
}

void ChemicalDataObject::setType( int type )
{
	d->m_type = ( ChemicalDataObject::BlueObelisk ) type;
}

QString ChemicalDataObject::dictRef() const
{
	QString botype;
	switch ( d->m_type ){
		case atomicNumber:
			botype = "atomicNumber";
			break;
		case symbol:
			botype = "symbol";
			break;
		case name:
			botype = "name";
			break;
		case mass:
			botype = "mass";
			break;
		case exactMass:
			botype = "exactMass";
			break;
		case spin:
			botype = "spin";
			break;
		case magneticMoment:
			botype = "magneticMoment";
			break;
		case halfLife:
			botype = "halfLife";
			break;
		case alphaDecay:
			botype = "alphaDecay";
			break;
		case alphaDecayLikeliness:
			botype = "alphaDecayLikeliness";
			break;
		case betaminusDecayLikeliness:
			botype = "betaminusDecayLikeliness";
			break;
		case betaminusDecay:
			botype = "betaminusDecay";
			break;
		case betaplusDecayLikeliness:
			botype = "betaplusDecayLikeliness";
			break;
		case betaplusDecay:
			botype = "betaplusDecay";
			break;
		case ecDecayLikeliness:
			botype = "ecDecayLikeliness";
			break;
		case ecDecay:
			botype = "ecDecay";
			break;
		case ionization:
			botype = "ionization";
			break;
		case electronAffinity:
			botype = "electronAffinity";
			break;
		case electronegativityPauling:
			botype = "electronegativityPauling";
			break;
		case radiusCovalent:
			botype = "radiusCovalent";
			break;
		case radiusVDW:
			botype = "radiusVDW";
			break;
		case meltingpoint:
			botype = "meltingpoint";
			break;
		case boilingpoint:
			botype = "boilingpoint";
			break;
		case periodTableBlock:
			botype = "periodTableBlock";
			break;
		case nameOrigin:
			botype = "nameOrigin";
			break;
		case orbit:
			botype = "orbit";
			break;
		case date:
			botype = "date";
			break;
		case discoverers:
			botype = "discoverers";
			break;
		case period:
			botype = "period";
			break;
		case relativeAbundance:
			botype = "relativeAbundance";
			break;
		case family:
			botype ="family";
			break;
		case group:
			botype ="group";
			break;
		case acidicbehaviour:
			botype ="acidicbehaviour";
			break;
		case electronicConfiguration:
			botype ="electronicConfiguration";
			break;
		case crystalstructure:
			botype ="crystalstructure";
			break;
		case density:
			botype ="density";
			break;
		case dangerSymbol:
			botype ="dangerSymbol";
			break;
		case RPhrase:
			botype ="RPhrase";
			break;
		case SPhrase:
			botype ="SPhrase";
			break;
		case discoveryCountry:
			botype ="discoveryCountry";
			break;
	}

	botype = botype.prepend( "bo:" );
	
	return botype;
}

QString ChemicalDataObject::unitAsString() const
{
	QString bounit;
	switch ( d->m_unit ){
		case kelvin:
			bounit = "kelvin";
			break;
		case nm:
			bounit = "nm";
			break;
		case pm:
			bounit = "pm";
			break;
		case ev:
			bounit = "ev";
			break;
		case ang:
			bounit = "degree";
			break;
		case noUnit:
			return "noUnit";
		case y:
			return "y";
		case s:
			return "s";
	}
	
	bounit = bounit.prepend( "bo:" );
	
	return bounit;
}
	

ChemicalDataObject::BlueObeliskUnit ChemicalDataObject::unit( const QString& unit )
{
    if ( unit == "siUnits:kelvin" ) 
        return ChemicalDataObject::kelvin;
    else if ( unit == "units:ev" )
        return ChemicalDataObject::ev;
    else if ( unit == "units:ang" )
        return ChemicalDataObject::ang;
    else if ( unit == "bo:noUnit" )
        return ChemicalDataObject::noUnit;
    else
        return ChemicalDataObject::noUnit;
}
