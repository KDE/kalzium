/***************************************************************************
    copyright            : (C) 2008 by Carsten Niehaus
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

#include "kalzium_engine.h"

#include <element.h>
#include <elementparser.h>

#include <QFile>
#include <QStringList>

#include <KLocale>
#include <KUrl>
#include <KStandardDirs>
#include <KConfigGroup>

#include "plasma/datacontainer.h"

KalziumEngine::KalziumEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent)
{
    Q_UNUSED(args)
    setMinimumUpdateInterval(1000);

    // reading elements
    ElementSaxParser * parser = new ElementSaxParser();

    QFile xmlFile( KStandardDirs::locate( "data", "libkdeedu/data/elements.xml" ) );
    QXmlInputSource source(&xmlFile);
    QXmlSimpleReader reader;

    reader.setContentHandler(parser);
    reader.parse(source);

    m_elements = parser->getElements();
    delete parser;
    
    m_random = new KRandomSequence(QDateTime::currentDateTime().toTime_t() );
    m_currentElement = m_elements.at(m_random->getLong(m_elements.count()));

    qDebug() << "KalziumEngine::KalziumEngine";
}

KalziumEngine::~KalziumEngine()
{
    delete m_random;
}

QStringList KalziumEngine::sources() const
{
    qDebug() << "sources";
    QStringList list;
    list << QLatin1String("BlueObelisk");
    return list;
}

bool KalziumEngine::sourceRequested(const QString &source)
{
    qDebug() << "sourceRequested";
    if (source == QLatin1String("BlueObelisk")) {
        setData("BlueObelisk", m_currentElement->dataAsString( ChemicalDataObject::symbol )       );
        return true;
    }
    return false;
}

bool KalziumEngine::updateSource(const QString &source)
{
    qDebug() << "updateSource()";
    m_currentElement = m_elements.at(m_random->getLong(m_elements.count()));
    if (!m_currentElement) {
        setData(source, i18n("No element set."));
        return false;
    }
    setData("BlueObelisk", m_currentElement->dataAsString( ChemicalDataObject::symbol )       );
    
    qDebug() << "KalziumEngine::updateSource()" << source;
    return true;
}

#include "kalzium_engine.moc"
