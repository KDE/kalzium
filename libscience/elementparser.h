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

#ifndef ELEMENTPARSER_H
#define ELEMENTPARSER_H

#include <QList>
#include <QXmlAttributes>
#include <QXmlDefaultHandler>

#include "libkdeedu_science_export.h"

class Element;

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class SCIENCE_EXPORT ElementSaxParser : public QXmlDefaultHandler
{
public:
    /**
     * Constructor
     */
    ElementSaxParser();
    ~ElementSaxParser();
    bool startElement(const QString&, const QString &localName, const QString&,
                      const QXmlAttributes &attrs);

    bool endElement(const QString& namespaceURI, const QString &localName, const QString& qName);

    bool characters(const QString &ch);

    QList<Element*> getElements();

private:
    /**
    * Looks up a name @p unitname. The valid names are
    * hard-coded in the C++ code, currently
    *   - bo:kelvin
    *   - bo:ev
    *   - bo:nm
    *   - bo:pm
    *   - bo:y
    *   - bo:s
    *   - bo:noUnit
    *
    * @return the BlueObeliskUnit of a ChemicalDataObject
    *   corresponding to @p unitname, or noUnit if the name
    *   doesn't match any of the known values.
    * @param unitname the attribute-text of the XML parsed
    */
    int unit(const QString& unitname) const;

    class Private;
    Private *const d;
};

#endif // ELEMENTPARSER_H
