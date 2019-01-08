/***************************************************************************

copyright            : (C) 2004, 2005, 2006, 2007 by Carsten Niehaus
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

#include "detailedgraphicaloverview.h"

#include "kalziumdataobject.h"
#include "kalziumutils.h"

#include <KLocalizedString>

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFontDatabase>
#include <QPainter>
#include <QRect>
#include <QStandardPaths>
#include <QSvgRenderer>

#include <element.h>
#include "prefs.h"

DetailedGraphicalOverview::DetailedGraphicalOverview(QWidget *parent)
        : QWidget(parent), m_element(nullptr)
{
    setAttribute(Qt::WA_NoBackground, true);

    setMinimumSize(300, 200);

    // Set Hydrogen as initial element.
    setElement(1);
}

void DetailedGraphicalOverview::setElement(int el)
{
    m_element = KalziumDataObject::instance()->element(el);
    setBackgroundColor(KalziumElementProperty::instance()->getElementColor(el));
    update();
}

void DetailedGraphicalOverview::setBackgroundColor(QColor bgColor)
{
    if (bgColor == Qt::transparent) {
       bgColor = palette().background().color();
    }

    // add a gradient
    QLinearGradient grad(QPointF(0, 0), QPointF(0, height()));
    grad.setColorAt(0,bgColor);
    qreal h, s, v, a;
    bgColor.getHsvF(&h, &s, &v, &a);
    bgColor.setHsvF(h, s, v*0.6, a);
    grad.setColorAt(1,bgColor);

    m_backgroundBrush = QBrush(grad);
}

void DetailedGraphicalOverview::paintEvent(QPaintEvent*)
{
    QRect rect(0, 0, width(), height());

    QPixmap pm(width(), height());

    QPainter p;
    p.begin(&pm);

    p.setBrush(Qt::SolidPattern);

    if (!m_element)
    {
        pm.fill(palette().background().color());
        p.drawText(0, 0, width(), height(), Qt::AlignCenter | Qt::TextWordWrap, i18n("No element selected"));
    } else if (Prefs::colorschemebox() == 2) { //The iconic view is the 3rd view (0,1,2,...)
        pm.fill(palette().background().color());

        QString pathname = QStandardPaths::locate(QStandardPaths::DataLocation, QStringLiteral("data/iconsets/"), QStandardPaths::LocateDirectory);

        int enumii = m_element->dataAsVariant(ChemicalDataObject::atomicNumber).toInt();

        QString filename = pathname + "school" + '/' + QString::number(enumii)  + ".svg";

        QSvgRenderer svgrenderer;
        if (QFile::exists(filename) && svgrenderer.load(filename)) {
            QSize size = svgrenderer.defaultSize();
            size.scale(width(), height(), Qt::KeepAspectRatio);

            QRect bounds(QPoint(0, 0), size);
            bounds.moveCenter(QPoint(width()/2, height()/2));
            svgrenderer.render(&p, bounds);
        } else {
            p.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, i18n("No graphic found"));
        }
    } else {
        const int h_t = 20; //height of the texts

        p.setBrush(m_backgroundBrush);
        p.drawRect(rect);
        p.setBrush(Qt::black);
        p.setBrush(Qt::NoBrush);

        QFont fA = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
        QFont fB = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
        QFont fC = QFontDatabase::systemFont(QFontDatabase::GeneralFont);

        fA.setPointSize(fA.pointSize() + 20); //Huge font
        fA.setBold(true);
        fB.setPointSize(fB.pointSize() + 6); //Big font
        fC.setPointSize(fC.pointSize() + 4); //Big font
        fC.setBold(true);
        QFontMetrics fmA = QFontMetrics(fA);
        QFontMetrics fmB = QFontMetrics(fB);

        //coordinates for element symbol: near the center
        int xA = 4 * width() / 10;
        int yA = height() / 2;

        //coordinates for the atomic number: offset from element symbol to the upper left
        int xB = xA - fmB.width(m_element->dataAsString(ChemicalDataObject::atomicNumber));
        int yB = yA + fmB.height()/2;

        //Element Symbol
        p.setFont(fA);
        p.drawText(xA, yA, m_element->dataAsString(ChemicalDataObject::symbol));

        //Atomic number
        p.setFont(fB);
        p.drawText(xB, yB, m_element->dataAsString(ChemicalDataObject::atomicNumber));

        //Name and other data
        fC.setPointSize(h_t);
        p.setFont(fC);

        //Name
        p.drawText(1, 0, width(), height(), Qt::AlignLeft, m_element->dataAsString(ChemicalDataObject::name));

        //TODO Oxidationstates -> not there yet

        //Mass
        QString massString = i18nc("For example '1.0079u', the mass of an element in units", "%1 u", m_element->dataAsString(ChemicalDataObject::mass));
        int size3 = KalziumUtils::maxSize(massString, rect, fC, &p);
        fC.setPointSize(size3);
        p.setFont(fC);
        int offset = KalziumUtils::StringHeight(massString, fC, &p);
        p.drawText(0,
                    height() - offset,
                    width(),
                    offset,
                    Qt::AlignRight,
                    massString
                  );
    }

    p.end();

    p.begin(this);
    p.drawPixmap(0, 0, pm);
    p.end();
}

