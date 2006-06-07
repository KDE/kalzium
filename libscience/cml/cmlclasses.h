#ifndef CMLCLASSES_H
#define CMLCLASSES_H
/***************************************************************************
 *   Copyright (C) 2006 by Carsten Niehaus                                 *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         * 
 ***************************************************************************/

#include <QString>
#include <QList>

namespace CML
{
	
class Molecule
{
	public:
		Molecule();

};

class Atom
{
	public:
		Atom( );

		void setID( const QString& id ){
			m_id = id;
		}
		
		void setElementType( const QString& et ){
			m_elementType = et;
		}

		void setX2( double i ){
			coord_x2 = i;
		}

		void setX3( double i ){
			coord_x3 = i;
		}

		void setY2( double i ){
			coord_y2 = i;
		}

		void setY3( double i ){
			coord_y3 = i;
		}

		void setZ3( double i ){
			coord_z3 = i;
		}

	private:
		QString m_elementType;
		QString m_id;

		//we need to store five possible coordinates, 2 for 2d, 3 for 3d
		double coord_x2, coord_x3,
			   coord_y2, coord_y3,
			   coord_z3;
};

class Bond
{
	public:
		Bond();

		/**
		 * defines the two Atoms which start and stop the bond
		 */
		void setAtoms( CML::Atom* a1, CML::Atom* a2 ){
			m_start = a1;
			m_end = a2;
		}

		void setOrder( int o ){
			m_order = o;
		}
	
	private:
		Atom* m_start;
		Atom* m_end;

		int m_order;
};

}
#endif // CMLCLASSES_H
