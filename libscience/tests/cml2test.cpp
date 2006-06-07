#include <kdebug.h>
#include <iostream>

#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <qxml.h>

#include "../xml_cml.h"
#include "../cmlclasses.h"

int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 2) {
		std::cout << "Usage: elements <XML_FILE>\n";
		return 1;
	}

	CMLParser *handler = new CMLParser();
	
 	QFile xmlFile(argv[1]);
	
	if ( !xmlFile.open( IO_ReadOnly ) ) 
		return false;
	
	QXmlInputSource source( xmlFile );
	QXmlSimpleReader xmlReader;
	xmlReader.setContentHandler( handler );
	xmlReader.parse( source );
	
	QList<CML::Atom*> parsedAtoms;
	parsedAtoms = handler->getAtoms();
	QList<CML::Bond*> parsedBonds;
	parsedBonds = handler->getBonds();
	
	kDebug() << "Found " << parsedAtoms.count() << " Atoms and " << parsedBonds.count() << " Bonds!" << endl;

	foreach( CML::Atom* a, parsedAtoms )
		kDebug() << a->debug() << endl;
	foreach( CML::Bond* b, parsedBonds )
		kDebug() << b->debug() << endl;
	
	return 0;
}
