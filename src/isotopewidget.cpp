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
	
	QWidget *widget = new QWidget( this );
	QHBoxLayout *h = new QHBoxLayout( widget );
	QLabel *l1 = new QLabel( "<b>Weight</b>" , widget );
	QLabel *l2 = new QLabel( i18n( "<b>Neutrons</b>" ) , widget );
	QLabel *l3 = new QLabel( "<b>Percentage</b>" , widget );
	h->addWidget( l1 );
	h->addWidget( l2 );
	h->addWidget( l3 );

	vbox->addWidget( widget );
	
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

QWidget* IsotopeWidget::isotopeLabel( const QString& str )
{
	QWidget *w = new QWidget( this );
	
	QHBoxLayout *hbox = new QHBoxLayout( w );
	
	//number of neutrons of the isotope
	int pos = str.find( ":" );
	QString neutrons_str = str.left( pos );
	QString tmp = str.right( str.length()-pos-1 );

	//Weight of the isotope
	pos = tmp.find( ":" );
	QString weight_str = tmp.left( pos );
	
	//Percentage of the isotope
	tmp = tmp.right( tmp.length()-pos-1 );
	
	QLabel *neutrons = new QLabel( w );
	QLabel *weight = new QLabel( w );
	QLabel *percentage = new QLabel( w );

	weight_str.append( i18n( " u" ) );
	neutrons->setText( weight_str );
	weight->setText( neutrons_str );
	percentage->setText( tmp );

	hbox->addWidget( neutrons );
	hbox->addWidget( weight );
	hbox->addWidget( percentage );

	return w;
}

#include "isotopewidget.moc"
