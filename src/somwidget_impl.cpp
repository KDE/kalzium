/***************************************************************************
    copyright            : (C) 2005 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "somwidget_impl.h"

#include <qslider.h>
#include <qtextedit.h>
#include <qvaluelist.h>

#include <kdebug.h>
#include <knuminput.h>
#include <klocale.h>

#include <math.h>

#include "element.h"

SOMWidgetIMPL::SOMWidgetIMPL( QValueList<Element*> l, QWidget *parent, const char* name )
	: SOMWidget( parent,name )
{
	m_list = l;

	text->setAlignment( text->alignment() | Qt::WordBreak );
	text->setTextFormat( Qt::RichText );
	text->setReadOnly( true );
	text->setPaletteBackgroundColor( paletteBackgroundColor() );
	text->setFrameStyle( QFrame::NoFrame );

	m_htmlBegin = "<qt>";
	m_htmlEnd = "</qt>";

	connect( temp_slider, 	SIGNAL( valueChanged( int ) ),
			 this, 			SLOT( slotTemp( int ) ) );
}

void SOMWidgetIMPL::slotTemp( int temp )
{
	static const int threshold = 25;

	QValueList<Element*>::ConstIterator it = m_list.begin();
	const QValueList<Element*>::ConstIterator itEnd = m_list.end();

	QStringList listMeltingPoint;
	QStringList listBoilingPoint;
	QStringList listBoilingPointValue;
	QStringList listMeltingPointValue;
	for ( ; it != itEnd; ++it )
	{
		if ( ( ( *it )->melting() > 0.0 ) && fabs( ( *it )->melting() - temp ) <= threshold )
		{
			listMeltingPoint << ( *it )->elname();
			listMeltingPointValue << ( *it )->adjustUnits( Element::MELTINGPOINT );
		}
		if ( ( ( *it )->boiling() > 0.0 ) && fabs( ( *it )->boiling() - temp ) <= threshold )
		{
			listBoilingPoint << ( *it )->elname();
			listBoilingPointValue << ( *it )->adjustUnits( Element::BOILINGPOINT );
		}
	}
	QString htmlcode;
	if ( listMeltingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with melting point around this temperature:" ) + "<br>";
		for ( uint i = 0; i < listMeltingPoint.count(); i++ )
		{
			htmlcode += "&nbsp;<b>&middot;</b>&nbsp;" + i18n( "For example: Carbon (300K)", "%1 (%2)" ).arg( listMeltingPoint[i] ).arg( listMeltingPointValue[i] ) + "<br>";
		}
		htmlcode += "<br>";
	}
	else
	{
		htmlcode += i18n( "No elements with a melting point around this temperature" );
		htmlcode += "<br>";
	}
	if ( listBoilingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with boiling point around this temperature:" ) + "<br>";
		for ( uint i = 0; i < listBoilingPoint.count(); i++ )
		{
			htmlcode += "&nbsp;<b>&middot;</b>&nbsp;" + i18n( "For example: Carbon (300K)", "%1 (%2)" ).arg( listBoilingPoint[i] ).arg( listBoilingPointValue[i] ) + "<br>";
		}
		htmlcode += "<br>";
	}
	else
	{
		htmlcode += i18n( "No elements with a boiling point around this temperature" );
		htmlcode += "<br>";
	}

//	kdDebug() << m_htmlBegin + htmlcode + m_htmlEnd << endl;

	text->setText( m_htmlBegin + htmlcode + m_htmlEnd );

}

#include "somwidget_impl.moc"
