/***************************************************************************
 *   Copyright (C) 2005      by Pino Toscano, toscano.pino@tiscali.it      *
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
#include <QLinearGradient>
#include <QVariant>

#include <klocale.h>

KalziumSchemeTypeFactory::KalziumSchemeTypeFactory()
{
	m_schemes << KalziumMonoColorSchemeType::instance();
	m_schemes << KalziumBlocksSchemeType::instance();
	m_schemes << KalziumIconicSchemeType::instance();
	m_schemes << KalziumFamilySchemeType::instance();
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
	QLinearGradient linearGrad( elrect.topLeft(), elrect.bottomRight() );
	linearGrad.setColorAt( 0, Prefs::noscheme() );
	linearGrad.setColorAt( 1, Prefs::noscheme() );
	return QBrush( linearGrad );
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

QBrush KalziumBlocksSchemeType::elementBrush( int el, const QRect& elrect ) const
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

	QLinearGradient linearGrad( elrect.topLeft(), elrect.bottomRight() );
	linearGrad.setColorAt( 0, c );
	linearGrad.setColorAt( 1, c );
	return QBrush( linearGrad );
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

QBrush KalziumIconicSchemeType::elementBrush( int el, const QRect& elrect ) const
{
	QPixmap pixmap = KalziumDataObject::instance()->pixmap( el );
	
	return QBrush( pixmap );
}

QColor KalziumIconicSchemeType::textColor( int el ) const
{
	Q_UNUSED( el );
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

QBrush KalziumFamilySchemeType::elementBrush( int el, const QRect& elrect ) const
{
	QString family = KalziumDataObject::instance()->element( el )->dataAsString( ChemicalDataObject::family );

	QColor c;

	if (  family == QLatin1String(  "Noblegas" ) ){
		c = Prefs::noble_gas();
	}
	if (  family == QLatin1String(  "Non-Metal" ) ){
		c = Prefs::nonmetal();
	}
	if (  family == QLatin1String(  "Rare_Earth" ) ){
		c = Prefs::rare();
	}
	if (  family == QLatin1String(  "Alkaline_Earth" ) ){
		c = Prefs::alkaline();
	}
	if (  family == QLatin1String(  "Alkali_Earth" ) ){
		c = Prefs::alkalie();
	}
	if (  family == QLatin1String(  "Transition" ) ){
		c = Prefs::transition();
	}
	if (  family == QLatin1String(  "Other_Metal" ) ){
		c = Prefs::other_metal();
	}
	if (  family == QLatin1String(  "Metalloids" ) ){
		c = Prefs::metalloid();
	}
	if (  family == QLatin1String(  "Halogene" ) ){
		c = Prefs::halogene();
	}

	QLinearGradient linearGrad( elrect.topLeft(), elrect.bottomRight() );
	linearGrad.setColorAt( 0, c );
	linearGrad.setColorAt( 1, c );
	return QBrush( linearGrad );
}

QColor KalziumFamilySchemeType::textColor( int el ) const
{
	Q_UNUSED( el );
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
