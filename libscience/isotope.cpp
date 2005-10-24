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

Isotope::Isotope( ChemicalDataObject* mass,
		ChemicalDataObject* ID )
{
	m_mass = mass;
	m_identifier = ID;
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

	return;
}

double Isotope::mass() const
{
	return m_mass->value().toDouble();
}

QString Isotope::errorMargin() const
{
	return m_mass->errorValue().toString();
}

int Isotope::parentElementNumber() const
{
	return m_identifier->value().toInt();
}

QString Isotope::parentElementSymbol() const
{
	return m_parentElementSymbol;
}
