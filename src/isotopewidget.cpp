/***************************************************************************

    copyright            : (C) 2004 by Carsten Niehaus
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
#include "isotopewidget.h"

//KDE-Includes
#include <kdebug.h>
#include <klocale.h>

//QT-Includes
#include <qpainter.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qstring.h>

IsotopeWidget::IsotopeWidget( QWidget *parent, const char* name )
	: QWidget( parent, name )
{
}

void IsotopeWidget::setIsotopes( const QString& isotopes_string )
{
	QVBoxLayout *vbox = new QVBoxLayout( this );
		
	QString isotopes = isotopes_string;

	int pos;
	int l;
	
	for ( int num = 0; num < isotopes_string.contains( ";" ) ; ++num )
	{
		pos = isotopes.find( ";" );
		l = isotopes.length();

		QString str = isotopes.left( pos );
		QString new_str = isotopes.right( l-pos-1 );

		vbox->addWidget( isotopeLabel( str ) );

		isotopes = new_str;
	}
	
	vbox->addWidget( isotopeLabel( isotopes ) );
}

QLabel* IsotopeWidget::isotopeLabel( const QString& str )
{
	QLabel *label = new QLabel("foo", this );

	return label;
}

#include "isotopewidget.moc"
