/* Sample parsing with QT's SAX2 by Riku Leino <tsoots@gmail.com> */

#include "elementparser.h"
#include "element.h"
#include <kdebug.h>
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
            foreach( ChemicalDataObject o, list ){
                    QString unit = o.unitAsString();
                    if ( unit == "bo:noUnit" )
                        unit = "";
                    kDebug() << "Name: " << o.dictRef() << " " << o.valueAsString()  <<" "  << unit << endl;
            }
        }
    }

    delete parser;
    qDeleteAll(v);

    return 0;
}
