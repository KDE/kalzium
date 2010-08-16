/***************************************************************************
 *   Copyright (C) 2010     by Etienne Rebetez etienne.rebetez@oberwallis.ch    *
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

#include "kalziumelementproperty.h"

#include "kalziumdataobject.h"
#include "search.h"

#include "klocale.h"

#include <prefs.h>
#include "qdebug.h"

KalziumElementProperty::KalziumElementProperty()
        : m_mode(0)
{
    m_gradient = KalziumGradientTypeFactory::instance();
    m_scheme = KalziumSchemeTypeFactory::instance();

    m_currentScheme = Prefs::colorschemebox();
    m_currentGradient = Prefs::colorgradientbox();

    if (m_currentGradient > 1) {
        m_mode = GRADIENTVALUE;
    }
}


KalziumElementProperty::~KalziumElementProperty()
{
//     delete m_scheme;
//     delete m_gradient;
}



void KalziumElementProperty::setScheme(int newScheme)
{
    m_currentScheme = newScheme;
    Prefs::setColorschemebox(newScheme);
    Prefs::self()->writeConfig();
    propertyChanged();
}

void KalziumElementProperty::setGradient(int newGradient)
{
    m_currentGradient = newGradient;
    Prefs::setColorgradientbox(newGradient);
    Prefs::self()->writeConfig();

    if (m_currentGradient > 1) {
        m_mode = GRADIENTVALUE;
    } else {
        m_mode = NORMAL;
    }

    propertyChanged();
}

void KalziumElementProperty::redrawPse()
{
    propertyChanged();
}

QStringList KalziumElementProperty::schemeList() const
{
    return m_scheme->schemes();
}

QStringList KalziumElementProperty::gradientList() const
{
    QStringList customList;
    customList << i18n("None");
    customList << m_gradient->gradients();
    return customList;
}


KalziumSchemeType* KalziumElementProperty::scheme() const
{
    return m_scheme->build(m_currentScheme);
}

KalziumGradientType* KalziumElementProperty::gradient() const
{
    if (m_currentGradient == NOGRADIENT ) {
        // FIXME Fix the case when no gradient is choosen. gradientwidget or here.
        return m_gradient->build(0);
    }

    return m_gradient->build(m_currentGradient - 1);
}

int KalziumElementProperty::gradientId() const
{
    return m_currentGradient;
}

int KalziumElementProperty::schemeId() const
{
    return m_currentScheme;
}



void KalziumElementProperty::setSliderValue(int slide)
{
    m_slider = slide;
    // To get the decimals the slidervalue is multipied and
    // therefore must be devided again. The factor is from the gradientwidget.
    m_slider = m_slider / 1000;
    propertyChanged();
}


double KalziumElementProperty::getValue(int el) const
{
    if (m_currentGradient > 0) {
        return gradient()->value(el);
    }
    return 0;
}


QBrush KalziumElementProperty::getElementBrush(int el)
{
    // Hinding filtered elements
    if (!KalziumDataObject::instance()->search()->matches( el ) && KalziumDataObject::instance()->search()->isActive()) {
        return QBrush(Qt::darkGray, Qt::Dense7Pattern);
    }

    QBrush elementBrush(scheme()->elementBrush(el));

    // handel Gradient cases
    gradientBrushLogic(&elementBrush, el);

    // add an nice gradient
    QColor color = elementBrush.color();
    // Pixmaps get the color black. And no stylepattern is set
    if (color != QColor(Qt::black) && elementBrush.style() == Qt::SolidPattern) {
        QLinearGradient grad(QPointF(0, 0), QPointF(0, 40));
        grad.setColorAt(0,color);
        qreal h, s, v, a;
        color.getHsvF(&h, &s, &v, &a);
        color.setHsvF(h, s, v*0.7, a);
        grad.setColorAt(1,color);

        elementBrush = QBrush(grad);
    }

    return elementBrush;
}


QColor KalziumElementProperty::getTextColor(int el) const
{
    return scheme()->textColor(el);
}

QColor KalziumElementProperty::getBorderColor(int el) const
{
    // Show sheme color as border when gradients are selected.
    if (m_currentGradient > 0 ) {
        return scheme()->elementBrush(el).color();
    }

    // Transparent Borders don't make sens.
    if (getTextColor(el) == QColor(Qt::transparent)) {
        return QColor(Qt::black);
    }

    return getTextColor(el);

}

int KalziumElementProperty::getMode() const
{
    return m_mode;
}


void KalziumElementProperty::gradientBrushLogic(QBrush* elementBrush, int el)
{
    if (m_currentGradient == NOGRADIENT) return;

    const double gradientValue = gradient()->value( el );

    // Proof of concept SOM-Widget replacment. TODO make a class or somthing cool
    if (m_currentGradient == SOMGradientType) {
        const double melting = KalziumDataObject::instance()->element( el )->dataAsVariant( ChemicalDataObject::meltingpoint ).toDouble();
        const double boiling = KalziumDataObject::instance()->element( el )->dataAsVariant( ChemicalDataObject::boilingpoint ).toDouble();

        if ( m_slider < melting ) {
            //the element is solid
            elementBrush->setColor( Prefs::color_solid() );
        }
        else if ( ( m_slider > melting ) && ( m_slider < boiling ) ) {
            //the element is liquid
            elementBrush->setColor( Prefs::color_liquid() );
        }
        else if ( ( m_slider >= boiling ) && ( boiling > 0.0 ) ) {
            //the element is vaporous
            elementBrush->setColor( Prefs::color_vapor() );
        } else {
            elementBrush->setColor( Qt::lightGray );
        }
        return;
    }

    bool isActiv = true;
    switch( m_currentGradient ) {

      case DISCOVERYDATE:
        if ( gradientValue >= m_slider ) {
            isActiv = false;
	}
	break;

      default: // All other gradients
        if ( gradientValue < m_slider ) {
            isActiv = false;
	}
	break;
    }

    if ( !isActiv && gradientValue != -1) { //FIXME No magic number...
        elementBrush->setColor(Qt::transparent);
//         elementBrush->setStyle(Qt::Dense3Pattern);
    } else {
        const double coeff = gradient()->elementCoeff(el);
        elementBrush->setColor( gradient()->calculateColor(coeff) );
    }
}


#include "kalziumelementproperty.moc"
