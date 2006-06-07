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
	
	CML::Molecule * mol = handler->getMolecule();
		kDebug() << mol->debug() << endl;
	
	return 0;
}
