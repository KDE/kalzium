/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007 by Carsten Niehaus                     *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "kalziumdataobject.h"

#include <elementparser.h>
#include <isotope.h>
#include <isotopeparser.h>
#include <spectrumparser.h>

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QGlobalStatic>
#include <QLocale>
#include <QPainter>
#include <QStandardPaths>
#include <QSvgRenderer>
#include <QUrl>

#include <KUnitConversion/Converter>

struct StaticKalziumDataObject
{
    KalziumDataObject kdo;
};

Q_GLOBAL_STATIC(StaticKalziumDataObject, s_kdo)

KalziumDataObject* KalziumDataObject::instance()
{
    return &s_kdo->kdo;
}

KalziumDataObject::KalziumDataObject()
        : m_search(nullptr)
{
    // reading elements
    ElementSaxParser * parser = new ElementSaxParser();

    QFile xmlFile(QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("libkdeedu/data/elements.xml")));
    QXmlInputSource source(&xmlFile);
    QXmlSimpleReader reader;

    reader.setContentHandler(parser);
    reader.parse(source);

    ElementList = parser->getElements();

    //we don't need parser anymore, let's free its memory
    delete parser;

    //read the spectra
    SpectrumParser * spectrumparser = new SpectrumParser();

    QFile xmlSpFile(QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("libkdeedu/data/spectra.xml")));
    QXmlInputSource spsource(&xmlSpFile);
    QXmlSimpleReader sp_reader;

    sp_reader.setContentHandler(spectrumparser);
    sp_reader.parse(spsource);

    m_spectra = spectrumparser->getSpectrums();

    //we don't need spectrumparser anymore, let's free its memory
    delete spectrumparser;

    // reading isotopes
    IsotopeParser * isoparser = new IsotopeParser();

    QFile xmlIsoFile(QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("libkdeedu/data/isotopes.xml")));
    QXmlInputSource isosource(&xmlIsoFile);
    QXmlSimpleReader isoreader;

    isoreader.setContentHandler(isoparser);
    isoreader.parse(isosource);

    QList<Isotope*> isotopes = isoparser->getIsotopes();

    //we don't need isoparser anymore, let's free its memory
    delete isoparser;

    foreach (Isotope *iso, isotopes)
    {
        int num = iso->parentElementNumber();
        if (m_isotopes.contains(num)) {
            m_isotopes[num].append(iso);
        } else {
            QList<Isotope*> newlist;
            newlist.append(iso);
            m_isotopes.insert(num, newlist);
        }
    }

    // cache it
    m_numOfElements = ElementList.count();

    qAddPostRoutine(KalziumDataObject::cleanup);
}

KalziumDataObject::~KalziumDataObject()
{
    //Delete all elements
    qDeleteAll(ElementList);

    //Delete all isotopes
    QHashIterator<int, QList<Isotope*> > i(m_isotopes);
    while (i.hasNext()) {
        i.next();
        qDeleteAll(i.value());
    }

    //Delete the spectra
    qDeleteAll(m_spectra);
}

Element* KalziumDataObject::element(int number)
{
    // checking that we are requesting a valid element
    if ((number <= 0) || (number > m_numOfElements))
        return nullptr;
    return ElementList[ number-1 ];
}

QString KalziumDataObject::unitAsString(const int unit) const
{
    return KUnitConversion::Converter().unit(KUnitConversion::UnitId(unit)).symbol();
}

QPixmap KalziumDataObject::pixmap(int number)
{
    // checking that we are requesting a valid element
    if ((number <= 0) || (number > m_numOfElements))
        return QPixmap();
    if (PixmapList.isEmpty())
        loadIconSet();
    return PixmapList[ number-1 ];
}

QList<Isotope*> KalziumDataObject::isotopes(Element * element)
{
    return isotopes(element->dataAsVariant(ChemicalDataObject::atomicNumber).toInt());
}

QList<Isotope*> KalziumDataObject::isotopes(int number)
{
    return m_isotopes.contains(number) ? m_isotopes.value(number) : QList<Isotope*>();
}

Spectrum * KalziumDataObject::spectrum(int number)
{
    foreach (Spectrum * s, m_spectra) {
        if (s->parentElementNumber() == number) {
            return s;
        }
    }

    return nullptr;
}


void KalziumDataObject::setSearch(Search *srch)
{
    m_search = srch;
}

Search* KalziumDataObject::search() const
{
    return m_search;
}

void KalziumDataObject::cleanup()
{
    KalziumDataObject::instance()->cleanPixmaps();
}

void KalziumDataObject::loadIconSet()
{
    //FIXME in case we ever get more than one theme we need
    //a settings-dialog where we can select the different iconsets...
    const QString setname = QStringLiteral("school");
    QString pathname = QStandardPaths::locate(
        QStandardPaths::DataLocation, "data/iconsets/" + setname + '/',
        QStandardPaths::LocateDirectory);
    QSvgRenderer renderer;

    for (int i = 0; i < m_numOfElements; ++i) {
        QString filename = pathname + QString::number(i + 1) + ".svg";

        renderer.load(filename);
        QPixmap pix(40, 40);
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        renderer.render(&p);

        Element *e = ElementList.at(i);
        QString esymbol = e->dataAsString(ChemicalDataObject::symbol);
        p.drawText(0,0,40,40, Qt::AlignCenter | Qt::TextWordWrap, esymbol);
        p.end();

        PixmapList << pix;
    }
}

void KalziumDataObject::cleanPixmaps()
{
    PixmapList.clear();
}
