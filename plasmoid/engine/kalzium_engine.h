/*
    SPDX-FileCopyrightText: 2008 Carsten Niehaus
    email                : cniehaus@kde.org
*/
/***************************************************************************
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 *                                                                         *
 ***************************************************************************/
#ifndef KALZIUM_ENGINE_H
#define KALZIUM_ENGINE_H

#include <moleculeparser.h>

#include "plasma/dataengine.h"

#include <KRandomSequence>

class Element;

class KalziumEngine : public Plasma::DataEngine
{
    Q_OBJECT

public:
    KalziumEngine(QObject* parent, const QVariantList& args);
    ~KalziumEngine();
    QStringList sources() const;

    /**
      * set the element to the element with the number @number. For example, setElementNumber( 1 )
      * will make Hydrogen the currently active element
      */
    Element* getElement( int number );

    /**
     * This methods makes a random element the current one.
     */
    Element* getRandomElement();

    /**
     * This methods generates a random fact about the chemical elements.
     */
    QString generateFact();

    bool sourceRequestEvent(const QString &name);

protected: // FIXME is protected needed? private?
    bool setElementData();
    bool setMoleculeData();
    bool setPeriodicTableData();

private:
    ///Summs up all Elements of a Molecule.
    QString sumUpMolecue(ElementCountMap &elementMap);

    /**
     * Get keyword from source string
     * given the source string: "source:my:keyword"
     * @parm id
     */
    QString getKeyWord( int id = -1);

    /// TODO:use int instead of a QString
    QString m_currentTableTyp;

    const QString *currentSource;

    Element * m_currentElement;
    QList<Element*> m_elements;
    KRandomSequence * m_random;

    MoleculeParser *m_moleculeParser;
};

K_EXPORT_PLASMA_DATAENGINE(kalzium, KalziumEngine)

#endif // KALZIUM_ENGINE_H
