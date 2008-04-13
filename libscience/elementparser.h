#ifndef ELEMENTPARSER_H
#define ELEMENTPARSER_H
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

#include <QtCore/QList>

#include "libkdeedu_science_export.h"

#include <QtXml/QtXml>

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
		bool startElement( const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs );

		bool endElement( const QString& namespaceURI, const QString &localName, const QString& qName );
		
		bool characters(const QString &ch);

		QList<Element*> getElements();

	private:
		class Private;
		Private *const d;
};
#endif // ELEMENTPARSER_H
