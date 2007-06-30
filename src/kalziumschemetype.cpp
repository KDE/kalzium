/***************************************************************************
 *   Copyright (C) 2005, 2006      by Pino Toscano, toscano.pino@tiscali.it      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "kalziumschemetype.h"

#include "kalziumdataobject.h"
#include "prefs.h"

#include <QBrush>
#include <QVariant>

#include <klocale.h>
#include <kdebug.h>
#include <kstandarddirs.h>

KalziumSchemeTypeFactory::KalziumSchemeTypeFactory()
{
	m_schemes << KalziumMonoColorSchemeType::instance();
	m_schemes << KalziumBlocksSchemeType::instance();
	m_schemes << KalziumIconicSchemeType::instance();
	m_schemes << KalziumFamilySchemeType::instance();
	m_schemes << KalziumGroupsSchemeType::instance();
	m_schemes << KalziumCrystalSchemeType::instance();
	m_schemes << KalziumDiscoverymapSchemeType::instance();
}

KalziumSchemeTypeFactory* KalziumSchemeTypeFactory::instance()
{
	static KalziumSchemeTypeFactory kstf;
	return &kstf;
}

KalziumSchemeType* KalziumSchemeTypeFactory::build( int id ) const
{
	if ( ( id < 0 ) || ( id >= m_schemes.count() ) )
		return 0;

	return m_schemes.at( id );
}

KalziumSchemeType* KalziumSchemeTypeFactory::build( const QByteArray& id ) const
{
	for ( int i = 0; i < m_schemes.count(); i++ )
	{
		if ( m_schemes.at( i )->name() == id )
			return m_schemes.at( i );
	}
	
	// not found
	return 0;
}

QStringList KalziumSchemeTypeFactory::schemes() const
{
	QStringList l;
	for ( int i = 0; i < m_schemes.count(); i++ )
	{
		l << m_schemes.at( i )->description();
	}
	return l;
}


KalziumSchemeType* KalziumSchemeType::instance()
{
	return 0;
}

KalziumSchemeType::KalziumSchemeType()
{
}

KalziumSchemeType::~KalziumSchemeType()
{
}

KalziumMonoColorSchemeType::KalziumMonoColorSchemeType()
  : KalziumSchemeType()
{
}

KalziumMonoColorSchemeType* KalziumMonoColorSchemeType::instance()
{
	static KalziumMonoColorSchemeType kmcst;
	return &kmcst;
}

QByteArray KalziumMonoColorSchemeType::name() const
{
	return "MonoColor";
}

QString KalziumMonoColorSchemeType::description() const
{
	return i18n( "One Colour" );
}

QBrush KalziumMonoColorSchemeType::elementBrush( int el, const QRect& elrect ) const
{
	Q_UNUSED( el );
	Q_UNUSED( elrect );
	return QBrush( Prefs::noscheme() );
}

QColor KalziumMonoColorSchemeType::textColor( int el ) const
{
	Q_UNUSED( el );
	return Qt::black;
}

QList<legendPair> KalziumMonoColorSchemeType::legendItems() const
{
	QList<legendPair> ll;
	ll << qMakePair( i18n( "All the Elements" ), QBrush( Prefs::noscheme() ) );
	return ll;
}


KalziumBlocksSchemeType::KalziumBlocksSchemeType()
  : KalziumSchemeType()
{
}

KalziumBlocksSchemeType* KalziumBlocksSchemeType::instance()
{
	static KalziumBlocksSchemeType kbst;
	return &kbst;
}

QByteArray KalziumBlocksSchemeType::name() const
{
	return "Blocks";
}

QString KalziumBlocksSchemeType::description() const
{
	return i18n( "Blocks" );
}

QBrush KalziumBlocksSchemeType::elementBrush( int el, const QRect& ) const
{
	QString block = KalziumDataObject::instance()->element( el )->dataAsString( ChemicalDataObject::periodTableBlock );

	QColor c;
	if ( block == QLatin1String( "s" ) )
		c = Prefs::block_s();
	else if ( block == QLatin1String( "p" ) )
		c = Prefs::block_p();
	else if ( block == QLatin1String( "d" ) )
		c = Prefs::block_d();
	else if ( block == QLatin1String( "f" ) )
		c = Prefs::block_f();
	else
		c = Qt::lightGray;

	return QBrush( c );
}

QColor KalziumBlocksSchemeType::textColor( int el ) const
{
	Q_UNUSED( el );
	return Qt::black;
}

QList<legendPair> KalziumBlocksSchemeType::legendItems() const
{
	QList<legendPair> ll;
	ll << qMakePair( i18n( "s-Block" ), QBrush( Prefs::block_s() ) );
	ll << qMakePair( i18n( "p-Block" ), QBrush( Prefs::block_p() ) );
	ll << qMakePair( i18n( "d-Block" ), QBrush( Prefs::block_d() ) );
	ll << qMakePair( i18n( "f-Block" ), QBrush( Prefs::block_f() ) );
	return ll;
}

///ICONIC SCHEME///

KalziumIconicSchemeType::KalziumIconicSchemeType()
  : KalziumSchemeType()
{
}

KalziumIconicSchemeType* KalziumIconicSchemeType::instance()
{
	static KalziumIconicSchemeType kist;
	return &kist;
}

QByteArray KalziumIconicSchemeType::name() const
{
	return "Iconic";
}

QString KalziumIconicSchemeType::description() const
{
	return i18n( "Iconic" );
}

QBrush KalziumIconicSchemeType::elementBrush( int el, const QRect& ) const
{
	QPixmap pixmap = KalziumDataObject::instance()->pixmap( el );
	
	return QBrush( pixmap );
}

QColor KalziumIconicSchemeType::textColor( int ) const
{
	return Qt::black;
}

QList<legendPair> KalziumIconicSchemeType::legendItems() const
{
	QList<legendPair> ll;
	ll << qMakePair( i18n( "Each element is represented by an icon which represents its use." ), QBrush( ) );
	return ll;
}


///Family///
KalziumFamilySchemeType::KalziumFamilySchemeType()
  : KalziumSchemeType()
{
}

KalziumFamilySchemeType* KalziumFamilySchemeType::instance()
{
	static KalziumFamilySchemeType kbst;
	return &kbst;
}

QByteArray KalziumFamilySchemeType::name() const
{
	return "Family";
}

QString KalziumFamilySchemeType::description() const
{
	return i18n( "Family" );
}

QBrush KalziumFamilySchemeType::elementBrush( int el, const QRect& ) const
{
	QString family = KalziumDataObject::instance()->element( el )->dataAsString( ChemicalDataObject::family );

	QColor c;

	if (  family == QLatin1String(  "Noblegas" ) ){
		c = Prefs::noble_gas();
	}
	else if (  family == QLatin1String(  "Non-Metal" ) ){
		c = Prefs::nonmetal();
	}
	else if (  family == QLatin1String(  "Rare_Earth" ) ){
		c = Prefs::rare();
	}
	else if (  family == QLatin1String(  "Alkaline_Earth" ) ){
		c = Prefs::alkaline();
	}
	else if (  family == QLatin1String(  "Alkali_Earth" ) ){
		c = Prefs::alkalie();
	}
	else if (  family == QLatin1String(  "Transition" ) ){
		c = Prefs::transition();
	}
	else if (  family == QLatin1String(  "Other_Metal" ) ){
		c = Prefs::other_metal();
	}
	else if (  family == QLatin1String(  "Metalloids" ) ){
		c = Prefs::metalloid();
	}
	else if (  family == QLatin1String(  "Halogene" ) ){
		c = Prefs::halogene();
	}
	else
		c = Qt::lightGray;

	return QBrush( c );
}

QColor KalziumFamilySchemeType::textColor( int ) const
{
	return Qt::black;
}

QList<legendPair> KalziumFamilySchemeType::legendItems() const
{
	QList<legendPair> ll;
	ll << qMakePair( i18n( "Alkaline" ),QBrush( Prefs::alkalie() ) );
	ll << qMakePair( i18n( "Rare Earth" ),QBrush( Prefs::rare() ) );
	ll << qMakePair( i18n( "Non-Metals" ),QBrush( Prefs::nonmetal() ) );
	ll << qMakePair( i18n( "Alkalie Metal" ),QBrush( Prefs::alkaline() ) );
	ll << qMakePair( i18n( "Other Metal" ),QBrush( Prefs::other_metal() ) );
	ll << qMakePair( i18n( "Halogen" ),QBrush( Prefs::halogene() ) );
	ll << qMakePair( i18n( "Transition Metal" ),QBrush( Prefs::transition() ) );
	ll << qMakePair( i18n( "Noble Gas" ),QBrush( Prefs::noble_gas() ) );
	ll << qMakePair( i18n( "Metalloid" ),QBrush( Prefs::metalloid() ) );

	return ll;
}

///GROUPS///
KalziumGroupsSchemeType::KalziumGroupsSchemeType()
  : KalziumSchemeType()
{
}

KalziumGroupsSchemeType* KalziumGroupsSchemeType::instance()
{
	static KalziumGroupsSchemeType kbst;
	return &kbst;
}

QByteArray KalziumGroupsSchemeType::name() const
{
	return "Groups";
}

QString KalziumGroupsSchemeType::description() const
{
	return i18n( "Groups" );
}

QBrush KalziumGroupsSchemeType::elementBrush( int el, const QRect& ) const
{
	QString group = KalziumDataObject::instance()->element( el )->dataAsString( ChemicalDataObject::group );

	QColor c;

	if (  group == QLatin1String(  "1" ) ){
		c = Prefs::group_1();
	}
	else if (  group == QLatin1String(  "2" ) ){
		c = Prefs::group_2();
	}
	else if (  group == QLatin1String(  "3" ) ){
		c = Prefs::group_3();
	}
	else if (  group == QLatin1String(  "4" ) ){
		c = Prefs::group_4();
	}
	else if (  group == QLatin1String(  "5" ) ){
		c = Prefs::group_5();
	}
	else if (  group == QLatin1String(  "6" ) ){
		c = Prefs::group_6();
	}
	else if (  group == QLatin1String(  "7" ) ){
		c = Prefs::group_7();
	}
	else if (  group == QLatin1String(  "8" ) ){
		c = Prefs::group_8();
	}
	else
		c = Qt::lightGray;

	return QBrush( c );
}

QColor KalziumGroupsSchemeType::textColor( int ) const
{
	return Qt::black;
}

QList<legendPair> KalziumGroupsSchemeType::legendItems() const
{
	QList<legendPair> ll;
	ll << qMakePair( i18n( "Group 1" ), QBrush( Prefs::group_1() ) );
	ll << qMakePair( i18n( "Group 2" ), QBrush( Prefs::group_2() ) );
	ll << qMakePair( i18n( "Group 3" ), QBrush( Prefs::group_3() ) );
	ll << qMakePair( i18n( "Group 4" ), QBrush( Prefs::group_4() ) );
	ll << qMakePair( i18n( "Group 5" ), QBrush( Prefs::group_5() ) );
	ll << qMakePair( i18n( "Group 6" ), QBrush( Prefs::group_6() ) );
	ll << qMakePair( i18n( "Group 7" ), QBrush( Prefs::group_7() ) );
	ll << qMakePair( i18n( "Group 7" ), QBrush( Prefs::group_8() ) );

	return ll;
}

///CRYSTAL///
KalziumCrystalSchemeType::KalziumCrystalSchemeType()
  : KalziumSchemeType()
{
}

KalziumCrystalSchemeType* KalziumCrystalSchemeType::instance()
{
	static KalziumCrystalSchemeType kbst;
	return &kbst;
}

QByteArray KalziumCrystalSchemeType::name() const
{
	return "Crystal";
}

QString KalziumCrystalSchemeType::description() const
{
	return i18n( "Crystal Structures" );
}

QBrush KalziumCrystalSchemeType::elementBrush( int el, const QRect& elrect ) const
{
	QString crystal = KalziumDataObject::instance()->element( el )->dataAsString( ChemicalDataObject::crystalstructure );

	static QString resourcepath;
	if ( resourcepath.isEmpty() )
	{
		resourcepath = KGlobal::dirs()->findResourceDir( "appdata", "data/latticeicons/" ) + "data/latticeicons/";
	}

	QString filename;
	if (  crystal == "bcc" ){
		filename = "ci.png";
	}
	else if (  crystal == "ccp" ){
		filename = "cp.png";
	}
	else if (  crystal ==  "fcc" ){
		filename = "cf.png";
	}
	else if (  crystal ==  "hcp" ){
		filename = "hp.png";
	}
	else if (  crystal ==  "rh" ){
		filename = "hr.png";//Rhombohedral primitive
	}
	else if (  crystal ==  "or" ){
		filename = "op.png";//Orthorhombic primitive
	}
	else if (  crystal ==  "mono" ){
		filename = "ms.png";//Monoclinic primitive
	}
	else if (  crystal ==  "tri" ){
		filename = "ap.png";//Triclinic
	}
	else if (  crystal ==  "tp" ){
		filename = "tp.png";//Tetragonal primitive
	}

	QBrush ret;
	if ( !filename.isEmpty() )
	{
		QPixmap pixmap( resourcepath + filename );
		ret = QBrush( pixmap.scaled( elrect.size(), Qt::KeepAspectRatio ) );
	}
	else
	{
		ret.setColor( Qt::gray );
	}

	return ret;
}

QColor KalziumCrystalSchemeType::textColor( int ) const
{
	return Qt::black;
}

QList<legendPair> KalziumCrystalSchemeType::legendItems() const
{
	static QString resourcepath;
	if ( resourcepath.isEmpty() )
	{
		resourcepath = KGlobal::dirs()->findResourceDir( "appdata", "data/latticeicons/" ) + "data/latticeicons/";
	}

	QList<legendPair> ll;
	ll << qMakePair( i18n( "bcc, body centered cubic"  ), QBrush( QPixmap( resourcepath + "ci.png" ) ) );
	ll << qMakePair( i18n( "ccp, cubic close packed" ), QBrush( QPixmap( resourcepath + "cp.png" ) ) );
	ll << qMakePair( i18n( "fcc, face centered cubic" ), QBrush( QPixmap( resourcepath + "cf.png" ) ) );
	ll << qMakePair( i18n( "hcp, hexagonal close packed" ), QBrush( QPixmap( resourcepath + "hp.png" ) ) );
	ll << qMakePair( i18n( "rh, rhombohedral" ), QBrush( QPixmap( resourcepath + "hr.png" ) ) );
	ll << qMakePair( i18n( "or, orthorhombic primitive" ), QBrush( QPixmap( resourcepath + "op.png" ) ) );
	ll << qMakePair( i18n( "ms, monoclinic" ), QBrush( QPixmap( resourcepath + "ms.png" ) ) );
	ll << qMakePair( i18n( "ap, triclinic" ), QBrush( QPixmap( resourcepath + "ap.png" ) ) );
	ll << qMakePair( i18n( "tp, tetragonal primitive" ), QBrush( QPixmap( resourcepath + "tp.png" ) ) );

	return ll;
}

////
KalziumDiscoverymapSchemeType::KalziumDiscoverymapSchemeType()
  : KalziumSchemeType()
{
}

KalziumDiscoverymapSchemeType* KalziumDiscoverymapSchemeType::instance()
{
	static KalziumDiscoverymapSchemeType kbst;
	return &kbst;
}

QByteArray KalziumDiscoverymapSchemeType::name() const
{
	return "Crystal";
}

QString KalziumDiscoverymapSchemeType::description() const
{
	return i18n( "Discovery Country");
}

QBrush KalziumDiscoverymapSchemeType::elementBrush( int el, const QRect& elrect ) const
{
	QString map = KalziumDataObject::instance()->element( el )->dataAsString( ChemicalDataObject::discoveryCountry );

	static QString resourcepath;
	if ( resourcepath.isEmpty() )
	{
		resourcepath = KGlobal::dirs()->findResourceDir( "appdata", "data/maps/" ) + "data/maps/";
	}

	QString filename;
	if (  map == "se" ){
		filename = "se.png";
	}
	else if (  map == "uk" ){
		filename = "uk.png";
	}
	else if (  map == "us" ){
		filename = "us.png";
	}
	else if (  map == "ru" ){
		filename = "ru.png";
	}
	else if (  map == "it" ){
		filename = "it.png";
	}
	else if (  map == "de" ){
		filename = "de.png";
	}
	else if (  map == "dk" ){
		filename = "dk.png";
	}
	else if (  map == "fr" ){
		filename = "fr.png";
	}
	else if (  map == "fi" ){
		filename = "fi.png";
	}
	else if (  map == "es" ){
		filename = "es.png";
	}
	else if (  map == "ancient" ){
    return QBrush( Qt::lightGray );
	}
	else if (  map == "uk,fr" ){
		filename = "ukfr.png";
	}
	else if (  map == "se,uk" ){
		filename = "ukse.png";
	}
	else if (  map == "ru,us" ){
		filename = "ruus.png";
	} else {
      return QBrush( Qt::blue);
  }

	QBrush ret;
	if ( !filename.isEmpty() )
	{
		QPixmap pixmap( resourcepath + filename );
		ret = QBrush( pixmap.scaled( elrect.size(), Qt::KeepAspectRatio ) );
	}
	else
	{
		ret.setColor( Qt::gray );
	}

	return ret;
}

QColor KalziumDiscoverymapSchemeType::textColor( int ) const
{
	return Qt::black;
}

QList<legendPair> KalziumDiscoverymapSchemeType::legendItems() const
{
	static QString resourcepath;
	if ( resourcepath.isEmpty() )
	{
		resourcepath = KGlobal::dirs()->findResourceDir( "appdata", "data/maps/" ) + "data/maps/";
	}

	QList<legendPair> ll;
	ll << qMakePair( i18n( "Germany"  ), QBrush( QPixmap( resourcepath + "de.png" ) ) );
	ll << qMakePair( i18n( "United Kindom"  ), QBrush( QPixmap( resourcepath + "uk.png" ) ) );
	ll << qMakePair( i18n( "Sweden"  ), QBrush( QPixmap( resourcepath + "se.png" ) ) );
	ll << qMakePair( i18n( "USA"  ), QBrush( QPixmap( resourcepath + "us.png" ) ) );
	ll << qMakePair( i18n( "Russia"  ), QBrush( QPixmap( resourcepath + "ru.png" ) ) );
	ll << qMakePair( i18n( "Italy"  ), QBrush( QPixmap( resourcepath + "it.png" ) ) );
	ll << qMakePair( i18n( "Denmark"  ), QBrush( QPixmap( resourcepath + "dk.png" ) ) );
	ll << qMakePair( i18n( "France"  ), QBrush( QPixmap( resourcepath + "fr.png" ) ) );
	ll << qMakePair( i18n( "Finland"  ), QBrush( QPixmap( resourcepath + "fi.png" ) ) );
	ll << qMakePair( i18n( "Spain"  ), QBrush( QPixmap( resourcepath + "es.png" ) ) );

	return ll;
}
