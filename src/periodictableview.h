/***************************************************************************
 *   Copyright (C) 2003-2007 by Carsten Niehaus                            *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#ifndef PeriodicTableView_H
#define PeriodicTableView_H

class Element;
class KalziumPainter;
class KalziumGradientType;

#include <QTimer>
#include <QWidget>
#include <QPoint>
#include <QPixmap>
#include <QMap>

#include "kalziumpainter.h"

#include <chemicaldataobject.h>

/**
 * A PeriodicTableView is the widget on which we paint a periodic table.
 *
 * It does not contain any code for painting, as all the painting routines
 * belongs to KalziumPainter.
 *
 * @author Carsten Niehaus
 * @author Pino Toscano
 */
class PeriodicTableView : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent parent widget
     */
    PeriodicTableView( QWidget *parent = 0 );
    ~PeriodicTableView();

    /**
     * Draw the full table next time
     */
    void setFullDraw();

    /**
     * Sets the numeration type to the one with index @p which.
     */
    void setNumeration( int which );

    /**
     * put the limit for the @p type specified, but do not actually (de)activate it
     * @param type type of the @p value
     * @param value the value of the limit for @p type
     */
    void setValueLimit( int value, ChemicalDataObject::BlueObelisk type );

    /**
    * Generate a SVG from the current table and save it in the file @p filename
    */
    void generateSvg(const QString& filename);

    /**
     * Sets the scheme to use.
     * @param nr is the index of the new scheme
     */
    void activateColorScheme( const int nr);

    /**
     * @return the KalziumSchemeType of the current view
     */
    KalziumSchemeType* scheme() const;

    /**
     * @return the short and descriptive name of this PeriodicTableView
     */
    QString shortName() const {
        return m_ShortName;
    }

    /**
     * @return the current gradient type
     */
    KalziumGradientType* gradient() const;

    /**
     * sets te KalziumGradientType
     */
    void setGradient( int which );

    void setMode( KalziumPainter::MODE m );

    /**
     * @return the current KalziumPainter::MODE
     */
    KalziumPainter::MODE mode() const;

    /**
     * @param type the type of value which is searched for
     * @return the value of the @p type
     */
    int sliderValue( ChemicalDataObject::BlueObelisk type );

    /**
     * @return the QBrush for the Element @p element
     */
    QBrush brushForElement( int element ) const;

    KalziumTableType* tableType() const;

private:
    QMap <ChemicalDataObject::BlueObelisk, int> m_sliderValueList;

    /**
     * the currently selected element
     */
    int m_currentElement;

    /**
     * this is a short, descriptive name of the PeriodicTableView
     */
    QString m_ShortName;

    /**
     * implements double buffering of the widget.
     */
    QPixmap *table;			// The basic PeriodicTableView
    QPixmap *table2;		// Basic PeriodicTableView + extra data such as tooltip, etc

    /**
     * used for bitBlit. If true the complete table will be drawn
     */
    bool doFullDraw;

    KalziumPainter *m_painter;

    QPoint m_startDrag;

    /**
     * Used to delay the emit of MouseOver().
     */
    QTimer m_hoverTimer;
    int m_prevHoverElement;

protected:
    virtual void paintEvent( QPaintEvent *e );
    virtual void mouseReleaseEvent( QMouseEvent* );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* );

public slots:
    /**
     * This method sets the color for the buttons corresponding to
     * the given temperature @p temp
     * @param temp is the temperature to which all buttons will be set
     */
    void setTemperature( int temp );

    void setTime( int time );

    void slotChangeTable( int table );

    /**
     * this slot removes the selection of any point
     */
    void unSelect();

private slots:
    /**
     * If called this slot will emit the signal MouseOver( num )
     * where num is the number of the element the mouse if over.
     * If the mouse is not over an element nothing will be emitted
     * @see MouseOver()
     */
    void slotMouseover();

    /**
     * this slot updates the currently selected point
     */
    void selectPoint( const QPoint& point );

    /**
     * this slot updates the element given in the @p num
     * @param num The number of the selected element
     */
    void selectElement( int num );

signals:
    /**
     * this signal is emitted when an element is clicked
     */
    void ElementClicked(int);

    /**
     * This signal is emitted when the mouse pointer is
     * over an element
     */
    void MouseOver( int );

    void ModeChanged( KalziumPainter::MODE );

    void TableTypeChanged( KalziumTableType * );

    void GradientTypeChanged( KalziumGradientType * );

    void SchemeChanged( KalziumSchemeType * );
};


#endif
