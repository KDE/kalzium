/***************************************************************************
 *   Copyright (C) 2005, 2006 by Carsten Niehaus <cniehaus@kde.org>        *
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

#ifndef KALZIUMDATAOBJECT_H
#define KALZIUMDATAOBJECT_H

#include <element.h>
#include <QHash>
#include <QPixmap>

class Search;

/**
 * @brief This class contains all Element objects
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
         * Set the main Search to @p srch
         */
        void setSearch( Search *srch );

        /**
         * @return the main Search
         */
        Search* search() const;

        /**
         * @return the Element with the number @p number
         * @param number the number of the Element which will be returned
         */
        Element* element( int number );

        /**
         * @return the isotopes of the Element with the number @p number
         */
        QList<Isotope*> isotopes( int number );

        /**
         * @return the isotopes of the Element @p Element
         */
        QList<Isotope*> isotopes( Element * element );

                /**
         * @return the Spectrum of the Element with the number @p number
                 */
                Spectrum * spectrum( int number );

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

        QHash<int, QList<Isotope*> > m_isotopes;
        QList<Spectrum*> m_spectra;

        /**
         * Caching the number of elements
         */
        int m_numOfElements;

        Search *m_search;
};

#endif // KALZIUMDATAOBJECT_H
