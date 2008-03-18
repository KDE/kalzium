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

#include <KRandomSequence>

#include "plasma/dataengine.h"

class Element;

class KalziumEngine : public Plasma::DataEngine
{
Q_OBJECT

public:
    KalziumEngine(QObject* parent, const QVariantList& args);
    ~KalziumEngine();
    QStringList sources() const;

protected:
    bool sourceRequested(const QString &name);
    bool updateSource(const QString& source);

private:
    Element * m_currentElement;
    QList<Element*> m_elements;
    KRandomSequence * m_random;
};

K_EXPORT_PLASMA_DATAENGINE(kalzium, KalziumEngine)

#endif // KALZIUM_ENGINE_H
