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
			SQUARE,
			HEX
		};
		
		/**
		 * @return the number of stones of the other player
		 * @param stone The stone of which the neighbours are counted.
		 * @param direct if true, only the direct neighbours are counted. Otherwise, also the orthogonal stones will be looked at. Only needed if m_fieldtype is Field::SQUARE.
		 */
		virtual int neighboursNum( Stone* stone, bool direct = true );
		
		/**
		 * @return the number of stones of the player
		 * @param stone The stone of which the neighbours are counted.
		 * @param direct if true, only the direct neighbours are counted. Otherwise, also the orthogonal stones will be looked at. Only needed if m_fieldtype is Field::SQUARE.
		 */
		virtual int neighboursTeam( Stone* stone, bool direct = true );

		/**
		 * Exchange the Stone at the postion @p point with one stone
		 * of the other team. That other Stone has to be in orthogonal
		 * contact with the Stone in @p point
		 * @param direct if true, only the direct neighbours are taken into regard. Otherwise, also the orthogonal stones will be looked at. Only needed if m_fieldtype is Field::SQUARE.
		 */
		virtual void exchangeStones( const QPoint& point, bool direct = true );
		
		/**
		 * Constructor
		 * @param type the FIELDTYPE of the Field. Defaults to Field::SQUARE
		 */
		Field( FIELDTYPE type = Field::SQUARE );

		void setFieldXSize( int x ){
			m_size_x = x;
		}
		
		void setFieldYSize( int y ){
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
		 * removes the stone from the field
		 */
		virtual void removeStone( Stone * stone );

		/**
		 * @return the Stone at the position @p pos
		 */
		virtual Stone* stoneAtPosition( const QPoint& pos );
		
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
		 * @return the tokens on position @p pos
		 */
		QList<Stone*> tokensOnPosition( const QPoint& pos );
		
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
