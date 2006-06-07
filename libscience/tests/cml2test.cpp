#include <kdebug.h>
#include <iostream>

#include <qtextstream.h>
#include <qfile.h>
#include <qdatetime.h>
#include <qxml.h>

#include "../cml/xml_cml.h"
#include "../cml/cmlclasses.h"

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
	
	QTextStream t1( &xmlFile );
	
	QXmlInputSource source;
//	source.setData( t1 );
	
	QXmlSimpleReader xmlReader;
	xmlReader.setContentHandler( handler );
	
	xmlReader.parse( source );

	return 0;
}
