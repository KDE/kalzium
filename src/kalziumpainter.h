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
 * @author Pino Toscano
 */
class KalziumPainter
{
	public:
		KalziumPainter( KalziumTableType *ktt );
		~KalziumPainter();

		enum MODE
		{
			NORMAL = 0,
			GRADIENT = 1,
			SOM
		};

		void begin( QPaintDevice *pd = 0 );
		void end();

		KalziumTableType* currentTableType();

		void drawAll();
		void drawElements();
		void drawElement( int element );
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
