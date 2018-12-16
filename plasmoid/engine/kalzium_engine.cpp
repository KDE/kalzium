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
#include <psetables.h>

#include <QFile>
#include <QLatin1String>
#include <QLocalizedString>
#include <QStandardPaths>
#include <QStringList>

#include <KConfigGroup>

#include "plasma/datacontainer.h"


KalziumEngine::KalziumEngine(QObject* parent, const QVariantList& args)
        : Plasma::DataEngine(parent)
{
    Q_UNUSED(args)
    // we don't need the data to fly by any faster - even at a second it's hard to read.
    setMinimumPollingInterval(1000);

    // init random sequence
    m_random = new KRandomSequence(QDateTime::currentDateTime().toTime_t());

    // reading elements
    ElementSaxParser * parser = new ElementSaxParser();

    QFile xmlFile(QStandardPaths::locate(QStandardPaths::GenericDataLocation, "libkdeedu/data/elements.xml"));
    QXmlInputSource source(&xmlFile);
    QXmlSimpleReader reader;

    reader.setContentHandler(parser);
    reader.parse(source);

    m_elements = parser->getElements();
    delete parser;

    //Initialising the MoleculeParser
    m_moleculeParser = new MoleculeParser(m_elements);

    // Initialising the Table typ.
    m_currentTableTyp = "Classic";
}

KalziumEngine::~KalziumEngine()
{
    delete m_random;
    delete m_moleculeParser;
    qDeleteAll(m_elements);
}

QStringList KalziumEngine::sources() const
{
    // BlueObelisk:Element:# is created dynamically, so we don't advertise it here.
    QStringList list;
    list << QLatin1String("BlueObelisk:RandomElement");
    list << QLatin1String("BlueObelisk:Element:<n>:");
    list << QLatin1String("Fact");
    return list;
}

bool KalziumEngine::sourceRequestEvent(const QString &source)
{
    currentSource = &source;

    // return a randomly chosen element
    if (currentSource->startsWith(QLatin1String("BlueObelisk:"))) {
        setElementData();
        return true;
    }

    if (currentSource->startsWith(QLatin1String("Molecule:Parser:"))) {
        setMoleculeData();
        return true;
    }

    if (currentSource->startsWith(QLatin1String("Table:"))) {
        setPeriodicTableData();
    }

    return false;
}



bool KalziumEngine::setElementData()
{
//     qDebug() << "updateSourceElement";
    Element *currentElement;

    if (currentSource->operator==("BlueObelisk:RandomElement")) {
        // decide for a randomly chosen element
        currentElement = getRandomElement();
    } else if (currentSource->operator==("Fact")) {
        setData(*currentSource, "fact", generateFact());
    } else {
        currentElement = getElement(getKeyWord().toInt());
    }

    if (!currentElement) {
        return false;
    }

    // fill the engine with data
    setData(*currentSource, "number", currentElement->dataAsString(ChemicalDataObject::atomicNumber));
    setData(*currentSource, "bp", currentElement->dataAsString(ChemicalDataObject::boilingpoint));
    setData(*currentSource, "mp", currentElement->dataAsString(ChemicalDataObject::meltingpoint));
    setData(*currentSource, "name", currentElement->dataAsString(ChemicalDataObject::name));
    setData(*currentSource, "mass", currentElement->dataAsString(ChemicalDataObject::mass));
    setData(*currentSource, "symbol", currentElement->dataAsString(ChemicalDataObject::symbol));
    setData(*currentSource, "electronconf", currentElement->dataAsString(ChemicalDataObject::electronicConfiguration));
    setData(*currentSource, "period", currentElement->dataAsString(ChemicalDataObject::period));
    setData(*currentSource, "group", currentElement->dataAsString(ChemicalDataObject::group));
    setData(*currentSource, "en", currentElement->dataAsString(ChemicalDataObject::electronegativityPauling));
    setData(*currentSource, "family", currentElement->dataAsString(ChemicalDataObject::family));

    // position Information
    int elementNumber = currentElement->dataAsVariant(ChemicalDataObject::atomicNumber).toInt();
    pseTable *currentTable = pseTables::instance()->getTabletype(m_currentTableTyp);
    setData(*currentSource, "x", currentTable->elementCoords(elementNumber).x());
    setData(*currentSource, "y", currentTable->elementCoords(elementNumber).y());
    setData(*currentSource, "previousof", currentTable->previousOf(elementNumber));
    setData(*currentSource, "nextof", currentTable->nextOf(elementNumber));

    // TODO add more/all data.

    return true;
}

QString KalziumEngine::getKeyWord(int id)
{
    QStringList idList;
    idList = currentSource->split(':');

    if (id == -1) {
        return idList.last();
    }

    if (idList.length() < id) {
        return QString();
    }

    return idList.at(id);
}


Element* KalziumEngine::getElement(int number)
{
    if (number > 0 && number <= m_elements.count()) {
        //Element N is N-1 in the list
        return m_elements.at(number - 1);
    }

    return 0;
}

Element* KalziumEngine::getRandomElement()
{
    qDebug() << "setting a random element";
    return m_elements.at(m_random->getLong(m_elements.count()));
}

QString KalziumEngine::generateFact()
{
    int rand = m_random->getLong(3);
    int eNumber = m_random->getLong(95);

    Element *currentElement;

    currentElement = getElement(eNumber);

    if (!currentElement) {
        return i18n("An error occurred. No element found.");
    }

    QString bp =        currentElement->dataAsString(ChemicalDataObject::boilingpoint);
    QString mp =        currentElement->dataAsString(ChemicalDataObject::meltingpoint)       ;
    QString name =      currentElement->dataAsString(ChemicalDataObject::name)      ;
    QString mass =      currentElement->dataAsString(ChemicalDataObject::mass)      ;
    QString symbol =    currentElement->dataAsString(ChemicalDataObject::symbol)       ;

    switch (rand) {
    case 0:
        return i18n("Did you know that\n the element %1 has the symbol %2?", name, symbol);
    case 1:
        return i18n("Did you know that\n %1 (%2) weighs %3 u?", name, symbol, mass);
    case 2:
        return i18n("Did you know that\n %1 (%2) weighs %3 u?", name, symbol, mass);
    default:
        return i18n("Did you know that\n the element %1 has the symbol %2?", name, symbol);
    }

    return i18n("An error occurred.");
}


bool KalziumEngine::setPeriodicTableData()
{
    // "Table:typ"

    if (getKeyWord() == "list") {
        setData(*currentSource, "tablelist", pseTables::instance()->tables());
        return true;
    }

    QString tableType;
    tableType = getKeyWord();

    if (tableType.isEmpty()) {
        return false;
    }

    pseTable *currentTable = pseTables::instance()->getTabletype(tableType);

    if (currentTable == 0) {
        return false;
    }

    m_currentTableTyp = tableType;

    setData(*currentSource, "description", currentTable->description());
    setData(*currentSource, "tablesize", currentTable->tableSize());
    setData(*currentSource, "firstelement", currentTable->firstElement());
    setData(*currentSource, "lastelement", currentTable->lastElement());
//     setData(*currentSource, "elementlist", currentTable->elements());

    return true;
}


bool KalziumEngine::setMoleculeData()
{
    QString          molecule;
    double           mass;
    ElementCountMap  elementMap;

    // Molecule:Parser:CH4
    molecule = getKeyWord();

    if (!m_moleculeParser->weight(molecule, &mass, &elementMap)) {
        return false;
    }

    setData(*currentSource, "molMass", mass);
    setData(*currentSource, "niceMolecule", sumUpMolecue(elementMap));
    return true;
}

QString KalziumEngine::sumUpMolecue(ElementCountMap &elementMap)
{
    QString niceMolecule;
    foreach (ElementCount * Ecount , elementMap.map()) {
        niceMolecule.append(Ecount->element()->dataAsString(ChemicalDataObject::symbol));
        if (Ecount->count() > 1) {
            niceMolecule.append(QString::number(Ecount->count()));
        }
    }
    return niceMolecule;
}


#include "kalzium_engine.moc"

