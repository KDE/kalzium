/***************************************************************************
 *   Copyright (C) 2003 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "element.h"
#include "prefs.h"
#include <qregexp.h>
#include <qmap.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kurl.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kfiledialog.h>
#include <kmessagebox.h>

#include <math.h>

#include <qpainter.h>


Element::Element()
{
}

QString Element::parsedOrbits( bool canBeEmpty )
{
	if ( m_orbits.isEmpty() )
		if ( !canBeEmpty )
			return i18n( "structure means orbital configuration in this case", "Unknown structure" );
		else
			return "";
	
	QString orbits = m_orbits;
	QRegExp rxs("([a-z])([0-9]+)");
	QRegExp rxb("([a-z]{2}) ",false);
	orbits.replace(rxs,"\\1<sup>\\2</sup>"); //superscript around electron number
	orbits.replace(rxb,"<b>\\1</b> "); //bold around element symbols
	return orbits;
}


double Element::strippedMass( double num )
{
	if ( !finite( num ) )
		return num;

	double power;
	power = 1e-6;
	while ( power < num )
		power *= 10;

	num = num / power * 10000;
	num = round( num );

	return num * power / 10000;
}


Element::~Element()
{
}



double Element::meanmass()
{
	return m_mass/m_number;
}

const QString Element::adjustRadius( RADIUSTYPE rtype )
{
	double val = 0.0;
	QString v;

	switch ( rtype )
	{
		case ATOMIC:
			val = m_RadiusAR;
			break;
		case IONIC:
			val = m_RadiusIon;
			break;
		case COVALENT:
			val = m_RadiusCR;
			break;
		case VDW:
			val = m_RadiusVDW;
			break;
	}

	if ( val <= 0 )
		v = i18n( "Value unknown" );
	else
	{
		switch ( Prefs::units() )
		{
			case 0://use SI-values (meter for length)
				v = i18n( "%1 10<sup>-12</sup> m" ).arg( QString::number( val ) );
				break;
			case 1://use picometer, the most common unit for radii
				v = i18n( "%1 pm" ).arg( QString::number( val ) );
				break;
		}
	}
	return v;
}

const QString Element::adjustUnits( const int type, double value )
{
	QString v;
	if ( type == IE  ) //an ionization energy
	{
		if ( Prefs::energies() == 0 )
		{
			value*=96.6;
			v = QString::number( value );
			v.append( " kJ/mol" );
		}
		else // use electronvolt
		{
			v = QString::number( value );
			v.append( " eV" );
		}
	}
	return v;
}

const QString Element::adjustUnits( const int type )
{
	QString v = QString::null;

	double val = 0.0; //the value to convert
	
	if ( type == BOILINGPOINT || type == MELTINGPOINT ) // convert a temperature
	{
		if ( type == BOILINGPOINT )
			val = boiling();
		else
			val = melting();

		if ( val <= 0 )
			v = i18n( "Value unknown" );
		else
		{
			switch (Prefs::temperature()) {
				case 0: //Kelvin
					v = i18n( "%1 is the temperature in Kelvin", "%1 K" ).arg( QString::number( val ) );
					break;
				case 1://Kelvin to Celsius
					val-=273.15;
					v = i18n( "%1 is the temperature in Celsius", "%1 C" ).arg( QString::number( val ) );
					break;
				case 2: // Kelvin to Fahrenheit
					val = val * 1.8 - 459.67;
					v = i18n( "%1 is the temperature in Fahrenheit", "%1 F" ).arg( QString::number( val ) );
					break;
			}
		}
	}
	else if ( type == EN ) //Electronegativity
	{
		val = electroneg();
		if ( val <= 0 )
			v = i18n( "Value not defined" );
		else
			v = QString::number( val );
	}
	else if ( type == MASS ) // its a mass
	{
		val = mass();
		if ( val <= 0 )
			v = i18n( "Value unknown" );
		else
		{
			v = QString::number( val );
			switch ( Prefs::units() )
			{
				case 0:
					v = i18n( "%1 g/mol" ).arg( QString::number( val ) );
					break;
				case 1:
					v = i18n( "%1 u" ).arg( QString::number( val ) );
					break;
			}
		}
	}
	else if ( type == DENSITY ) // its a density
	{
		val = density();

		if ( val <= 0 )
			v = i18n( "Value unknown" );
		else
		{
			switch ( Prefs::units() )
			{
				case 0://use SI (kg per cubic-meter)
					val *= 1000;
					v = i18n( "%1 kg/m<sup>3</sup>" ).arg( QString::number( val ) );
					break;
				case 1://use more common units
					if ( az() == 2 )//gasoline
					{
						v = i18n( "%1 g/L" ).arg( QString::number( val ) );
					}
					else//liquid or solid
					{
						v = i18n( "%1 g/cm<sup>3</sup>" ).arg( QString::number( val ) );
					}
					break;
			}
		}
	}
	else if ( type == DATE ) //its a date
	{
		val = date();
		if ( val < 1600 )
		{
			v = i18n( "This element was known to ancient cultures" );
		}
		else
		{
			v = i18n( "This element was discovered in the year %1" ).arg( QString::number( val ) );
		}
	}

	return v;
}

void Element::drawCrystalstructure( QPainter* p )
{
	//the height of a "line" inside an element
	int h_small = 15; //the size for the small units like elementnumber
	
	//The X-coordiante
	int X = xPos();
	
	//The Y-coordinate
	int Y = yPos();
	
	QColor color;
	QString name;
	QString structure = crystalstructure();
	if ( structure == "own"){
		color = Qt::blue;
		name = i18n( "own" );
	}else if ( structure == "bbc" ){
		color = Qt::red;
		name = i18n( "bbc" );
	}else if ( structure == "hdp" ){
		color = Qt::yellow;
		name = i18n( "hdp" );
	}else if ( structure == "ccp" ){
		color = Qt::green;
		name = i18n( "ccp" );
	}
	else{
		color = Qt::white;
		name = QString::null;
	}
		
	p->setPen( color );
	p->fillRect( X+3, Y+3,ELEMENTSIZE-6,ELEMENTSIZE-6, color );
	p->drawRoundRect( X+2, Y+2,ELEMENTSIZE-4,ELEMENTSIZE-4 );
	
	QFont symbol_font = p->font();
	symbol_font.setPointSize( 18 );
	QFont f = p->font();
	f.setPointSize( 9 );
		
	p->setFont( f );

	//top left
	p->setPen( Qt::black );
	p->drawText( X+5,Y+2 ,ELEMENTSIZE-2,h_small,Qt::AlignLeft, name );

	p->setFont( symbol_font );
	p->drawText( X+5,Y+2, ELEMENTSIZE,ELEMENTSIZE,Qt::AlignCenter, symbol() );
	
	//border
	p->setPen( Qt::black );
	p->drawRoundRect( X+1, Y+1,ELEMENTSIZE-2,ELEMENTSIZE-2);
}

void Element::drawStateOfMatter( QPainter* p, double temp )
{
	//the height of a "line" inside an element
	int h_small = 15; //the size for the small units like elementnumber

	//The X-coordiante
	int X = xPos();
	
	//The Y-coordinate
	int Y = yPos();
	
	QColor color = currentColor( temp );
	
	p->setPen( color );
	p->fillRect( X+3, Y+3,ELEMENTSIZE-6,ELEMENTSIZE-6, color );
	p->drawRoundRect( X+2, Y+2,ELEMENTSIZE-4,ELEMENTSIZE-4 );
	
	QString text;
	QFont symbol_font = p->font();
	symbol_font.setPointSize( 18 );
	QFont f = p->font();
	f.setPointSize( 9 );
		
	p->setFont( f );

	//top left
	p->setPen( Qt::black );
	text = QString::number( strippedMass( mass( ) ) );
	p->drawText( X+5,Y+2 ,ELEMENTSIZE-2,h_small,Qt::AlignLeft, text );

	text = QString::number( number() );
	p->drawText( X+5,( y )*ELEMENTSIZE - h_small, ELEMENTSIZE-2, h_small,Qt::AlignLeft, text );

	p->setFont( symbol_font );
	p->drawText( X+5,Y+2, ELEMENTSIZE,ELEMENTSIZE,Qt::AlignCenter, symbol() );
	
	//border
	p->setPen( Qt::black );
	p->drawRoundRect( X+1, Y+1,ELEMENTSIZE-2,ELEMENTSIZE-2);
}
	
QColor Element::currentColor( double temp )
{
	QColor color;
	//take the colours for the given temperature
	const int _az = az();
	if ( _az == 3 || _az == 4 )
	{ //check if the element is radioactive or artificial
		if ( _az == 3 ) color=Prefs::color_radioactive();
		if ( _az == 4 ) color=Prefs::color_artificial();
	}

	const double iButton_melting = melting();
	const double iButton_boiling = boiling();

	if ( temp < iButton_melting )
	{ //the element is solid
		color= Prefs::color_solid();
	}
	if ( temp > iButton_melting &&
			temp < iButton_boiling )
	{ //the element is liquid
		color= Prefs::color_liquid();
	}
	if ( temp > iButton_boiling )
	{ //the element is vaporous
		color= Prefs::color_vapor();
	}

	return color;
}

void Element::drawHighlight( QPainter* p, int coordinate, bool horizontal )
{
	//first: test if the element is in the selected period of group
	if ( horizontal )
	{
		if ( x != coordinate )
			return;
		//else the element is in the selected row
	}
	else if ( !horizontal )
	{
		if ( y != coordinate )
			return;
		//else the element is in the selected group
	}

	//the element matches. Now highlight it.
	//The X-coordiante
	int X = ( x-1 )*ELEMENTSIZE;

	//The Y-coordinate
	int Y = ( y-1 )*ELEMENTSIZE;

	p->fillRect( X, Y,ELEMENTSIZE,ELEMENTSIZE, Qt::darkRed );

	//now draw the elements over the red area
	drawSelf( p );
}
	
void Element::drawGradient( QPainter* p, const QString& value, const QColor& c)
{
	//the height of a "line" inside an element
	int h_small = 15; //the size for the small units like elementnumber

	//The X-coordiante
	int X = xPos();
	
	//The Y-coordinate
	int Y = yPos();

	p->setPen( c );
	p->fillRect( X+3, Y+3,ELEMENTSIZE-6,ELEMENTSIZE-6, c );
	p->drawRoundRect( X+2, Y+2,ELEMENTSIZE-4,ELEMENTSIZE-4 );
	
	p->setPen( Qt::black );
	QFont symbol_font = p->font();
	symbol_font.setPointSize( 18 );
	QFont f = p->font();
	f.setPointSize( 9 );
		
	p->setFont( f );

	p->drawText( X+5,Y+ELEMENTSIZE-h_small , ELEMENTSIZE-2, h_small,Qt::AlignLeft, value );

	p->setFont( symbol_font );
	p->drawText( X+5,Y+2, ELEMENTSIZE,ELEMENTSIZE,Qt::AlignCenter, symbol() );
	
	//border
	p->setPen( Qt::black );
	p->drawRoundRect( X+1, Y+1,ELEMENTSIZE-2,ELEMENTSIZE-2);
}

void Element::drawSelf( QPainter* p )
{
	//the height of a "line" inside an element
	int h_small = 15; //the size for the small units like elementnumber

	//The X-coordiante
	int X = xPos();
	
	//The Y-coordinate
	int Y = yPos();

	p->setPen( elementColor() );
	p->fillRect( X+3, Y+3,ELEMENTSIZE-6,ELEMENTSIZE-6, elementColor() );
	p->drawRoundRect( X+2, Y+2,ELEMENTSIZE-4,ELEMENTSIZE-4 );
	
	QString text;
	QFont symbol_font = p->font();
	symbol_font.setPointSize( 18 );
	QFont f = p->font();
	f.setPointSize( 9 );
		
	p->setFont( f );

	//top left
	p->setPen( Qt::black );
	text = QString::number( strippedMass( mass( ) ) );
	p->drawText( X+5,Y+2 ,ELEMENTSIZE+4,h_small,Qt::AlignLeft, text );

	text = QString::number( number() );
	p->drawText( X+5,Y+ELEMENTSIZE-h_small , ELEMENTSIZE-2, h_small,Qt::AlignLeft, text );

	p->setFont( symbol_font );
	p->drawText( X+5,Y+2, ELEMENTSIZE,ELEMENTSIZE,Qt::AlignCenter, symbol() );
	
	//border
	p->setPen( Qt::black );
	p->drawRoundRect( X+1, Y+1,ELEMENTSIZE-2,ELEMENTSIZE-2);
}

/*!
    This looks pretty evil and it is. The problem is that a PSE is pretty
    irregular and you cannot "calculate" the position. 
 */
void Element::setupXY()
{
 static const int posXRegular[111] = {1,18,
	 								1,2,13,14,15,16,17,18,
									1,2,13,14,15,16,17,18,
 									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
 									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,  //Element 54 (Xe)
									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,     //Element 71 (Lu)
									      4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,     //Element 71 (Lr)
									      4,5,6,7,8,9,10,11};
 static const int posYRegular[111] = {1,1,
	 								2,2, 2, 2, 2, 2, 2, 2,
									3,3, 3, 3, 3, 3, 3, 3,
 									4,4,4,4,4,4,4,4,4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 									5,5,5,5,5,5,5,5,5, 5, 5, 5, 5, 5, 5, 5, 5, 5,  //Element 54 (Xe)
						    		6,6,6,8,8,8,8,8,8, 8, 8, 8, 8, 8, 8, 8, 8,     //Element 71 (Lr)
									      6,6,6,6,6,6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
									7,7,7,9,9,9,9,9,9, 9, 9, 9, 9, 9, 9, 9, 9,
									      7,7,7,7,7,7,7,7};

 x = posXRegular[m_number-1];
 y = posYRegular[m_number-1];
}

void Element::setRadius( RADIUSTYPE type, double value, const QString& name )
{
	switch ( type )
	{
		case ATOMIC:
			m_RadiusAR = value;
			break;
		case IONIC:
			m_RadiusIon = value;
			m_ionvalue = name;
			break;
		case COVALENT:
			m_RadiusCR = value;
			break;
		case VDW:
			m_RadiusVDW = value;
			break;
	}
}

double Element::getRadius( RADIUSTYPE type )
{
	switch ( type )
	{
		case ATOMIC:
			return m_RadiusAR;
			break;
		case IONIC:
			return m_RadiusIon;
			break;
		case COVALENT:
			return m_RadiusCR;
			break;
		case VDW:
			return m_RadiusVDW;
			break;
	}
	return 0.0;
}

KalziumDataObject::KalziumDataObject()
{
	QDomDocument doc( "datadocument" );

	KURL url;
	url.setPath( locate("data", "kalzium/data/"));
	url.setFileName( "data.xml" );
	QFile layoutFile( url.path() );

	if (!layoutFile.exists())
	{
		KMessageBox::information( 0, i18n("data.xml does not exist"), i18n( "Loading File - Error" ) );
	}

	if (!layoutFile.open(IO_ReadOnly))
		KMessageBox::information( 0, i18n("data.xml IO-error"), i18n( "Loading File - IO_ReadOnly" ) );

	///Check if document is well-formed
	if (!doc.setContent(&layoutFile))
	{
		kdDebug() << "wrong xml" << endl;
		layoutFile.close();
	}
	layoutFile.close();

	ElementList = readData( doc );
}

KalziumDataObject::~KalziumDataObject()
{}

Element* KalziumDataObject::element( int number )
{
	return *( ElementList.at( number-1 ) );
}

EList KalziumDataObject::readData(  QDomDocument &dataDocument )
{
	EList list;
	QDomNodeList elementNodes; //the list of all element
	QDomElement  domElement;   //a single element

	//read in all elements
	elementNodes = dataDocument.elementsByTagName( "element" );

	for ( uint i = 0; i < elementNodes.count(); ++i )
	{//iterate through all elements
		domElement = ( const QDomElement& ) elementNodes.item( i ).toElement();

		double mass = domElement.namedItem( "mass" ).toElement().text().toDouble();
		double en = domElement.namedItem( "electronegativity" ).toElement().text().toDouble();
		double mp = domElement.namedItem( "meltingpoint" ).toElement().text().toDouble();
		double bp = domElement.namedItem( "boilingpoint" ).toElement().text().toDouble();
		double density = domElement.namedItem( "density" ).toElement().text().toDouble();
		double covalent_radius = domElement.namedItem( "radius" ).namedItem( "covalent" ).toElement().text().toDouble();
		//van der Walls-Radius
		double vdw_radius = domElement.namedItem( "radius" ).namedItem( "vdw" ).toElement().text().toDouble();
		double atomic_radius = domElement.namedItem( "radius" ).namedItem( "atomic" ).toElement().text().toDouble();
		double ionic_radius = domElement.namedItem( "radius" ).namedItem( "ionic" ).toElement().text().toDouble();
		QString ionic_charge = domElement.namedItem( "radius" ).namedItem( "ionic" ).toElement().attributeNode( "charge" ).value();
		
		int bio = domElement.namedItem( "biologicalmeaning" ).toElement().text().toInt();
		int az = domElement.namedItem( "aggregation" ).toElement().text().toInt();
		int date = domElement.namedItem( "date" ).toElement().text().toInt();
		int number = domElement.namedItem( "number" ).toElement().text().toInt();
		
		QString scientist = domElement.namedItem( "date" ).toElement().attributeNode( "scientist" ).value();
		QString crystal = domElement.namedItem( "crystalstructure" ).toElement().text();
		
		QDomElement nameE = domElement.namedItem( "name" ).toElement();
		QString name = nameE.text();
		QString origin = nameE.attributeNode( "origin" ).value();
		
		QString block = domElement.namedItem( "block" ).toElement().text();
		QString group = domElement.namedItem( "group" ).toElement().text();
		QString family = domElement.namedItem( "family" ).toElement().text();
		QString orbits = domElement.namedItem( "orbits" ).toElement().text();
		QString symbol = domElement.namedItem( "symbol" ).toElement().text();
		QString oxydation = domElement.namedItem( "oxydation" ).toElement().text();
		QString acidicbehaviour = domElement.namedItem( "acidicbehaviour" ).toElement().text();
		QString isotopes = domElement.namedItem( "isotopes" ).toElement().text();

		QDomNodeList elist = domElement.elementsByTagName( "energy" );
		QValueList<double> ionlist;
		for( uint i = 0; i < elist.length(); i++ )
		{
			ionlist.append( elist.item( i ).toElement().text().toDouble() );
		}
	
		Element *e = new Element();
		e->setDate(date);
		e->setBiologicalMeaning(bio);
		e->setAggregation(az);
		e->setNumber( number );
		e->setRadius( Element::ATOMIC, atomic_radius );
		e->setRadius( Element::IONIC, ionic_radius, ionic_charge );
		e->setRadius( Element::COVALENT, covalent_radius );
		e->setRadius( Element::VDW, vdw_radius );
		
		e->setScientist(scientist);
		e->setCrysatalstructure( crystal );
		e->setName(name);
		e->setOrigin(origin);
		e->setBlock(block);
		e->setGroup(group);
		e->setFamily(family);
		e->setOrbits(orbits);
		e->setSymbol(symbol);
		e->setOxydation(oxydation);
		e->setAcidicbehaviour(acidicbehaviour);
		e->setIsotopes(isotopes);
		e->setIonisationList( ionlist );
		
		e->setMass( mass );	
		e->setEN( en );
		e->setMeltingpoint( mp );
		e->setBoilingpoint( bp );
		e->setDensity( density );

		e->setupXY();

		list.append( e );
		coordinate point; point.x =  e->x; point.y = e->y;
		CoordinateList.append( point );
	}
	
	return list;
}
