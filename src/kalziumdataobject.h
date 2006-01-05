#ifndef KALZIUMDATAOBJECT_H
#define KALZIUMDATAOBJECT_H
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

#include <element.h>
#include <QPixmap>

/**
 * @short This class contains all Element objects
 *
 * This singleton class collects all the information about the elements of the
 * Periodic Table as list of Element.
 *
 * Use:
 * @code
 * KalziumDataObject::instance()->ElementList;
 * @endcode
 * to get the whole list of Element, while a
 * @code
 * KalziumDataObject::instance()->element( num );
 * @endcode
 * will return you the pointer to the num'th element of the Periodic Table.
 *
 * @author Carsten Niehaus
 */
class KalziumDataObject
{
	public:
		/**
		 * @return the instance of this class
		 */
		static KalziumDataObject* instance();

		/**
		 * The list of elements
		 */
		QList<Element*> ElementList;

		/**
		 * Store the matching elements
		 */
		void findElements( const QString& name );

		/**
		 * @return true if @p e matches the searched string
		 */
		bool elementMatchesSearch( Element* e );
		
		/**
		 * @return the Element with the number @p number
		 * @param number the number of the Element which will be returned
		 */
		Element* element( int number );
	
		QPixmap pixmap( int number );
		
		/**
		 * Use this to get the number of elements we have. It is cached
		 * so you are strongly suggested to use this instead of hardcode
		 * the number of elements.
		 * @return the number of elements we have
		 */
		int numberOfElements() const { return m_numOfElements; }

	private:
		KalziumDataObject();
		~KalziumDataObject();
	
		QList<QPixmap> PixmapList;

		QList<Element*> m_searchList;

		/**
		 * Caching the number of elements
		 */
		int m_numOfElements;
};
#endif // KALZIUMDATAOBJECT_H
