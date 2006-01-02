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
	QLinearGradient linearGrad( elrect.topLeft(), elrect.bottomRight() );
	
	linearGrad.setColorAt( 0, QColor( Qt::yellow ) );
	linearGrad.setColorAt( 1, QColor( Qt::green ) );
	return QBrush( linearGrad );
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
