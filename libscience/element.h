/***************************************************************************
 *   Copyright (C) 2003, 2004, 2005 by Carsten Niehaus                     *
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

#ifndef ELEMENT_H
#define ELEMENT_H

#include "libkdeedu_science_export.h"

#include <QtCore/QList>
#include <QtCore/QVariant>

#include "chemicaldataobject.h"

class Spectrum;
class Isotope;

/**
 * In this class all information about an element are stored. This means that
 * both the chemical date and the data about the position are stored
 * in this class.
 * @short This class is the represention of a chemical element
 * @author Carsten Niehaus
*/
class SCIENCE_EXPORT Element
{
	public:
		Element();

		virtual ~Element();
		
		/**
		 * Add the ChemicalDataObject @p o to this Element
		 * @param o the ChemicalDataObject to be added
		 */
		void addData( const ChemicalDataObject& o );

		/**
		 * Add a ChemicalDataObject with @p value of @p type to this
		 * Element
		 * @param value the QVariant to be added
		 * @param type the BlueObelisk type to be added
		 */
		void addData( const QVariant& value, ChemicalDataObject::BlueObelisk type );

		/**
		 * @return the requested data of the type @p type as a QVariant
		 */
		QVariant dataAsVariant( ChemicalDataObject::BlueObelisk type ) const;
		
		/**
		 * @return the requested data of the type @p type as a QString
		 */
		QString dataAsString( ChemicalDataObject::BlueObelisk type ) const;
		
		/**
		 * @return the data of the Element
		 */
		QList<ChemicalDataObject> data()const{
			return dataList;
		}

	private:
		/**
		 * this QList stores all information about an element
		 */
		QList<ChemicalDataObject> dataList;
};

#endif
