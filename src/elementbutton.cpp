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
#include "pse.h"

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

	setMaximumSize( 50, 50 );
	setFrameStyle(  QFrame::Box );
	setLineWidth(  2 );
	setMidLineWidth(  3 );
	setFrameShadow(  QFrame::Sunken );
	
	timer = new QTimer( this );
}

void ElementButton::mouseReleaseEvent( QMouseEvent *mouse )
{
	if ( ( ( PSE* )parentWidget() )->molcalcMode() ){
		emit ButtonClicked( e->number() );
		return;
	}
	
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
    h = w = 50;
	
	int h_small = 15; //the size for the small units like elementnumber
	
    QPainter p;
    p.begin( this );
	
	QString text;
	QFont symbol_font = p.font();
	symbol_font.setPointSize( 18 );
	QFont f = p.font();
	f.setPointSize( 9 );
		
	p.setFont( f );

	//top left
	text = QString::number( e->number() );
	p.drawText( 0,0,h_small,h_small,Qt::AlignCenter, text );
//	p.drawText( 0,h_small+10,h_small,h_small,Qt::AlignCenter, text );

	//top right
	text = QString::number( e->strippedWeight( e->weight( ) ) );
	p.drawText( w/2,0,w/2,h_small,Qt::AlignCenter, text );
	
	//bottom right
	text = QString::number( e->strippedWeight( e->density() ) );
	p.drawText( w/2,h-h_small,w/2,h_small,Qt::AlignCenter, text );
	
	//bottom left
	text = QString::number( e->strippedWeight( e->electroneg() ) );
	p.drawText( 0, h-h_small-h_small,h_small,h_small,Qt::AlignCenter, text );

	p.setFont( symbol_font );
	p.drawText( w/3,w/4,w/2,w/2,Qt::AlignLeft, e->symbol() );
	
	p.drawRect( 0, 0, w, h );
    p.end(); 
}

void ElementButton::enterEvent( QEvent* )
{
	connect(  timer, SIGNAL( timeout() ), this, SLOT( timerDone() ) );
	timer->start(  2000, TRUE ); // 2 seconds single-shot timer
}

void ElementButton::leaveEvent( QEvent* )
{
	timer->stop();
}

void ElementButton::timerDone()
{
	    emit num( e->number() );
}

#include "elementbutton.moc"

