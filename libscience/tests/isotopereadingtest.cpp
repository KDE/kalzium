/* Sample parsing with QT's SAX2 by Riku Leino <tsoots@gmail.com> */

#include "../isotopeparser.h"
#include "../isotope.h"
#include <kdebug.h>
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 2) {
		std::cout << "Usage: elements <XML_FILE>\n";
		return 1;
	}

	IsotopeParser * parser = new IsotopeParser();
	QFile xmlFile(argv[1]);
	QXmlInputSource source(xmlFile);
	QXmlSimpleReader reader;

	reader.setContentHandler(parser);
	reader.parse(source);

	QList<Isotope*> v = parser->getIsotopes();

	foreach( Isotope* e, v ){
		if ( e )
		{
			ChemicalDataObject* o = e->data();
			kdDebug() << "Name: " << o->dictRef() << " " << o->valueAsString()  << " errorMargin: " << e->errorMargin() << " parent Element: " << e->parentElementSymbol() << endl;
		}

	}

	return 0;
}
