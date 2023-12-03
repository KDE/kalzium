/*
    SPDX-FileCopyrightText: 2004, 2005, 2006, 2007 Carsten Niehaus <cniehaus@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "detailedgraphicaloverview.h"

#include "kalziumelementproperty.h"
#include "kalziumdataobject.h"
#include "kalziumutils.h"
#include "kalzium_debug.h"

#include <KLocalizedString>

#include <QFile>
#include <QFontDatabase>
#include <QPainter>
#include <QRect>
#include <QStandardPaths>
#include <QSvgRenderer>
#include <QGuiApplication>

#include "prefs.h"
#include "colorutils.h"
#include <element.h>

DetailedGraphicalOverview::DetailedGraphicalOverview(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_OpaquePaintEvent, true);

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
        bgColor = palette().window().color();
    }

    m_textColor = ColorUtils::contrastColor(bgColor);
    m_backgroundBrush = QBrush(ColorUtils::tintWithAlpha(qGuiApp->palette().color(QPalette::Normal, QPalette::Window), bgColor, 0.2));
}

void DetailedGraphicalOverview::paintEvent(QPaintEvent *)
{
    const int margins = 4;
    QPainter p;
    p.begin(this);

    p.setBrush(Qt::SolidPattern);

    if (!m_element) {
        p.setBrush(palette().window().color());
        p.drawRect(rect());
        p.drawText(0, 0, width(), height(), Qt::AlignCenter | Qt::TextWordWrap, i18n("No element selected"));
    } else if (Prefs::colorschemebox() == 2) { // The iconic view is the 3rd view (0,1,2,...)
        p.setBrush(palette().window().color());
        p.drawRect(rect());

        QString pathname = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, QStringLiteral("data/iconsets/"), QStandardPaths::LocateDirectory);

        int enumii = m_element->dataAsVariant(ChemicalDataObject::atomicNumber).toInt();

        QString filename = pathname + "school" + '/' + QString::number(enumii) + ".svg";

        QSvgRenderer svgrenderer;
        if (QFile::exists(filename) && svgrenderer.load(filename)) {
            QSize size = svgrenderer.defaultSize();
            size.scale(width(), height(), Qt::KeepAspectRatio);

            QRect bounds(QPoint(0, 0), size);
            bounds.moveCenter(QPoint(width() / 2, height() / 2));
            svgrenderer.render(&p, bounds);
        } else {
            p.drawText(rect(), Qt::AlignCenter | Qt::TextWordWrap, i18n("No graphic found"));
        }
    } else {
        const int h_t = 20; // height of the texts

        p.setPen(Qt::NoPen);
        p.setBrush(m_backgroundBrush);
        p.drawRect(rect());
        p.setBrush(Qt::black);
        p.setBrush(Qt::NoBrush);

        p.setPen(m_textColor);

        QFont fA = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
        QFont fB = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
        QFont fC = QFontDatabase::systemFont(QFontDatabase::GeneralFont);

        fA.setPointSize(fA.pointSize() + 20); // Huge font
        fA.setBold(true);
        fB.setPointSize(fB.pointSize() + 6); // Big font
        fC.setPointSize(fC.pointSize() + 4); // Big font
        fC.setBold(true);
        QFontMetrics fmA = QFontMetrics(fA);
        QFontMetrics fmB = QFontMetrics(fB);

        // coordinates for element symbol: near the center
        int xA = 4 * width() / 10;
        int yA = height() / 2;

        // coordinates for the atomic number: offset from element symbol to the upper left
        int xB = xA - fmB.boundingRect(m_element->dataAsString(ChemicalDataObject::atomicNumber)).width();
        int yB = yA + fmB.height() / 2;

        // Element Symbol
        p.setFont(fA);
        p.drawText(xA, yA, m_element->dataAsString(ChemicalDataObject::symbol));

        // Atomic number
        p.setFont(fB);
        p.drawText(xB, yB, m_element->dataAsString(ChemicalDataObject::atomicNumber));

        // Name and other data
        fC.setPointSize(h_t);
        p.setFont(fC);

        // Name
        p.drawText(margins, 0, width(), height(), Qt::AlignLeft, m_element->dataAsString(ChemicalDataObject::name));

        // TODO Oxidationstates -> not there yet

        // Mass
        QString massString = i18nc("For example '1.0079u', the mass of an element in units", "%1 u", m_element->dataAsString(ChemicalDataObject::mass));
        int size3 = KalziumUtils::maxSize(massString, rect(), fC, &p);
        fC.setPointSize(size3);
        p.setFont(fC);
        int offset = KalziumUtils::StringHeight(massString, fC, &p);
        p.drawText(-margins, height() - offset, width(), offset, Qt::AlignRight, massString);
    }

    p.end();
}

#include "moc_detailedgraphicaloverview.cpp"
