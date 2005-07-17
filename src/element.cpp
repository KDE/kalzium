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
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include "element.h"
#include "prefs.h"
#include "spectrum.h"

#include <qdom.h>
#include <qfile.h>
#include <qpainter.h>
#include <qregexp.h>

#include <kdebug.h>
#include <klocale.h>
#include <kurl.h>
#include <kstandarddirs.h>

#include <math.h>

Element::Element()
{
	m_radioactive = false;
	m_artificial = false;
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


double Element::strippedValue( double num )
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
		v = i18n( "%1 is a length, eg: 12.3 pm", "%1 pm" ).arg( QString::number( val ) );
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
	else if ( type == EA ) //Electron affinity
	{
		val = electroaf();
		if ( val == 0.0 )
			v = i18n( "Value not defined" );
		else
		{
			if ( Prefs::energies() == 0 )
			{
				v = i18n( "%1 kJ/mol" ).arg( QString::number( val ) );
			}
			else // use electronvolt
			{
				val/=96.6;
				v = i18n( "%1 eV" ).arg( QString::number( val ) );
			}
		}
	}
	else if ( type == MASS ) // its a mass
	{
		val = mass();
		if ( val <= 0 )
			v = i18n( "Value unknown" );
		else
			v = i18n( "%1 u" ).arg( QString::number( val ) );
	}
	else if ( type == DENSITY ) // its a density
	{
		val = density();

		if ( val <= 0 )
			v = i18n( "Value unknown" );
		else
		{
			if ( boiling() < 295.0 )//gasoline
			{
				v = i18n( "%1 g/L" ).arg( QString::number( val ) );
			}
			else//liquid or solid
			{
				v = i18n( "%1 g/cm<sup>3</sup>" ).arg( QString::number( val ) );
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
	p->fillRect( X, Y,ELEMENTSIZE,ELEMENTSIZE, color );
	
	QString text;
	QFont symbol_font = p->font();
	symbol_font.setPointSize( 18 );
	QFont f = p->font();
	f.setPointSize( 9 );
		
	p->setFont( f );

	//top left
	p->setPen( Qt::black );
	text = QString::number( strippedValue( mass( ) ) );
	p->drawText( X,Y ,ELEMENTSIZE,h_small,Qt::AlignCenter, text );

	text = QString::number( number() );
	p->drawText( X,Y+ELEMENTSIZE-h_small , ELEMENTSIZE, h_small,Qt::AlignCenter, text );

	p->setFont( symbol_font );
	p->drawText( X,Y, ELEMENTSIZE,ELEMENTSIZE,Qt::AlignCenter, symbol() );
	
	//border
	p->setPen( Qt::black );
	p->drawRect( X, Y,ELEMENTSIZE+1,ELEMENTSIZE+1);
}
	
QColor Element::currentColor( const double temp )
{
	QColor color;
	
	//take the colours for the given temperature
	const double iButton_melting = melting();
	const double iButton_boiling = boiling();

	//If either the mp or bp is not known return
	//This is to avoid undefined behaviour
	if ( iButton_boiling <= 0.0 || iButton_melting <= 0.0 )
		return Qt::lightGray;

	if ( temp < iButton_melting )
	{ //the element is solid
		color= Prefs::color_solid();
	}
	else if ( temp > iButton_melting &&
			temp < iButton_boiling ) 
	{ //the element is liquid
		color= Prefs::color_liquid();
	}
	else if ( temp > iButton_boiling )
	{ //the element is vaporous
		color= Prefs::color_vapor();
	}
	else
		color = Qt::lightGray;

	return color;
}

	
void Element::drawGradient( QPainter* p, const QString& value, const QColor& c)
{
	//Set the elementColor to c to make the overviewwidget show
	//the correct color
	setElementColor( c );
	
	//the height of a "line" inside an element
	int h_small = 15; //the size for the small units like elementnumber

	//The X-coordiante
	int X = xPos();
	
	//The Y-coordinate
	int Y = yPos();

	p->setPen( c );
	p->fillRect( X, Y,ELEMENTSIZE,ELEMENTSIZE, c );
	
	p->setPen( Qt::black );
	QFont symbol_font = p->font();
	symbol_font.setPointSize( 18 );
	QFont f = p->font();
	f.setPointSize( 9 );
		
	p->setFont( f );

	p->drawText( X,Y+ELEMENTSIZE-h_small , ELEMENTSIZE, h_small,Qt::AlignCenter, value );

	p->setFont( symbol_font );
	p->drawText( X,Y, ELEMENTSIZE,ELEMENTSIZE,Qt::AlignCenter, symbol() );
	
	//border
	p->setPen( Qt::black );
	p->drawRect( X, Y,ELEMENTSIZE+1,ELEMENTSIZE+1);
}

void Element::drawSelf( QPainter* p, bool simple, bool isCrystal )
{
	//the height of a "line" inside an element
	int h_small = 15; //the size for the small units like elementnumber

	//The X-coordiante
	int X = xPos();
	
	//The Y-coordinate
	int Y = yPos();

	p->setPen( elementColor() );
	p->fillRect( X, Y,ELEMENTSIZE,ELEMENTSIZE, elementColor() );
	
	QString text;
	QFont symbol_font = p->font();
	symbol_font.setPointSize( 18 );
	QFont f = p->font();
	f.setPointSize( 9 );
		
	p->setFont( f );
	p->setPen( Qt::black );

	if ( !simple )
	{//the user only want a simply pse, no weight the cell
		QString text;
		if ( isCrystal )
		{
			QString structure = crystalstructure();
			/**
			 * hcp: hexagonal close packed
			 * fcc: face centered cubic
			 * krz/bcc body centered cubic// kubisch raumzentriert
			 * kdp: kubisch dicht gepackt
			 * hdp: hexagonal dicht gepackt
			 * ccp: cubic close packed // kubisch dichteste Kugelpackung
			 */
			if ( structure == "own")
				text = i18n( "this means, the element has its 'own' structur", "own" );
			else if ( structure == "bcc" )
				text = i18n( "Crystalsystem body centered cubic", "bcc" );
			else if ( structure == "hdp" )
				text = i18n( "Crystalsystem hexagonal dense packed", "hdp" );
			else if ( structure == "ccp" )
				text = i18n( "Crystalsystem cubic close packed", "ccp" );
//			else
//				text = QString::null;
		}
		else
			text = QString::number( strippedValue( mass( ) ) );
		p->drawText( X,Y ,ELEMENTSIZE,h_small,Qt::AlignCenter, text );
	}

	text = QString::number( number() );
	p->drawText( X,Y+ELEMENTSIZE-h_small , ELEMENTSIZE, h_small,Qt::AlignCenter, text );

	p->setFont( symbol_font );
	if ( !simple )
		p->drawText( X,Y, ELEMENTSIZE,ELEMENTSIZE,Qt::AlignCenter, symbol() );
	else
		p->drawText( X,Y, ELEMENTSIZE,ELEMENTSIZE,Qt::AlignHCenter, symbol() );
	
	p->setPen( Qt::black );
	p->drawRect( X, Y,ELEMENTSIZE+1,ELEMENTSIZE+1);
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

double Element::radius( RADIUSTYPE type )
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

int Element::xPos() const
{
	return ( x-1 )*ELEMENTSIZE;
}

int Element::yPos() const
{
	// mind the small gap over rare earth!
	int tmp_y = ( y-1 )*ELEMENTSIZE + ELEMENTSIZE;

	// 57=Lanthanum, 72=Hafnium, 89=Actinium & 104=Rutherfordium (i.e., if
	// n_number is in rare earth's block)
	if ( (m_number > 57 && m_number < 72) || (m_number > 89 && m_number < 104) )
		tmp_y += ELEMENTSIZE/3;

	return tmp_y;
}

QPoint Element::pos() const
{
	return QPoint( xPos(), yPos() );
}

QPoint Element::coords() const
{
	return QPoint( x, y );
}

KalziumDataObject::KalziumDataObject()
{
	QDomDocument doc( "datadocument" );

	KURL url;
	url.setPath( locate("data", "kalzium/data/"));
	url.setFileName( "data.xml" );
	QFile layoutFile( url.path() );

	if (!layoutFile.exists())
		kdDebug() << "data.xml not found" << endl;

	if (!layoutFile.open(IO_ReadOnly))
		kdDebug() << "data.xml IO-error" << endl;

	///Check if document is well-formed
	if (!doc.setContent(&layoutFile))
	{
		kdDebug() << "wrong xml" << endl;
		layoutFile.close();
	}
	layoutFile.close();

	ElementList = readData( doc );
	m_numOfElements = ElementList.count();
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
		double ea = domElement.namedItem( "electronaffinity" ).toElement().text().toDouble();
		double mp = domElement.namedItem( "meltingpoint" ).toElement().text().toDouble();
		double bp = domElement.namedItem( "boilingpoint" ).toElement().text().toDouble();
		double density = domElement.namedItem( "density" ).toElement().text().toDouble();
		double covalent_radius = domElement.namedItem( "radius" ).namedItem( "covalent" ).toElement().text().toDouble();
		//van der Waals-Radius
		double vdw_radius = domElement.namedItem( "radius" ).namedItem( "vdw" ).toElement().text().toDouble();
		double atomic_radius = domElement.namedItem( "radius" ).namedItem( "atomic" ).toElement().text().toDouble();
		double ionic_radius = domElement.namedItem( "radius" ).namedItem( "ionic" ).toElement().text().toDouble();
		QString ionic_charge = domElement.namedItem( "radius" ).namedItem( "ionic" ).toElement().attributeNode( "charge" ).value();
		
		int bio = domElement.namedItem( "biologicalmeaning" ).toElement().text().toInt();
		int radioactive = domElement.namedItem( "radioactive" ).toElement().text().toInt();
		int artificial = domElement.namedItem( "artificial" ).toElement().text().toInt();
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

		QDomNodeList elist = domElement.elementsByTagName( "energy" );
		QValueList<double> ionlist;
		for( uint i = 0; i < elist.length(); i++ )
		{
			ionlist.append(  elist.item(  i ).toElement().text().toDouble() );
		}
		
		//now read in all the date for the isotopes
		QDomNodeList isotopelist = domElement.elementsByTagName( "isotope" );
		QValueList<Isotope*> isolist;
		for( uint i = 0; i < isotopelist.length(); i++ )
		{
			QDomElement iso = isotopelist.item( i ).toElement();
			double halflife = iso.attributeNode( "halflife" ).value().toDouble();
			double weight = iso.attributeNode( "weight" ).value().toDouble();
			QString format = iso.attributeNode( "halflifeformat" ).value();
			int neutrons  = iso.attributeNode( "neutron" ).value().toInt();
			double percentage = iso.attributeNode( "percentage" ).value().toDouble();
			QString alphadecay = iso.attributeNode( "alphadecay" ).value();
			QString betaplusdecay = iso.attributeNode( "betaplusdecay" ).value();
			QString betaminusdecay = iso.attributeNode( "betaminusdecay" ).value();
			QString ecdecay = iso.attributeNode( "ecdecay" ).value();
			double decayenergy = iso.attributeNode( "decayenergy" ).value().toDouble();
			QString spin = iso.attributeNode( "spin" ).value();
			QString magmoment = iso.attributeNode( "magmoment" ).value();
			
			bool alphadecay_ = false, betaminusdecay_ = false, betaplusdecay_ = false, ecdecay_ = false;
			if ( betaplusdecay == "true" ) betaplusdecay_ = true;
			if ( betaminusdecay == "true" ) betaminusdecay_ = true;
			if ( ecdecay == "true" ) ecdecay_ = true;
			if ( alphadecay == "true" ) alphadecay_ = true;
			

			Isotope *isotope = new Isotope( neutrons, percentage, weight, halflife, format, alphadecay_, betaplusdecay_, betaminusdecay_, ecdecay_, decayenergy, spin, magmoment );
			isolist.append( isotope );
		}

		QDomNodeList spectrumList = domElement.namedItem( "spectra" ).toElement().elementsByTagName( "spectrum" );

		Spectrum *spectrum = new Spectrum();
		for( uint i = 0; i < spectrumList.length(); i++ )
		{
			Spectrum::band b;
			QDomElement spec = spectrumList.item( i ).toElement();
			
			b.intensity = spec.attributeNode( "intensity" ).value().toInt();
			b.wavelength = spec.attributeNode( "wavelength" ).value().toDouble();
			b.aki = spec.attributeNode( "aki" ).value().toDouble();
			b.energy1 = spec.attributeNode( "energy1" ).value().toDouble();
			b.energy2 = spec.attributeNode( "energy2" ).value().toDouble();
			b.electronconfig1 = spec.attributeNode( "electronconfig1" ).value();
			b.electronconfig2 = spec.attributeNode( "electronconfig1" ).value();
			b.term1 = spec.attributeNode( "term1" ).value();
			b.term2 = spec.attributeNode( "term2" ).value();
			b.J1 = spec.attributeNode( "J1" ).value();
			b.J2 = spec.attributeNode( "J2" ).value();

			spectrum->addBand( b );
		}
	
		Element *e = new Element();
		e->setSpectrum( spectrum );
		e->setDate(date);
		e->setBiologicalMeaning(bio);
		e->setNumber( number );
		e->setName(i18n(name.utf8()));
		e->setRadius( Element::ATOMIC, atomic_radius );
		e->setRadius( Element::IONIC, ionic_radius, ionic_charge );
		e->setRadius( Element::COVALENT, covalent_radius );
		e->setRadius( Element::VDW, vdw_radius );

		if ( artificial == 1 )
			e->setArtificial();
		if ( radioactive == 1 )
			e->setRadioactive();
		
		e->setScientist(scientist);
		e->setCrysatalstructure( crystal );
		e->setOrigin(origin);
		e->setBlock(block);
		e->setGroup(group);
		e->setFamily(family);
		e->setOrbits(orbits);
		e->setSymbol(symbol);
		e->setOxydation(oxydation);
		e->setAcidicbehaviour(acidicbehaviour);
		e->setIonisationList( ionlist );
		e->setIsotopeList( isolist );
		
		e->setMass( mass );	
		e->setEN( en );
		e->setEA( ea );
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

const int KalziumDataObject::numberOfElements() const
{
	return m_numOfElements;
}

Isotope::Isotope(  int neutrons, double percentage, double weight, double halflife, QString format, bool alphadecay, bool betaplusdecay, bool betaminusdecay, bool ecdecay, double decayenergy, QString spin, QString magmoment )
{
	m_neutrons = neutrons;
	m_percentage = percentage;
	m_halflife = halflife;
	m_format = format;
	m_weight = weight;
	m_alphadecay = alphadecay;
	m_betaplusdecay = betaplusdecay;
	m_betaminusdecay = betaminusdecay;
	m_ecdecay = ecdecay;
	m_decayenergy = decayenergy;
	m_spin = spin;
	m_magmoment = magmoment;
}

QString Isotope::halflifeToHtml() const
{
	QString halflife;

	if ( !seconds() )//years
	{
		if ( m_halflife > 1000000 )
			halflife = i18n("%1 million years").arg( m_halflife/1000000.0 );
		if ( m_halflife > 1000000000 )
			halflife = i18n("%1 billion years").arg( m_halflife/1000000000.0 );
		else
			halflife = i18n("%1 years").arg( m_halflife );
	}
	else
	{
		if ( m_halflife < 120 )
			halflife = i18n("%1 seconds").arg( m_halflife );
		else if ( m_halflife > 1000 )
			halflife = i18n("%1 minutes").arg( m_halflife/60.0 );
		else if ( m_halflife > 3600 )
			halflife = i18n("%1 hours").arg( m_halflife/( 60*60 ) );
		if ( m_halflife > 86400 )   //one day
			halflife = i18n("%1 days").arg( m_halflife/( 60*60*24 ) );
		if ( m_halflife > ( 31536000 * 2 ) ) //two year
			halflife = i18n("%1 years").arg( m_halflife/( 3600.0*365 ));
	}

	return halflife;
}
