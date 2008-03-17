/***************************************************************************

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


#ifndef PARLEYENGINE_H
#define PARLEYENGINE_H

#include <KRandomSequence>

#include "plasma/dataengine.h"

class KEduVocDocument;

class ParleyEngine : public Plasma::DataEngine
{
Q_OBJECT

public:
    ParleyEngine(QObject* parent, const QVariantList& args);
    ~ParleyEngine();
    QStringList sources() const;

protected:
    bool sourceRequested(const QString &name);
    bool updateSource(const QString& source);

private:
    void openDocument(const QString& file);
    KEduVocDocument* m_doc;
    KRandomSequence *m_random;
    int m_current;
};

K_EXPORT_PLASMA_DATAENGINE(parley, ParleyEngine)

#endif
