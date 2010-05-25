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

#ifndef KALZIUMPAINTER_H
#define KALZIUMPAINTER_H

#include <QByteArray>
#include <QRect>

#include <chemicaldataobject.h>

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
    virtual ~KalziumPainter() {}

    void setTableType( KalziumTableType* ktt ) {
        m_ktt = ktt;
    }

    enum MODE
    {
        NORMAL = 0,
        SOM /**< State of matter*/,
        TIME/** Timeline*/,
        GRADIENT,
        NORMAL_GRADIENT /** Schema + Gradient*/
    };

    /**
     * Begin painting on the painting device @p pd.
     */
    void begin( QPaintDevice *pd );
    /**
     * Finish painting.
     */
    void end();

    KalziumTableType* currentTableType() const;

    /**
     * Draw all the things it can: elements and numeration
     */
    void drawAll();

    /**
     * Draw all the elements
     */
    void drawElements();

    /**
     * Draw the element with atomic number @p element.
     * Pass a QRect() to the @p r parameter if you want to use the
     * default rect for that element.
     */
    void drawElement( int element, const QRect& r = QRect() );

    /**
     * Draw the numeration
     */
    void drawNumeration();

    /**
     * Paint a marker around the element number @p element
     */
    void drawElementSelector( int element );

    void setMode( MODE m );

    /**
     * @return Return the current MODE
     */
    MODE mode() const {
        return m_mode;
    }

    void setScheme( int s );
    void setScheme( const QByteArray& s );
    KalziumSchemeType* scheme() const;

    void setGradient( int cs );
    void setGradient( const QByteArray& cs );
    KalziumGradientType* gradient() const;

    /**
     * Set the numeration @p n for the periodic table
     */
    void setNumeration( int n );
    void setNumeration( const QByteArray& n );

    KalziumNumerationType* numeration() const {
        return m_numeration;
    }

    void setTemperature( int temp );

    int temperature() const {
        return m_temperature;
    }

    void setTime( int time ) {
        m_time = time;
    }

    int time() const {
        return m_time;
    }

    QBrush brushForElement( int element ) const;

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

    ///for the state-of-matter
    int m_temperature;

    ///for the timeline
    int m_time;
};

#endif // KALZIUMPAINTER_H
