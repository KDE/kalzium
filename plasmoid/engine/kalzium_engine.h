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
#ifndef KALZIUM_ENGINE_H
#define KALZIUM_ENGINE_H

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
    void setElementNumber( int number );

    /**
     * This methods makes a random element the current one.
     */
    void getRandomElement();

    /**
     * This methods generates a random fact about the chemical elements.
     */
    QString generateFact();
    
    bool sourceRequested(const QString &name);

protected:
    bool updateSource(const QString& source);

private:
    Element * m_currentElement;
    QList<Element*> m_elements;
    KRandomSequence * m_random;
};

K_EXPORT_PLASMA_DATAENGINE(kalzium, KalziumEngine)

#endif // KALZIUM_ENGINE_H
