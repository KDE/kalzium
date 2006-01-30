/***************************************************************************
 *   Copyright (C) 2006      by Carsten Niehaus,    cniehaus@kde.org       *
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
#ifndef STONE_H
#define STONE_H

#include <QObject>
#include <QPoint>

/**
 * @author Carsten Niehaus
 * @brief A token to play a game
 *
 */
class Stone : public QObject
{
	Q_OBJECT

	public:
		enum PLAYER
		{
			Black = 0,
			White
		};

		/**
		 * if the stone is black make it white and vice versa
		 */
		void swap();
		
		/**
		 * Set the kind of player to @p player
		 */
		Stone( PLAYER player, const QPoint& point );

		/**
		 * set the player to @p player
		 */
		void setPlayer( PLAYER player )
		{
			m_player = player;
		}

		/**
		 * @return the player of this stone
		 */
		PLAYER player() const
		{
			return m_player;
		}

		/**
		 * @return the position of the stone
		 */
		QPoint position() const
		{
			return m_position;
		}

		/**
		 * set the position to @p point
		 */
		void setPosition( const QPoint& point )
		{
			m_position = point;
			emit moved( m_position );
		}

	private:
		PLAYER m_player;

		QPoint m_position;

	signals:
		void removed();
		void moved(QPoint);
};

#endif // STONE_H
