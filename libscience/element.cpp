/*
    SPDX-FileCopyrightText: 2003, 2004, 2005 Carsten Niehaus <cniehaus@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "element.h"

#include <QDebug>

#include <KUnitConversion/Converter>

Element::Element()
{
}

QVariant Element::dataAsVariant(ChemicalDataObject::BlueObelisk type) const
{
    foreach (const ChemicalDataObject &o, dataList) {
        if (o.type() == type) {
            return o.value();
        }
    }
    return QVariant();
}

QVariant Element::dataAsVariant(ChemicalDataObject::BlueObelisk type, int unit) const
{
    foreach (const ChemicalDataObject &o, dataList) {
        if (o.type() == type) {
            if (unit == KUnitConversion::NoUnit) {
                return o.value();
            }
            KUnitConversion::Value data(o.value().toDouble(), KUnitConversion::UnitId(o.unit()));
            return QVariant(data.convertTo(KUnitConversion::UnitId(unit)).number());
        }
    }
    return QVariant();
}

QString Element::dataAsString(ChemicalDataObject::BlueObelisk type) const
{
    return dataAsVariant(type).toString();
}

QString Element::dataAsString(ChemicalDataObject::BlueObelisk type, int unit) const
{
    return dataAsVariant(type, unit).toString();
}

QString Element::dataAsStringWithUnit(ChemicalDataObject::BlueObelisk type, int unit) const
{
    QString valueAndUnit(QString::number(dataAsVariant(type, unit).toDouble(), 'g', 4));

    if (valueAndUnit.isEmpty()) {
        return QString();
    }

    valueAndUnit.append(" ");
    valueAndUnit.append(KUnitConversion::Converter().unit(KUnitConversion::UnitId(unit)).symbol());
    return valueAndUnit;
}

Element::~Element()
{
}

void Element::addData(const ChemicalDataObject& o)
{
    dataList.append(o);
}

void Element::addData(const QVariant& value, ChemicalDataObject::BlueObelisk type)
{
    ChemicalDataObject tmp(value, type);
    dataList.append(tmp);
}
