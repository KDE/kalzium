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

#include <QString>
#include <QList>
#include <QStringList>
#include <qdom.h>

#include <qxml.h>

class Element;

/**
 * This class gives access to the elements which are listed in a CML-file
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class ElementParser
{
	public:
		/**
		 * @return the Element with the symbol symbol
		 * @param dataDocument the document to parse
		 * @param symbol the symbol of the Element which is looked for
		 */
		static Element* loadElement( const QString& symbol, const QDomDocument& dataDocument );
		
		/**
		 * @return the element represented in the QDomeElement @p element
		 * @param element the XML-representation of the Element
		 */
		static Element* loadElement( const QDomElement& element );

		/**
		 * @return all chemical elements in the xml-file
		 * @param dataDocument the document to parse
		 */
		static QList<Element*> loadAllElements(const QDomDocument& dataDocument);
		
	private:
		/**
		 * @return a QStringList with the symbols of all known elements
		 * @param dataDocument the document to parse
		 */
		static QStringList loadElementSymbols(const QDomDocument& dataDocument);

};


class ElementSaxParser : public QXmlDefaultHandler
{
	public:
		ElementSaxParser();
		bool startElement(const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs);
		bool endElement(const QString&, const QString &localName, const QString&, const QXmlAttributes&);
		bool characters(const QString &ch);
		QList<Element*> getElements();

	private:
		Element *currentElement_;
		QList<Element*> elements_;
		bool inElement_;
		bool inName_;

};
#endif // ELEMENTPARSER_H
