#ifndef FIELD_H
#define FIELD_H
/***************************************************************************
 *   Copyright (C) 2005,2006 by Carsten Niehaus,    cniehaus@kde.org       *
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
class Stone;

#include "stone.h"

#include <QPoint>
#include <QList>

/**
 * Each Simulation is graphically represented by a Field. The games implement
 * a specific Field, this is the baseclass for these classes. A field stores
 * the information about the size of the game and what stone are on
 * what position
 *
 * @author Carsten Niehaus
 * @brief The playground of a game
 */
class Field
{
	public:
		/**
		 * Defines if the cells will be squares or hex
		 */
		enum FIELDTYPE
		{
			SQUARE/**<for qubic fields (imagine chess)*/,
			HEX/**<for hexagonal fields (imagine most strategic games)*/
		};

		/**
		 * @return a list of points surrounding the Stone on point @p p
		 * @param stone The stone of which the neighbours are gathered.
		 * @param direct if true, only the direct neighbours are counted. Otherwise, also the orthogonal stones will be looked at. Only needed if m_fieldtype is Field::SQUARE.
		 */
		virtual QList<Stone*> neighbours( Stone* stone , bool direct = true );
		
		/**
		 * @return the number of stones of the player. Stone of other teams are not counted.
		 * @param stone The stone of which the neighbours are counted.
		 * @param direct if true, only the direct neighbours are counted. Otherwise, also the orthogonal stones will be looked at. Only needed if m_fieldtype is Field::SQUARE.
		 */
		virtual QList<Stone*> neighboursTeam( Stone* stone, bool direct = true );

		/**
		 * Exchange the Stone at the postion @p point with one stone
		 * of the other team. If @p direct is true all neighbours will be taken
		 * into account if m_fieldtype is Field::SQUARE. For Field::HEX this is 
		 * not needed as all stones around @p stone are equivalent.
		 *
		 * The Stone which is swapped is choosen randomly. For example, if the Stone
		 * @p stone has 3 neighbours of the other team one of the three is taken.
		 *
		 * @param point The stone on this point will be exchanged
		 * @param direct if true, only the direct neighbours are taken into regard. Otherwise, also the orthogonal stones will be looked at. Only needed if m_fieldtype is Field::SQUARE.
		 */
		virtual void exchangeStones( const QPoint& point, bool direct = true );

		/**
		 * If this method returns the same point as @p p then no free cell was found
		 * @return Returns a free cell next to the point @p p
		 * @param p the point a which a free neighbourcell is searched for
		 */
		QPoint freeNeighbourCell( const QPoint& p );
		
		/**
		 * Constructor
		 * @param type the FIELDTYPE of the Field. Defaults to Field::SQUARE
		 */
		Field( FIELDTYPE type = Field::SQUARE );

		/**
		 * Sets the size of the Field
		 * @param x the vertical size of the field
		 * @param y the horizontal size of the field
		 */
		void setFieldSize( int x, int y ){
			m_size_x = x;
			m_size_y = y;
		}
		
		/**
		 * @return the FIELDTYPE of the field
		 */
		FIELDTYPE fieldtype() const{
			return m_fieldtype;
		}

		/**
		 * @return the x-size of the field
		 */
		int xSize() const {
			return m_size_x;
		}
		
		/**
		 * @return the y-size of the field
		 */
		int ySize() const {
			return m_size_y;
		}
		
		/**
		 * Destructor
		 */
		virtual ~Field();

		/**
		 * Add the Stone @p stone to position @p pos
		 */
		virtual void addStone( Stone* stone ){
			m_stones.append( stone );
		}

		/**
		 * removes the stone @p stone from the field
		 * @param stone The Stone to remove
		 */
		virtual void removeStone( Stone * stone );

		/**
		 * @return the Stone at the position @p pos
		 */
		virtual Stone* stoneAtPosition( const QPoint& pos );

		/**
		 * @return a random stone of the player @p player
		 */
		virtual Stone* randomStone( Stone::PLAYER player );

		
		/**
		 * @return the stones of the game
		 */
		virtual QList<Stone*> stones() const
		{
			return m_stones;
		}

		/**
		 * removes all stones from the field
		 */
		virtual void clear();

		/**
		 * this method will print the current field in ASCII-code on STDOUT
		 */
		void debugOutput();

	protected:
		int m_size_x;
		
		int m_size_y;

		FIELDTYPE m_fieldtype;
		
		/**
		 * The list of stones
		 */
		QList<Stone*> m_stones;
		
};


#endif // FIELD_H
