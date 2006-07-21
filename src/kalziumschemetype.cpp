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
	m_schemes << KalziumAcidicSchemeType::instance();
	m_schemes << KalziumGroupsSchemeType::instance();
	m_schemes << KalziumCrystalSchemeType::instance();
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

legendList KalziumMonoColorSchemeType::legendItems() const
{
	legendList ll;
	ll << qMakePair( i18n( "All the Elements" ), Prefs::noscheme() );
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

legendList KalziumBlocksSchemeType::legendItems() const
{
	legendList ll;
	ll << qMakePair( i18n( "s-Block" ), Prefs::block_s() );
	ll << qMakePair( i18n( "p-Block" ), Prefs::block_p() );
	ll << qMakePair( i18n( "d-Block" ), Prefs::block_d() );
	ll << qMakePair( i18n( "f-Block" ), Prefs::block_f() );
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

legendList KalziumIconicSchemeType::legendItems() const
{
	legendList ll;
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

legendList KalziumFamilySchemeType::legendItems() const
{
	legendList ll;
	ll << qMakePair( i18n( "Alkaline" ),Prefs::alkalie() );
	ll << qMakePair( i18n( "Rare Earth" ),Prefs::rare() );
	ll << qMakePair( i18n( "Non-Metals" ),Prefs::nonmetal() );
	ll << qMakePair( i18n( "Alkalie-Metal" ),Prefs::alkaline() );
	ll << qMakePair( i18n( "Other Metal" ),Prefs::other_metal() );
	ll << qMakePair( i18n( "Halogene" ),Prefs::halogene() );
	ll << qMakePair( i18n( "Transition Metal" ),Prefs::transition() );
	ll << qMakePair( i18n( "Noble Gas" ),Prefs::noble_gas() );
	ll << qMakePair( i18n( "Metalloid" ),Prefs::metalloid() );

	return ll;
}


///Acidic///
KalziumAcidicSchemeType::KalziumAcidicSchemeType()
  : KalziumSchemeType()
{
}

KalziumAcidicSchemeType* KalziumAcidicSchemeType::instance()
{
	static KalziumAcidicSchemeType kbst;
	return &kbst;
}

QByteArray KalziumAcidicSchemeType::name() const
{
	return "Acidic";
}

QString KalziumAcidicSchemeType::description() const
{
	return i18n( "Acidic behaviour" );
}

QBrush KalziumAcidicSchemeType::elementBrush( int el, const QRect& ) const
{
	QString acid = KalziumDataObject::instance()->element( el )->dataAsString( ChemicalDataObject::acidicbehaviour );

	QColor c;

	if (  acid == QLatin1String(  "0" ) ){
		c = Prefs::beh_acidic();
	}
	else if (  acid == QLatin1String(  "1" ) ){
		c = Prefs::beh_basic();
	}
	else if (  acid == QLatin1String(  "2" ) ){
		c = Prefs::beh_neutral();
	}
	else if (  acid == QLatin1String(  "3" ) ){
		c = Prefs::beh_amphoteric();
	}
	else
		c = Qt::lightGray;

	return QBrush( c );
}

QColor KalziumAcidicSchemeType::textColor( int ) const
{
	return Qt::black;
}

legendList KalziumAcidicSchemeType::legendItems() const
{
	legendList ll;
	ll << qMakePair( i18n( "Basic" ), Prefs::beh_basic() );
	ll << qMakePair( i18n( "Acidic" ), Prefs::beh_acidic() );
	ll << qMakePair( i18n( "Neutral" ), Prefs::beh_neutral() );
	ll << qMakePair( i18n( "Amphoteric" ), Prefs::beh_amphoteric() );

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

legendList KalziumGroupsSchemeType::legendItems() const
{
	legendList ll;
	ll << qMakePair( i18n( "Group 1" ), Prefs::group_1() );
	ll << qMakePair( i18n( "Group 2" ), Prefs::group_2() );
	ll << qMakePair( i18n( "Group 3" ), Prefs::group_3() );
	ll << qMakePair( i18n( "Group 4" ), Prefs::group_4() );
	ll << qMakePair( i18n( "Group 5" ), Prefs::group_5() );
	ll << qMakePair( i18n( "Group 6" ), Prefs::group_6() );
	ll << qMakePair( i18n( "Group 7" ), Prefs::group_7() );
	ll << qMakePair( i18n( "Group 7" ), Prefs::group_8() );

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
	if (  crystal == "own" ){
		filename = "1.png";
	}
	else if (  crystal == "bcc" ){
		filename = "2.png";
	}
	else if (  crystal == "hdp" ){
		filename = "3.png";
	}
	else if (  crystal == "ccp" ){
		filename = "4.png";
	}
	else if (  crystal ==  "hcp" ){
		filename = "5.png";
	}
	else if (  crystal ==  "fcc" ){
		filename = "6.png";
	}
	else if (  crystal ==  "d" ){
		filename = "7.png";
	}
	else if (  crystal ==  "sc" ){
		filename = "8.png";
	}
	else if (  crystal ==  "tet" ){
		filename = "1.png";
	}
	else if (  crystal ==  "rh" ){
	}
	else if (  crystal ==  "or" ){
	}
	else if (  crystal ==  "mono" ){
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

legendList KalziumCrystalSchemeType::legendItems() const
{
	legendList ll;

	return ll;
}
