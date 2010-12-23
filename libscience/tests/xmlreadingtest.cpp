/***************************************************************************
 *   Copyright (C) 2005, 2006 by Carsten Niehaus <cniehaus@kde.org>        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include "elementparser.h"
#include "element.h"
#include <QtCore/QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 2) {
        std::cout << "Usage: elements <XML_FILE>\n";
        return 1;
    }

    ElementSaxParser * parser = new ElementSaxParser();
    QFile xmlFile(argv[1]);

    QXmlInputSource source(&xmlFile);
    QXmlSimpleReader reader;

    reader.setContentHandler(parser);
    reader.parse(source);

    QList<Element*> v = parser->getElements();

    std::cout << "Found " << v.count() << " elements." << std::endl;

    foreach( Element* e, v ){
        if ( e )
        {
            QList<ChemicalDataObject> list = e->data();

            //Test: give me all data available
            foreach( const ChemicalDataObject &o, list ){
                    QString unit = o.unitAsString();
                    if ( unit == "bo:noUnit" )
                        unit = "";
                    qDebug() << "Name: " << o.dictRef() << " " << o.valueAsString()  <<" "  << unit;
            }
        }
    }

    delete parser;
    qDeleteAll(v);

    return 0;
}
