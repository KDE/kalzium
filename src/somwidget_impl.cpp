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

#include <qdom.h>
#include <qfile.h>
#include <qlabel.h>
#include <qvaluelist.h>

#include <kdebug.h>
#include <kfiledialog.h>
#include <knuminput.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include <math.h>

#include "element.h"
#include <qslider.h>

SOMWidgetIMPL::SOMWidgetIMPL( QValueList<Element*> l, QWidget *parent, const char* name )
	: SOMWidget( parent,name )
{
	m_list = l;
	
	connect( temp_slider, 	SIGNAL( valueChanged( int ) ),
			 this, 			SLOT( slotTemp( int ) ) );
}

void SOMWidgetIMPL::slotTemp( int temp )
{
	QString appBaseDir = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
	appBaseDir.append("kalzium/data/");
	appBaseDir.append("bg.jpg");
	QString htmlcode = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body background=\"" ;
	htmlcode += appBaseDir + "\">";

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
			listMeltingPointValue << QString::number( ( *it )->melting() );
		}
		if ( ( ( *it )->boiling() > 0.0 ) && fabs( ( *it )->boiling() - temp ) <= threshold )
		{
			listBoilingPoint << ( *it )->elname().utf8();
			listBoilingPointValue << QString::number( ( *it )->boiling() );
		}
	}
	if ( listMeltingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with melting point around this temperature:" ) + "<br><ul type=\"disc\">";
		for ( uint i = 0; i < listMeltingPoint.count(); i++ )
		{
			htmlcode += i18n( "For example: Carbon (300K)", "<li>%1</li> (%2K)" ).arg( listMeltingPoint[i] ).arg( listMeltingPointValue[i]);
		}
		htmlcode += "</ul><br>";
	}
	if ( listBoilingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with boiling point around this temperature:" ) + "<br><ul type=\"disc\">";
		for ( uint i = 0; i < listBoilingPoint.count(); i++ )
		{
			htmlcode += i18n( "For example: Carbon (300K)", "<li>%1</li> (%2K)" ).arg( listBoilingPoint[i] ).arg( listBoilingPointValue[i]);
		}
		htmlcode += "</ul><br>";
	}

	htmlcode += "</body></html>";

	kdDebug() << htmlcode << endl;

	text->setText( htmlcode );

}

#include "somwidget_impl.moc"
