/***************************************************************************
    copyright            : (C) 2005, 2006, 2007 by Carsten Niehaus
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

#include "kalziumutils.h"

#include <config-kalzium.h>

#include <QDebug>
#include <QFont>
#include <QPainter>
#include <QRect>

#include <KLocalizedString>
#include <KUnitConversion/Converter>

#include <element.h>

#include "prefs.h"

#include <math.h>
#if defined(HAVE_IEEEFP_H)
#include <ieeefp.h>
#endif

int KalziumUtils::maxSize(const QString& string, const QRect& rect, QFont font, QPainter* p, int minFontSize, int maxFontSize)
{
    bool goodSizeFound = false;
    int size = maxFontSize;
    QRect r;

    do {
        font.setPointSize(size);
        p->setFont(font);
        r = p->boundingRect(QRect(), Qt::AlignTop | Qt::AlignLeft, string);
        r.translate(rect.left(), rect.top());

        if (rect.contains(r)) {
            goodSizeFound = true;
        } else {
            --size;
        }
    }
    while (!goodSizeFound && (size > minFontSize));

    return size;
}

int KalziumUtils::StringHeight(const QString& string, const QFont& font, QPainter* p)
{
    Q_UNUSED(font);
    return p->boundingRect(QRect(), Qt::AlignTop | Qt::AlignLeft, string).height();
}

int KalziumUtils::StringWidth(const QString& string, const QFont& font, QPainter* p)
{
    Q_UNUSED(font);
    return p->boundingRect(QRect(), Qt::AlignTop | Qt::AlignLeft, string).width();
}

double KalziumUtils::strippedValue(double num)
{
    if (!finite(num)) {
        return num;
    }

    double power;
    power = 1e-6;
    while (power < num) {
        power *= 10;
    }

    num = num / power * 10000;
    num = qRound(num);

    return num * power / 10000;
}

QString KalziumUtils::prettyUnit(const Element* el, ChemicalDataObject::BlueObelisk kind)
{
    if (!el) {
        return i18n("Error");
    }

    QString result;
    double val = 0.0; //the value to convert

    switch (kind) {
    case ChemicalDataObject::meltingpoint: // a temperature
    case ChemicalDataObject::boilingpoint:
        result = el->dataAsStringWithUnit(kind, Prefs::temperatureUnit());
        break;

    case ChemicalDataObject::electronegativityPauling: // electronegativity
    {
        val = el->dataAsVariant(kind).toDouble();
        if (val <= 0.0) {
            result = i18n("Value not defined");
        } else {
            result = i18nc("Just a number", "%1", val);
        }
        break;
    }
    case ChemicalDataObject::electronAffinity: // an energy
    case ChemicalDataObject::ionization:
        result = el->dataAsStringWithUnit(kind, Prefs::energiesUnit());
        break;

    case ChemicalDataObject::mass: // a mass
    {
        val = el->dataAsVariant(kind).toDouble();
        if (val <= 0.0) {
            result = i18n("Unknown Value");
        } else {
            result = i18nc("x u (units). The atomic mass.", "%1 u", val);
        }
        break;
    }
    case ChemicalDataObject::date: // a date
    {
        //val = el->dataAsVariant(kind).toInt();
        int v_int = el->dataAsVariant(kind).toInt();
        if (val > 1600) {
            result = i18n("This element was discovered in the year <numid>%1</numid>.", val);
        } else if (v_int == -1) {
            result = i18n("The element has not yet been officially recognized by the IUPAC.");
        } else { // this should now really be 0. If not there is a bug in the database
            result = i18n("This element was known to ancient cultures.");
        }
        break;
    }
    case ChemicalDataObject::radiusCovalent:
    case ChemicalDataObject::radiusVDW:
    {
        result = el->dataAsStringWithUnit(kind, Prefs::lengthUnit());
        break;
    }
    case ChemicalDataObject::electronicConfiguration:
    {
        QString newOrbit = el->dataAsString(kind);

        QRegExp reg("(.*)([spdf])(\\d+)(.*)");

        while (newOrbit.contains(reg)) {
            newOrbit = newOrbit.replace(reg, "\\1\\2<sup>\\3</sup>\\4");
        }
        result = newOrbit;
        break;
    }
    case ChemicalDataObject::oxidation:
    {
        QStringList oxidationList = el->dataAsString(kind).split(',');
        result = oxidationList.join(QStringLiteral(", "));
        break;
    }
    default:
        result = el->dataAsVariant(kind).toString();
    }

    if (result.isEmpty()) {
        result = i18n("No Data");
    }
    return result;
}


void KalziumUtils::populateUnitCombobox(QComboBox *comboBox, const QList<int> &unitList)
{
    comboBox->clear();

    QString unitString;

    foreach (int unit, unitList) {
        unitString = KUnitConversion::Converter().unit(KUnitConversion::UnitId(unit)).description();
        unitString.append(" (");
        unitString.append(KUnitConversion::Converter().unit(KUnitConversion::UnitId(unit)).symbol());
        unitString.append(")");
        comboBox->addItem(unitString, unit);
    }
}
