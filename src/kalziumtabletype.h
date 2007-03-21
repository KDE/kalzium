/***************************************************************************
 *   Copyright (C) 2005, 2006      by Pino Toscano, toscano.pino@tiscali.it      *
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
		 * Returns the rect where to put the legend.
		 */
		virtual QRect legendRect() const = 0;
		/**
		 * Returns the rect for the @p numelement 'th item of the
		 * numeration @p nt.
		 */
		virtual QRect numerationRect( const int numelem, KalziumNumerationType *nt ) const = 0;

        /**
          * @return The list of elements this table contains
          */
        virtual QList<int> elementList() const = 0;

	protected:
		KalziumTableType();
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
		QRect legendRect() const;
		QRect numerationRect( const int numelem, KalziumNumerationType *nt ) const;
        
        QList<int> elementList() const;

	private:
		KalziumClassicTableType();

		/**
		 * @return the coordinates of the element under the point
		 * @p coords.
		 * For example, H will be 1/1 and Li will be 1/2
		 */
		QPoint elementUnderMouse( const QPoint& coords ) const;

        QList<int> m_elementList;
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
		QRect legendRect() const;
		QRect numerationRect( const int numelem, KalziumNumerationType *nt ) const;
        
        QList<int> elementList() const;

	private:
		KalziumShortTableType();

		/**
		 * @return the coordinates of the element under the point
		 * @p coords.
		 * For example, H will be 1/1 and Li will be 1/2
		 */
		QPoint elementUnderMouse( const QPoint& coords ) const;
        
        QList<int> m_elementList;

        static int translateToShort(int num);
};

#endif // KALZIUMTABLETYPE_H
