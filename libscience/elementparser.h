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

//X /**
//X  * This class gives access to the elements which are listed in a CML-file
//X  * @author Carsten Niehaus <cniehaus@kde.org>
//X  */
//X class ElementParser
//X {
//X 	public:
//X 		/**
//X 		 * @return the Element with the symbol symbol
//X 		 * @param dataDocument the document to parse
//X 		 * @param symbol the symbol of the Element which is looked for
//X 		 */
//X 		static Element* loadElement( const QString& symbol, const QDomDocument& dataDocument );
//X 		
//X 		/**
//X 		 * @return the element represented in the QDomeElement @p element
//X 		 * @param element the XML-representation of the Element
//X 		 */
//X 		static Element* loadElement( const QDomElement& element );
//X 
//X 		/**
//X 		 * @return all chemical elements in the xml-file
//X 		 * @param dataDocument the document to parse
//X 		 */
//X 		static QList<Element*> loadAllElements(const QDomDocument& dataDocument);
//X 		
//X 	private:
//X 		/**
//X 		 * @return a QStringList with the symbols of all known elements
//X 		 * @param dataDocument the document to parse
//X 		 */
//X 		static QStringList loadElementSymbols(const QDomDocument& dataDocument);
//X 
//X };


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
			 inSymbol_,
			 inAtomicNumber_;
};
#endif // ELEMENTPARSER_H
