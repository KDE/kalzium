#ifndef ISOTOPE_H
#define ISOTOPE_H
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

class ChemicalDataObject;

#include <QString>

/**
 * @author Carsten Niehaus
 *
 * This class represents an Isotope with all its properties
 */
class Isotope
{
	public:
		Isotope();
		Isotope(ChemicalDataObject* mass, ChemicalDataObject* ID);
		virtual ~Isotope();

		ChemicalDataObject* data() const;

		double mass() const;

		QString errorMargin() const;

		int parentElementNumber() const;

		QString parentElementSymbol() const;

		void addData( ChemicalDataObject* o );

		void setParentSymbol( const QString& symbol );

	private:
		/**
		 * the symbol of the element the isotope belongs to
		 */
		QString m_parentElementSymbol;
		
		/**
		 * stores the infomation about the mass of the Isotope
		 */
		ChemicalDataObject* m_mass;

		/**
		 * stores the atomicNumber of the Isotope
		 */
		ChemicalDataObject* m_identifier;
};

#endif // ISOTOPE_H
