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

#ifndef SPECTRUMPARSER_H
#define SPECTRUMPARSER_H

#include "libkdeedu_science_export.h"

#include <QFile>
#include <QXmlDefaultHandler>
#include <QXmlAttributes>

#include "spectrum.h"

#include "chemicaldataobject.h"

class Spectrum;

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class SCIENCE_EXPORT SpectrumParser : public QXmlDefaultHandler
{
public:
    /**
     * Constructor
     */
    SpectrumParser();
    ~SpectrumParser();
    bool startElement( const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs );

    bool endElement( const QString& namespaceURI, const QString &localName, const QString& qName );

    bool characters(const QString &ch);

    QList<Spectrum*> getSpectrums();

private:
    QString currentElementID;

private:
    class Private;
    Private *const d;
};

#endif // SPECTRUMPARSER_H
