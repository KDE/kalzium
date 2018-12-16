/***************************************************************************
    copyright            : (C) 2005 by Carsten Niehaus
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

#ifndef ISOTOPEPARSER_H
#define ISOTOPEPARSER_H

#include <QList>
#include <QXmlAttributes>
#include <QXmlDefaultHandler>

#include "libkdeedu_science_export.h"


class Isotope;

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class SCIENCE_EXPORT IsotopeParser : public QXmlDefaultHandler
{
public:
    /**
     * Constructor
     */
    IsotopeParser();
    ~IsotopeParser();
    bool startElement(const QString &, const QString &localName, const QString &,
                      const QXmlAttributes &attrs);

    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);

    bool characters(const QString &ch);

    QList<Isotope*> getIsotopes();

private:
    class Private;
    Private *const d;
};
#endif // ISOTOPEPARSER_H

