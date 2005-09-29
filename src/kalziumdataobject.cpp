/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include "kalziumdataobject.h"

#include "isotope.h"
#include "spectrum.h"

#include <qdom.h>
#include <qfile.h>

#include <klocale.h>
#include <kdebug.h>
#include <klocale.h>
#include <kurl.h>
#include <kstandarddirs.h>
#include <kapplication.h>

KalziumDataObject* KalziumDataObject::instance()
{
	static KalziumDataObject kdo;
	return &kdo;
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
		kdDebug() << "data.xml not found, exiting" << endl;
		kapp->exit(0);
		return;
	}

	if (!layoutFile.open(IO_ReadOnly))
	{
		kdDebug() << "data.xml IO-error" << endl;
		return;
	}

	// Check if the document is well-formed
	if (!doc.setContent(&layoutFile))
	{
		kdDebug() << "wrong xml" << endl;
		layoutFile.close();
		return;
	}
	layoutFile.close();

	ElementList = readData( doc );
	m_numOfElements = ElementList.count();
}

KalziumDataObject::~KalziumDataObject()
{
}

Element* KalziumDataObject::element( int number )
{
	// checking that we are requesting a valid element
	if ( ( number <= 0 ) || ( number > m_numOfElements ) )
		return 0;
	return *( ElementList.at( number-1 ) );
}

EList KalziumDataObject::readData(  QDomDocument &dataDocument )
{
	EList list;
	QDomNodeList elementNodes; //the list of all element
	QDomElement  domElement;   //a single element

	//read in all elements
	elementNodes = dataDocument.elementsByTagName( "element" );

	const uint count = elementNodes.count();

	for ( uint i = 0; i < count; ++i )
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
		int period = domElement.namedItem( "period" ).toElement().text().toInt();
		int artificial = domElement.namedItem( "artificial" ).toElement().text().toInt();
		int date = domElement.namedItem( "date" ).toElement().text().toInt();
		int number = domElement.namedItem( "number" ).toElement().text().toInt();
		int abundance = domElement.namedItem( "abundance" ).toElement().text().toInt();
		
		QString scientist = domElement.namedItem( "date" ).toElement().attributeNode( "scientist" ).value();
		QString crystal = domElement.namedItem( "crystalstructure" ).toElement().text();
		
		QDomElement nameE = domElement.namedItem( "name" ).toElement();
		QString name = nameE.text();
		QString origin = i18n( nameE.attributeNode( "origin" ).value().utf8() );
		
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
			double alphapercentage = iso.attributeNode( "alphapercentage" ).value().toDouble();
			double betapluspercentage = iso.attributeNode( "betapluspercentage" ).value().toDouble();
			double betaminuspercentage = iso.attributeNode( "betaminuspercentage" ).value().toDouble();
			double ecpercentage = iso.attributeNode( "ecpercentage" ).value().toDouble();
			double alphadecay = iso.attributeNode( "alphadecay" ).value().toDouble();
			double betaplusdecay = iso.attributeNode( "betaplusdecay" ).value().toDouble();
			double betaminusdecay = iso.attributeNode( "betaminusdecay" ).value().toDouble();
			double ecdecay = iso.attributeNode( "ecdecay" ).value().toDouble();
			QString spin = iso.attributeNode( "spin" ).value();
			QString magmoment = iso.attributeNode( "magmoment" ).value();
			
						
			Isotope *isotope = new Isotope( neutrons, 
					number, 
					percentage, 
					weight, 
					halflife, 
					format, 
					alphadecay, 
					betaplusdecay, 
					betaminusdecay, 
					ecdecay, 
					alphapercentage, 
					betapluspercentage, 
					betaminuspercentage, 
					ecpercentage, 
					spin, 
					magmoment );
			isolist.append( isotope );
		}

		QDomNodeList spectrumList = domElement.namedItem( "spectra" ).toElement().elementsByTagName( "spectrum" );

		Element *e = new Element();
		e->setDate(date);
		e->setBiologicalMeaning(bio);
		e->setNumber( number );
		e->setName(i18n(name.utf8()));
		e->setRadius( Element::ATOMIC, atomic_radius );
		e->setRadius( Element::IONIC, ionic_radius, ionic_charge );
		e->setRadius( Element::COVALENT, covalent_radius );
		e->setRadius( Element::VDW, vdw_radius );
		e->setAbundance( abundance );

		if ( artificial == 1 )
			e->setArtificial();
		if ( radioactive == 1 )
			e->setRadioactive();
		
		e->setScientist(scientist);
		e->setPeriod( period );
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

		Spectrum *spectrum = new Spectrum();
		bool spectrum_temp = false;
		if ( spectrumList.length() > 0 )
			spectrum_temp = true;
		for( uint i = 0; i < spectrumList.length(); i++ )
		{
			Spectrum::band b;
			QDomElement spec = spectrumList.item( i ).toElement();
			
			b.intensity = spec.attributeNode( "intensity" ).value().toInt();
			b.wavelength = spec.attributeNode( "wavelength" ).value().toDouble()/10.0;
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
		if ( spectrum_temp ) //if no spectrumdata are found don't use this object
			e->setSpectrum( spectrum );
		e->setHasSepctrum( spectrum_temp );

		list.append( e );
		coordinate point; point.x =  e->x; point.y = e->y;
		CoordinateList.append( point );
	}

	return list;
}

