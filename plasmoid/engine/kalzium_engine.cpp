/***************************************************************************
    copyright            : (C) 2008, 2009 by Carsten Niehaus
    email                : cniehaus@kde.org
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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
#include <QLatin1String>
#include <QStringList>

#include <KLocale>
#include <KUrl>
#include <KStandardDirs>
#include <KConfigGroup>

#include "plasma/datacontainer.h"

KalziumEngine::KalziumEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent)
{
    KGlobal::locale()->insertCatalog( "libkdeedu" );
    Q_UNUSED(args)
    // we don't need the data to fly by any faster - even at a second it's hard to read.
    setMinimumPollingInterval(1000);

    // init random sequence
    m_random = new KRandomSequence( QDateTime::currentDateTime().toTime_t() );

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
    delete m_random;
}

QStringList KalziumEngine::sources() const
{
    // BlueObelisk:Element:# is created dynamically, so we don't advertise it here.
    QStringList list;
    list << QLatin1String("BlueObelisk:RandomElement");
    list << QLatin1String("Fact");
    return list;
}

bool KalziumEngine::sourceRequestEvent(const QString &source)
{
    // return a randomly chosen element
    if (source == "BlueObelisk:RandomElement"){
        // create the data
        updateSource(source);
        return true;
    }

    // return element #
    if (source.startsWith( QLatin1String("BlueObelisk:Element:") ) ) {
        // create the data
        updateSource(source);
        return true;
    }
    
    if (source == "Fact" ) {
        // create the data
        updateSource(source);
        return true;
    }
    
    return false;
}

bool KalziumEngine::updateSource(const QString &source)
{
    qDebug() << "updateSource";
    if (source == "BlueObelisk:RandomElement") {
        // decide for a randomly chosen element
        getRandomElement();
    } else if ( source == "Fact" ) { 
        qDebug() << "Fact is the current source";
        setData(source, "fact", generateFact() );
    } else { // parse the string to know which element to display
        setElementNumber( source.right(source.length()-source.lastIndexOf(':') - 1 ).toInt() );
    }

    if (!m_currentElement) {
        return false;
    }

    // fill the engine with data
    setData(source, "bp", m_currentElement->dataAsString( ChemicalDataObject::boilingpoint )       );
    setData(source, "mp", m_currentElement->dataAsString( ChemicalDataObject::meltingpoint )       );
    setData(source, "name", m_currentElement->dataAsString( ChemicalDataObject::name )       );
    setData(source, "mass", m_currentElement->dataAsString( ChemicalDataObject::mass )       );
    setData(source, "symbol", m_currentElement->dataAsString( ChemicalDataObject::symbol )       );

    return true;
}

void KalziumEngine::setElementNumber( int number )
{
    if (number > 0 && number <= m_elements.count()) {
        //Element N is N-1 in the list
        m_currentElement = m_elements.at(number-1);
    } else {
        m_currentElement = 0;
    }
}

void KalziumEngine::getRandomElement()
{
    qDebug() << "setting a random element";
    m_currentElement = m_elements.at(m_random->getLong(m_elements.count()));
}

QString KalziumEngine::generateFact()
{
    int rand = m_random->getLong(3);
    int eNumber = m_random->getLong(95);
        
    m_currentElement = m_elements.at(eNumber);

    if ( !m_currentElement ) 
	    return i18n( "An error occurred." );

    QString bp =        m_currentElement->dataAsString( ChemicalDataObject::boilingpoint );
    QString mp =        m_currentElement->dataAsString( ChemicalDataObject::meltingpoint )       ;
    QString name =      m_currentElement->dataAsString( ChemicalDataObject::name )      ;
    QString mass =      m_currentElement->dataAsString( ChemicalDataObject::mass )      ;
    QString symbol =    m_currentElement->dataAsString( ChemicalDataObject::symbol )       ;

    switch (rand) {
        case 0:
            return i18n( "Did you know that\n the element %1 has the symbol %2?", name, symbol );
        case 1:
            return i18n( "Did you know that\n %1 (%2) weighs %3 u?", name, symbol, mass );
        case 2:
            return i18n( "Did you know that\n %1 (%2) weighs %3 u?", name, symbol, mass );
        default:
            return i18n( "Did you know that\n the element %1 has the symbol %2?", name, symbol );
    }

    return i18n( "An error occurred." );
}

#include "kalzium_engine.moc"
