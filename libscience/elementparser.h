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

#include <QList>

#include <qxml.h>

class Element;

class ElementSaxParser : public QXmlDefaultHandler
{
	public:
		ElementSaxParser();
		bool startElement(const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs);

		bool endElement (  const QString & namespaceURI, const QString & localName, const QString & qName );
		
		bool characters(const QString &ch);

		QList<Element*> getElements();

	private:
		Element *currentElement_;
		QList<Element*> elements_;
		bool inElement_;
		bool inName_,
			 inMass_,
			 inExactMass_,
			 inAtomicNumber_,
			 inSymbol_;
};
#endif // ELEMENTPARSER_H
