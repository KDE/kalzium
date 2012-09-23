#ifndef KALZIUMUTILS_H
#define KALZIUMUTILS_H

/***************************************************************************
    copyright            : (C) 2005, 2006 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

class QString;
class QRect;
class QFont;
class QPainter;
class Element;

#include <chemicaldataobject.h>
#include <QComboBox>

class KalziumUtils
{
public:
    /**
     * @param string the string which is measured
     * @param rect the rect needed for measurement
     * @param font the used font
     * @param painter the used painter
     * @param maxFontSize the maximum fontsize
     * @param minFontSize the maximum fontsize
     */
    static int maxSize( const QString& string, const QRect& rect, QFont font, QPainter* painter, int minFontSize = 4, int maxFontSize = 20 );

    /**
     * calculate the 4-digit value of the value @p w. For
     * example, 12.3456 will be returned as 12.35
     * @param value the value which will be stripped
     * @return the 4-digit value
     */
    static double strippedValue( double value );

    /**
     * @param string the string which is measured
     * @param font the used font
     * @param painter the used painter
     * @return the width of the string @p string
     */
    static int StringWidth( const QString& string, const QFont& font, QPainter* painter );
    /**
     * @param string the string which is measured
     * @param font the used font
     * @param painter the used painter
     * @return the height of the string @p string
     */
    static int StringHeight( const QString& string, const QFont& font, QPainter* painter );

    /**
     * An almost standard way to get an unit well converted for
     * displaying including (eventual) unit.
     * @param el the element
     * @param kind the unit we want to print of the element @p el
     * @return the formatted string
     */
    static QString prettyUnit( const Element* el, ChemicalDataObject::BlueObelisk kind );

    /**
     * Populates a a combobox wiht the names of the units. @see KUnitConversion.
     * @param combobox the pointer to the combobox
     * @param unitList Unit list with enums of KUnitConversion
     */
    static void populateUnitCombobox(QComboBox *comboBox, const QList< int > &unitList);

};

#endif // KALZIUMUTILS_H

