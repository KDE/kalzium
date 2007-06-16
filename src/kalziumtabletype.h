/***************************************************************************
 *   Copyright (C) 2005, 2006     by Pino Toscano, toscano.pino@tiscali.it *
 *   Copyright (C) 2007           by Carste Niehaus, cniehaus@kde.org      *
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

#ifndef KALZIUMTABLETYPE_H
#define KALZIUMTABLETYPE_H

class KalziumNumerationType;
class KalziumTableType;

#include <QByteArray>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QStringList>

/**
 * Factory for KalziumTableType classes.
 *
 * @author Pino Toscano
 */
class KalziumTableTypeFactory
{
	public:
		/**
		 * Get the instance of this factory.
		 */
		static KalziumTableTypeFactory* instance();

		/**
		 * Returns the KalziumTableType with the @p id specified.
		 * It will gives 0 if none found.
		 */
		KalziumTableType* build( int id ) const;

		/**
		 * Returns the KalziumTableType whose name is the @p id
		 * specified.
		 * It will gives 0 if none found.
		 */
		KalziumTableType* build( const QByteArray& id ) const;

		/**
		 * Returns a list with the names of the table types we support.
		 */
		QStringList tables() const;
	private:
		KalziumTableTypeFactory();

		QList<KalziumTableType*> m_tables;
};

/**
 * This is one of the most important classes. It represents a table type, with
 * all its attributes and metrics.
 *
 * @author Pino Toscano
 */
class KalziumTableType
{
	public:
		/**
		 * Get its instance.
		 */
		static KalziumTableType* instance();

		virtual ~KalziumTableType();

		/**
		 * Returns the ID of this table type.
		 * Mainly used when saving/loading.
		 */
		virtual QByteArray name() const = 0;
		/**
		 * Returns the description of this table type.
		 * Used in all the visible places.
		 */
		virtual QString description() const = 0;

		/**
		 * Returns the size of this table type.
		 */
		virtual QSize size() const = 0;

		/**
		 * @return the number of the element at the coordinates @p coords.
		 * If there is no element it will return 0.
         * For example, for Carbon "6" will be returned
		 */
		virtual int elementAtCoords( const QPoint& coords ) const = 0;
		/**
		 * Returns the rect for the element with atomic number @p numelem.
		 */
		virtual QRect elementRect( const int numelem ) const = 0;

		/**
		 * Returns the rect for the @p numelement 'th item of the
		 * numeration @p nt.
		 */
		virtual QRect numerationRect( const int numelem, KalziumNumerationType *nt ) const;

		/**
		 * Returns the element that comes right before the specified @p element.
		 * -1 means that @p element is the first in this table type.
		 *
		 * The default implementation returns <tt>element - 1</tt> if @p element
		 * is not 1, else -1.
		 */
		virtual int previousOf( int element ) const;

		/**
		 * Returns the element that comes right after the specified @p element.
		 * -1 means that @p element is the last in this table type.
		 *
		 * The default implementation returns <tt>element + 1</tt> if @p element
		 * is not the latest element, else -1.
		 */
		virtual int nextOf( int element ) const;

		/**
		 * Returns the first element of the table.
		 *
		 * The default implementation returns 1.
		 */
		virtual int firstElement() const;
        
        /**
        * @return the list of elements the specific KalziumTableType displays
        */
        virtual QList<int> elementList() const;

	protected:
		KalziumTableType();

                /**
                 * This QList stores the numbers of the elements the table will display
                 */
                QList<int> m_elementList;

		/**
		 * @return the coordinates of the element under the point
		 * @p coords.
		 * For example, H will be 1/1 and Li will be 1/2
		 */
		QPoint elementUnderMouse( const QPoint& coords ) const;
};

/**
 * The class representing the "classic" periodic table, and its metrics.
 *
 * @author Pino Toscano
 */
class KalziumClassicTableType : public KalziumTableType
{
	public:
		static KalziumClassicTableType* instance();

		QByteArray name() const;
		QString description() const;

		QSize size() const;

		int elementAtCoords( const QPoint& coords ) const;
		QRect elementRect( const int numelem ) const;

	private:
		KalziumClassicTableType();
};

/**
 * The class representing the "short" periodic table, and its metrics.
 *
 * @author Carsten Niehaus
 */
class KalziumShortTableType : public KalziumTableType
{
	public:
		static KalziumShortTableType* instance();

		QByteArray name() const;
		QString description() const;

		QSize size() const;

		int elementAtCoords( const QPoint& coords ) const;
		QRect elementRect( const int numelem ) const;

		int previousOf( int element ) const;
		int nextOf( int element ) const;

	private:
		KalziumShortTableType();

        /**
          * @return The number of the element in this scheme. As we skip
          *         the d- and f-block, for example element 31 is infact the 21st
                    element in this scheme (because elements 21 to 30 are skipped)
         */
        static int translateToShort(int num);
};

/**
 * The class representing the d-Block of the periodic table, and its metrics.
 *
 * @author Carsten Niehaus
 */
class KalziumDTableType : public KalziumTableType
{
	public:
		static KalziumDTableType* instance();

		QByteArray name() const;
		QString description() const;

		QSize size() const;

		int elementAtCoords( const QPoint& coords ) const;
		QRect elementRect( const int numelem ) const;

		int previousOf( int element ) const;
		int nextOf( int element ) const;
		
        int firstElement() const;

	private:
		KalziumDTableType();

        /**
          * @return The number of the element in this scheme. As we skip
          *         the s-, p and f-block, for example element 1st is infact the 21st
                    element in this scheme
         */
        static int translateToD(int num);
};

/**
 * This class represents the table the DZ (Deutscher Zentralausschuss, "German Central Comitee")
 * suggests.
 *
 * @author Carsten Niehaus
 */
class KalziumDZTableType : public KalziumTableType
{
	public:
		static KalziumDZTableType* instance();

		QByteArray name() const;
		QString description() const;

		QSize size() const;

		int elementAtCoords( const QPoint& coords ) const;
		QRect elementRect( const int numelem ) const;

	private:
		KalziumDZTableType();
};

#endif // KALZIUMTABLETYPE_H
