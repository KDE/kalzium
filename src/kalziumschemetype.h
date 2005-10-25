/***************************************************************************
 *   Copyright (C) 2005      by Pino Toscano, toscano.pino@tiscali.it      *
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

#ifndef KALZIUMSCHEMETYPE_H
#define KALZIUMSCHEMETYPE_H

class QPainter;
class KalziumSchemeType;

#include <QByteArray>
#include <QColor>
#include <QGradient>
#include <QPair>
#include <QRect>
#include <QStringList>

typedef QPair<QString, QColor> legendItem;
typedef QList<legendItem> legendList;

/**
 * Factory for KalziumSchemeType classes.
 *
 * @author Pino Toscano
 */
class KalziumSchemeTypeFactory
{
	public:
		/**
		 * Get the instance of this factory.
		 */
		static KalziumSchemeTypeFactory* instance();

		/**
		 * Returns the KalziumSchemeType with the @p id specified.
		 * It will gives 0 if none found.
		 */
		KalziumSchemeType* build( int id ) const;
		/**
		 * Returns the KalziumSchemeType whose name is the @p id
		 * specified.
		 * It will gives 0 if none found.
		 */
		KalziumSchemeType* build( const QByteArray& id ) const;

		/**
		 * Returns a list with the names of the schemes we support.
		 */
		QStringList schemes() const;

	private:
		KalziumSchemeTypeFactory();

		QList<KalziumSchemeType*> m_schemes;
};

/**
 * This is the base class representing a colour scheme.
 *
 * @author Pino Toscano
 */
class KalziumSchemeType
{
	public:
		/**
		 * Get its instance.
		 */
		static KalziumSchemeType* instance();

		virtual ~KalziumSchemeType();

		/**
		 * Returns the ID of this scheme.
		 * Mainly used when saving/loading.
		 */
		virtual QByteArray name() const = 0;
		/**
		 * Returns the description of this scheme.
		 * Used in all the visible places.
		 */
		virtual QString description() const = 0;

		/**
		 * Returns the gradient with the element with atomic number
		 * @p el should be painted.
		 * @param elrect is the rect designed for the element
		 */
		virtual QGradient elementGradient( int el, const QRect& elrect ) const = 0;
		/**
		 * Returns the color which will be used to draw the texts for
		 * the element with atomic number @p el.
		 */
		virtual QColor textColor( int el ) const = 0;

		/**
		 * Returns a list with the legend of the current scheme.
		 */
		virtual legendList legendItems() const = 0;

	protected:
		KalziumSchemeType();
};

/**
 * Class representing the all-one-colour scheme.
 *
 * @author Pino Toscano
 */
class KalziumMonoColorSchemeType : public KalziumSchemeType
{
	public:
		static KalziumMonoColorSchemeType* instance();

		QByteArray name() const;
		QString description() const;

		QGradient elementGradient( int el, const QRect& elrect ) const;
		QColor textColor( int el ) const;

		legendList legendItems() const;

	private:
		KalziumMonoColorSchemeType();
};

/**
 * The scheme by element block.
 *
 * @author Pino Toscano
 */
class KalziumBlocksSchemeType : public KalziumSchemeType
{
	public:
		static KalziumBlocksSchemeType* instance();

		QByteArray name() const;
		QString description() const;

		QGradient elementGradient( int el, const QRect& elrect ) const;
		QColor textColor( int el ) const;

		legendList legendItems() const;

	private:
		KalziumBlocksSchemeType();
};

#endif // KALZIUMSCHEMETYPE_H
