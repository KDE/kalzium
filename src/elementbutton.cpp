	/***************************************************************************
	 *   Copyright (C) 2003 by Carsten Niehaus                                 *
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
	 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
	 ***************************************************************************/
#include "elementbutton.h"
#include "prefs.h"
#include "infodialog_small_impl.h"
#include "detailinfodlg.h"
#include <qpainter.h>
#include <qfont.h>
#include <qstring.h>
#include <qtimer.h>
#include <qlabel.h>

#include <kdebug.h>
#include <klocale.h>

ElementButton::ElementButton( Element *el, QWidget *parent, const char *name)
 : QFrame(parent, name)
{
	e = el;

	setMaximumSize( 40, 40 );
	setFrameStyle(  QFrame::Box );
	setLineWidth(  2 );
	setMidLineWidth(  3 );
	setFrameShadow(  QFrame::Sunken );
}

void ElementButton::mouseReleaseEvent( QMouseEvent *mouse )
{
	int small = Prefs::lMBbeh();
	if ( small == 1 )
	{
		if ( mouse->button() == LeftButton )
		{
			infoDlgSmallImpl *smallDlg = new infoDlgSmallImpl( e, this , "smallDlg" );
			smallDlg->show();
		}else
		{
			DetailedInfoDlg *detailedDlg = new DetailedInfoDlg( e, this , "detailedDlg" );
			detailedDlg->show();
		}	
	}
	if ( small == 0 )
	{
		if ( mouse->button() == LeftButton )
		{
			DetailedInfoDlg *detailedDlg = new DetailedInfoDlg( e, this , "detailedDlg" );
			detailedDlg->show();
		}else
		{
			infoDlgSmallImpl *smallDlg = new infoDlgSmallImpl( e, this , "smallDlg" );
			smallDlg->show();
		}	
	}
}

ElementButton::~ElementButton()
{
}

/*!
    \fn ElementButton::paintEvent( QPaintEvent* )
 */
void ElementButton::paintEvent( QPaintEvent* )
{
    int h, w;
    h = w = 60;
    QPainter p;
    p.begin( this );
	
	QString text;
	QFont symbol_font = p.font();
	symbol_font.setPointSize( 18 );
	QFont f = p.font();
	f.setPointSize( 8 );

	switch ( Prefs::pselook() )
	{
		case 0:
			p.setFont( symbol_font );
    		p.drawText( h/6, h/2, e->symbol() );
			p.setFont( f );
			text = QString::number( e->strippedWeight( e->weight( ) ) );
			text.append( i18n( " u" ) );
			p.drawText( 2, h/6, text );
			break;
		case 1:
			p.setFont( symbol_font );
    		p.drawText( h/6, h/2, e->symbol() );
			p.setFont( f );
			text = QString::number( e->number() );
			p.drawText( 2, h/6, text );
			break;
		case 2:
			p.setFont( symbol_font );
    		p.drawText( h/6, h/2, e->symbol() );
			p.setFont( f );
			text = QString::number( e->density() );
			if ( text != "-1" )
				p.drawText( 2, h/6, text );
			break;

	}
	
	p.drawRect( 0, 0, w, h );
    p.end(); 
}

void ElementButton::enterEvent( QEvent* )
{
	QTimer *timer = new QTimer( this );
	connect(  timer, SIGNAL( timeout() ), this, SLOT( timerDone() ) );
	timer->start(  2000, TRUE ); // 2 seconds single-shot timer
}

void ElementButton::timerDone()
{
	    emit num( e->number() );
}

#include "elementbutton.moc"

