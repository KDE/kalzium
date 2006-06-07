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

#include <libkdeedu_science_export.h>

#include <QString>
#include <QList>

namespace CML
{

class Atom;
class Bond;
	
class EDUSCIENCE_EXPORT Molecule
{
	public:
		Molecule();

		void setAtoms( QList<CML::Atom*> list ){
			m_atoms = list;
		}

		void setBonds( QList<CML::Bond*> list ){
			m_bonds = list;
		}

		void setID( const QString& id ){
			m_id = id;
		}

		QString debug();

	private:
		QList<CML::Atom*> m_atoms;
		QList<CML::Bond*> m_bonds;

		QString m_id;
};

class EDUSCIENCE_EXPORT Atom
{
	public:
		Atom( );

		QString debug(){
			QString db = "ID: " + m_id + " Type: " + m_elementType + " Coordinates: " + "x3: " + QString::number( coord_x3 ) + " y3: " + QString::number( coord_y3 )+ " z3:" + QString::number( coord_z3 ) + "\n";
			return db;
		}

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

		QString id() const{
			return m_id;
		}

	private:
		QString m_elementType;
		QString m_id;

		//we need to store five possible coordinates, 2 for 2d, 3 for 3d
		double coord_x2, coord_x3,
			   coord_y2, coord_y3,
			   coord_z3;
};

class EDUSCIENCE_EXPORT Bond
{
	public:
		Bond();
		
		QString debug(){
			QString db = "Atom 1: " + m_startatom_id + " ... Atom 2: " + m_endatom_id;
			return db;
		}


		/**
		 * defines the two Atoms which start and stop the bond
		 */
		void setAtoms( const QString& id1, const QString& id2 ){
			m_startatom_id = id1;
			m_endatom_id = id2;
		}

		void setOrder( int o ){
			m_order = o;
		}
	
	private:
		QString m_startatom_id;
		QString m_endatom_id;

		int m_order;
};

}
#endif // CMLCLASSES_H
