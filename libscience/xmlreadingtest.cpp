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
    
    if (xmlFile.exists())
        kDebug() << "file exists" << endl;
    else
        kDebug() << "file DOES NOT exist" << endl;

    QXmlInputSource source(&xmlFile);
    QXmlSimpleReader reader;

    reader.setContentHandler(parser);
    reader.parse(source);

    QList<Element*> v = parser->getElements();

//X     foreach( Element* e, v ){
//X         if ( e )
//X         {
//X             QList<ChemicalDataObject*> list = e->data();
//X 
//X             //Test: give me all data available
//X             foreach( ChemicalDataObject*o, list ){
//X                 if ( o )
//X                 {
//X                     QString unit = o->unitAsString();
//X                     if ( unit == "bo:noUnit" )
//X                         unit = "";
//X                     kDebug() << "Name: " << o->dictRef() << " " << o->valueAsString()  <<" "  << unit << endl;
//X                 }
//X             }
//X         }
//X 
//X     }

    return 0;
}
