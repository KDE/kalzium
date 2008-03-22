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
}

KalziumEngine::~KalziumEngine()
{
}

QStringList KalziumEngine::sources() const
{
    QStringList list;
    list << QLatin1String("BlueObelisk");
    return list;
}

bool KalziumEngine::sourceRequested(const QString &source)
{
    qDebug() << "source ist in sourceRequested(): " << source ;

    if (source == "randomElement"){
        getRandomElement();
        return true;
    }
    if (source.startsWith( "element:") ) {
        qDebug() << source << " means to return Element number " << "234";
        setElementNumber( 34 );
        return true;
    }
    return false;
}

bool KalziumEngine::updateSource(const QString &source)
{
    qDebug() << "in KalziumEngine::updateSource() with source = " << source;

    if (!m_currentElement) {
        setData(source, i18n("No element set."));
        return false;
    }

    qDebug() << "source ist in updateSource(): " << source ;

    setData("BlueObelisk", "bp", m_currentElement->dataAsString( ChemicalDataObject::boilingpoint )       );
    setData("BlueObelisk", "mp", m_currentElement->dataAsString( ChemicalDataObject::meltingpoint )       );
    setData("BlueObelisk", "name", m_currentElement->dataAsString( ChemicalDataObject::name )       );
    setData("BlueObelisk", "mass", m_currentElement->dataAsString( ChemicalDataObject::mass )       );
    setData("BlueObelisk", "symbol", m_currentElement->dataAsString( ChemicalDataObject::symbol )       );
    
    return true;
}

void KalziumEngine::setElementNumber( int number )
{
    qDebug() << "setting data to element number " << number;
    //Element N is N-1 in the list
    m_currentElement = m_elements.at(number + 1 );
}

void KalziumEngine::getRandomElement()
{
    qDebug() << "setting a random element";
    m_currentElement = m_elements.at(5);
    //m_currentElement = m_elements.at(m_random->getLong(m_elements.count()));
}

#include "kalzium_engine.moc"
