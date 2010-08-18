/***************************************************************************
 *   Copyright (C) 2010   by Etienne Rebetez etienne.rebetez@oberwallis.ch *
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


#ifndef KALZIUMELEMENTPROPERTY_H
#define KALZIUMELEMENTPROPERTY_H

#include <QBrush>

#include "kalziumschemetype.h"
#include "kalziumgradienttype.h"

/**
 * The logic of the scheme and the gradients is merged in this class. It provides
 * an api to set the gradient and scheme whitout worring about the real id's.
 * Elements can then get their propertys with the elementfunctions api.
 * Here is also the place where the gradientslider is evaluated. (see gradientwidget)
 * @short This class holds the logic of the appiriance from the periodic table
 * @author Etienne Rebetez
*/

class KalziumElementProperty : public QObject
{
    Q_OBJECT

public:
    KalziumElementProperty();
    ~KalziumElementProperty();

    enum ELEMENTTEXTINFORMATIONMODE
    {
        NORMAL = 0, // Only Symbol Number
        ELNUMBER,
        GRADIENTVALUE
    };

    enum SPECIALGRADIENTYPDEF
    {
       NOGRADIENT = 0,
       SOMGradientType = 1,
       DISCOVERYDATE = 9
    };

    /**
     * all available schemes. Can be used to populate menues.
     */
    QStringList schemeList() const;
    /**
     * all available gradients. Can be used to populate menues.
     */
    QStringList gradientList() const;

    /**
     * Returns the current scheme id as int. Equivalent to the scheme list.
     */
    int schemeId() const;
    /**
     * Returns the current gradient id as int. Equivalent to the gradent list.
     */
    int gradientId() const;

    /**
     * Returns the current real KalziumSchemeType class
     */
    KalziumSchemeType *scheme() const;
    /**
     * Returns the current real KalziumGradientType class
     */
    KalziumGradientType *gradient() const;

    /**
     * This is an Element function.
     * It returns the Brush (Backround) for the given element.
     * @param el Number of the element.
     */
    QBrush getElementBrush(int el);
    /**
     * This is an Element function.
     * It returns the Text color for the given element.
     * @param el Number of the element.
     */
    QColor getTextColor(int el) const;
    /**
     * This is an Element function.
     * It returns the border color of the for the given element.
     * @param el Number of the element.
     */
    QColor getBorderColor(int el) const;

    /**
     * This is an Element function.
     * It returns the value from the current gradent type for the given element.
     * @param el Number of the element.
     */
    double getValue(int el) const;

    /**
     * This is an Element function.
     * The mode is reflects the enum ELEMENTTEXTINFORMATIONMODE.
     * It is used to use different layouts in the elementitem.
     */
    int getMode() const;

Q_SIGNALS:
    /**
     * Is emitet every time a property (scheme, gradent, ...) is changed.
     * all elements will be redrawn.
     */
    void propertyChanged();

public slots:
    /**
     * gets the value which is used to determin if an element is activ or not.
     * The value has to be factor 1000 bigger, so that decimals can be given as int's.
     * @param slide value of the current gradient * 1000
     */
    void setSliderValue(double slide);
    /**
     * sets the new scheme
     */
    void setScheme(int newScheme);
    /**
     * sets the new gradient
     */
    void setGradient(int newGradient);
    /**
     * Used to trigger a redraw of the periodic system (all elements).
     * just emits propertyChanged().
     */
    void redrawPse();

private:
    void gradientBrushLogic(QBrush *brush, int el);
    KalziumSchemeTypeFactory *m_scheme;
    KalziumGradientTypeFactory *m_gradient;

    int m_currentScheme;
    int m_currentGradient;

    double m_slider;
    int m_mode;
};





#endif // KALZIUMELEMENTPROPERTY_H
