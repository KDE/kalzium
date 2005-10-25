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

#ifndef KALZIUMPAINTER_H
#define KALZIUMPAINTER_H

class QPaintDevice;
class QPainter;
class KalziumGradientType;
class KalziumTableType;
class KalziumSchemeType;
class KalziumNumerationType;

/**
 * The KalziumPainter is /the/ way to draw a table, any kind of it.
 *
 * One of the best features of it is that you can construct it once and do
 * multiple drawing session without destroying it.
 *
 * A typical usage would be:
 * @code
 * // build the classic periodic table
 * KalziumTableType *table = KalziumTableTypeFactory::instance()->build( "Classic" );
 * // creating the painter with the table type
 * KalziumPainter painter = new KalziumPainter( table );
 * // setting the various options
 * ...
 * painter->begin( widget/pixmap/paintdevice );
 * painter->drawAll();
 * // draw the element selector for the element with atomic number 20 (Calcium)
 * painter->drawElementSelector( 20 );
 * painter->end();
 * @endcode
 *
 * @author Pino Toscano
 */
class KalziumPainter
{
	public:
		/**
		 * Construct a new KalziumPainter for the table type @p ktt.
		 */
		KalziumPainter( KalziumTableType *ktt );
		~KalziumPainter();

		enum MODE
		{
			NORMAL = 0,
			GRADIENT = 1,
			SOM
		};

		/**
		 * Begins painting on the painting device @p pd.
		 */
		void begin( QPaintDevice *pd );
		/**
		 * Finish painting.
		 */
		void end();

		KalziumTableType* currentTableType();

		void drawAll();
		void drawElements();
		void drawElement( int element );
		/**
		 * draw the legend
		 */
		void drawLegend( bool force = false );
		void drawNumeration();

		/**
		 * Paint a marker around the element number @p element
		 */
		void drawElementSelector( int element );

		void toggleLegend( bool active );
		bool legendShown() const;

		void setMode( MODE m );
		MODE mode() const;

		void setScheme( int s );
		KalziumSchemeType* scheme() const;

		void setGradient( int cs );
		KalziumGradientType* gradient() const;

		/**
		 * Set the numeration @p n for the periodic table
		 */
		void setNumeration( int n );
		KalziumNumerationType* numeration() const;

		void setDate( int year );
		int date() const;
		void setTimeline( bool timeline );
		bool timeline() const;

		void setTemperature( int temp );
		int temperature() const;

	private:
		QPainter *m_painter;

		KalziumTableType *m_ktt;

		MODE m_mode;

		KalziumSchemeType *m_scheme;
		KalziumGradientType *m_gradient;
		/**
		 * the type of the numeration
		 */
		KalziumNumerationType *m_numeration;

		int m_year;
		bool m_isTimeline;

		bool m_legend;
		bool m_legendNeedRepaint;
		bool m_elementsNeedRepaint;

		int m_temperature;
};

#endif // KALZIUMPAINTER_H
