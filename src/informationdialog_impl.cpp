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
#include "informationdialog_impl.h"

#include <qbuttongroup.h>
#include <qdom.h>
#include <qfile.h>
#include <qlabel.h>
#include <qpoint.h>
#include <qradiobutton.h>
#include <qslider.h>
#include <qtabwidget.h>
#include <qtextbrowser.h>

#include <kdebug.h>
#include <kfiledialog.h>
#include <knuminput.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include <math.h>

#include "pse.h"
#include "informationdialog.h"
#include "element.h"

InformationWidget::InformationWidget( PSE *pse )
    : KDialogBase( Swallow, i18n( "Learn More About the Table of Elements" ), Help|Close, Close, 0L, "learn-dialog", false )
{
	m_pse = pse;
	m_infoDialog = new InformationDialog( this );
	setMainWidget( m_infoDialog );

	m_infoDialog->Number2->setMinValue( m_infoDialog->temp_slider->minValue() );
	m_infoDialog->Number2->setMaxValue( m_infoDialog->temp_slider->maxValue() );

	connect( m_infoDialog->temp_slider, SIGNAL( sliderMoved(int) ), m_infoDialog->Number2, SLOT( setValue(int) ) );
	connect( m_infoDialog->temp_slider, SIGNAL( valueChanged(int) ), this, SLOT( slotTemp(int) ) );
	connect( m_infoDialog->temp_slider, SIGNAL( valueChanged(int) ), m_infoDialog->Number2, SLOT( setValue(int) ) );
	connect( m_infoDialog->Number2, SIGNAL( valueChanged(int) ), m_infoDialog->temp_slider, SLOT( setValue(int) ) );

	resize( 550, 400 );
}

void InformationWidget::slotClose()
{
	m_pse->setFullDraw();
	m_pse->activateSOMMode( false );
	emit closed();
	accept();
}

void InformationWidget::slotTemp( int temp )
{
	m_pse->setTemperature( temp );

	QString appBaseDir = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
	appBaseDir.append("kalzium/data/");
	appBaseDir.append("bg.jpg");
	QString htmlcode = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body background=\"" ;
	htmlcode += appBaseDir + "\">";

	static const int threshold = 25;

	EList kdo = m_pse->data()->ElementList;
	EList::ConstIterator it = kdo.begin();
	const EList::ConstIterator itEnd = kdo.end();

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

	m_infoDialog->m_explanation_3->setText( htmlcode );
}

void InformationWidget::showSOM()
{
	m_infoDialog->tabWidget->setCurrentPage( 0 );
}

#include "informationdialog_impl.moc"
