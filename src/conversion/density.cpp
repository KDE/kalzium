/*
 *   Copyright (C) 2009 Petri Damstén <damu@iki.fi>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "density.h"
#include <KLocale>

Density::Density(QObject* parent)
: Conversion::UnitCategory(parent)
{
    setObjectName("density");
    setName(i18n("Density"));
    addSIUnit("Kg/m\xb3", i18n("kilogram per cubic meter"), i18n("kilograms per cubic meter"));
	//	source http://www.coleparmer.com/techinfo/converters/engndensity.asp
    U(i18n("kilogram per liter"), i18n("kilograms per liter"), "kg/l", 1000.0, );
    U(i18n("gram per liter"), i18n("grams per liter"), "g/l", 1.0, );
    U(i18n("gram per milliliter"), i18n("grams per milliliter"), "g/ml", 1000.0, );
    U(i18n("ounce per cubic inch"), i18n("ounces per cubic inch"), "oz/in\xb3", 1729.994044387, );
    U(i18n("ounce per cubic foot"), i18n("ounces per cubic yard"), "lb/yd\xb3", 1.001153961, );        
    U(i18n("pound per cubic inch"), i18n("pounds per cubic inch"), "lb/in\xb3", 27679.904710191, );
    U(i18n("pound per cubic foot"), i18n("pounds per cubic foot"), "lb/ft\xb3", 16.018463374, );
    U(i18n("pound per cubic yard"), i18n("pounds per cubic yard"), "lb/yd\xb3", 0.593276421, );

    
}
