#include "../spectrumparser.h"
#include "../spectrum.h"
#include <kdebug.h>
#include <iostream>

int main(int argc, char *argv[])
{
	SpectrumParser * parser = new SpectrumParser();
	QFile xmlFile("cmlspec_example.cml");
	QXmlInputSource source(xmlFile);
	QXmlSimpleReader reader;

	reader.setContentHandler(parser);
	reader.parse(source);

//X 	QList<Isotope*> v = parser->getIsotopes();
//X 
//X 	foreach( Isotope* e, v ){
//X 		if ( e )
//X 		{
//X 			ChemicalDataObject* o = e->data();
//X 			kdDebug() << "Name: " << o->dictRef() << " " << o->valueAsString()  << " errorMargin: " << e->errorMargin() << " parent Element: " << e->parentElementSymbol() << endl;
//X 		}
//X 
//X 	}

	return 0;
}
