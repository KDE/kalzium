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
#include "isotope.h"
#include "chemicaldataobject.h"

#include <kdebug.h>
#include <klocale.h>

Isotope::Isotope()
	: m_mass( 0 ), m_identifier( 0 )
{ 
}

Isotope::Isotope( ChemicalDataObject* mass, ChemicalDataObject* ID )
	: m_parentElementSymbol( 0 ), m_mass( 0 ), m_identifier( 0 ),
	m_spin( 0 ), m_magmoment( 0 ), m_halflife( 0 )
{
	addData( mass );
	addData( ID );
}

Isotope::~Isotope(){}


ChemicalDataObject* Isotope::data() const
{
	return m_mass;
}

void Isotope::addData( ChemicalDataObject* o )
{
	if ( o->type() == ChemicalDataObject::exactMass )
		m_mass = o;
	else if ( o->type() == ChemicalDataObject::atomicNumber )
		m_identifier = o;
	else if ( o->type() == ChemicalDataObject::symbol )
		m_parentElementSymbol = o;
	else if ( o->type() == ChemicalDataObject::spin )
		m_spin = o;
	else if ( o->type() == ChemicalDataObject::magneticMoment )
		m_magmoment = o;
	else if ( o->type() == ChemicalDataObject::halfLife )
		m_halflife = o;
}

double Isotope::mass() const
{
	return m_mass ? m_mass->value().toDouble() : -1.0;
}

QString Isotope::errorMargin() const
{
	return m_mass ? m_mass->errorValue().toString() : QString();
}

int Isotope::parentElementNumber() const
{
	return m_identifier ? m_identifier->value().toInt() : -1;
}

QString Isotope::spin() const
{
	return m_spin ? m_spin->value().toString() : QString();
}

QString Isotope::magmoment() const
{
	return m_magmoment ? m_magmoment->value().toString() : QString();
}

double Isotope::halflife() const
{
	return m_halflife ? m_halflife->value().toDouble() : -1.0;
}

QString Isotope::parentElementSymbol() const
{
	return m_parentElementSymbol ? m_parentElementSymbol->value().toString() : QString();
}
