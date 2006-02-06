/***************************************************************************
 *   Copyright (C) 2005      by Pino Toscano,    toscano.pino@tiscali.it   *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include "animatedmoviewidget.h"

#include <QDir>
#include <QLabel>
#include <QLayout>
#include <QPixmap>
#include <QRegExp>
#include <QSizePolicy>

#include <kglobal.h>
#include <kiconloader.h>
#include <kpushbutton.h>

AnimatedMovieWidget::AnimatedMovieWidget( QWidget *parent )
    : QWidget( parent )
{
	m_picpath = ".";

	init();
}

AnimatedMovieWidget::AnimatedMovieWidget( const QString& picturePath, QWidget *parent )
    : QWidget( parent )
{
	m_picpath = picturePath.isEmpty() ? "." : picturePath;

	init();
}

AnimatedMovieWidget::~AnimatedMovieWidget()
{
	m_timer.stop();
}

void AnimatedMovieWidget::init()
{
	KIconLoader *il = KGlobal::iconLoader();

	QVBoxLayout *mainlay = new QVBoxLayout( this );

	m_pix = new QLabel( this );
	m_pix->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	m_pix->setLineWidth( 1 );
	m_pix->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
	mainlay->addWidget( m_pix );
	
	QHBoxLayout *hlay = new QHBoxLayout();
	mainlay->addLayout( hlay );
	hlay->setMargin( 0 );
	hlay->setSpacing( 5 );

	m_prev = new KPushButton( this );
	m_prev->setIcon( il->loadIconSet( "player_start", KIcon::NoGroup, 16 ) );
	hlay->addWidget( m_prev );

	m_start = new KPushButton( this );
	m_start->setIcon( il->loadIconSet( "player_play", KIcon::NoGroup, 16 ) );
	hlay->addWidget( m_start );

	m_pause = new KPushButton( this );
	m_pause->setIcon( il->loadIconSet( "player_pause", KIcon::NoGroup, 16 ) );
	m_pause->setEnabled( false );
	hlay->addWidget( m_pause );

	m_next = new KPushButton( this );
	m_next->setIcon( il->loadIconSet( "player_end", KIcon::NoGroup, 16 ) );
	hlay->addWidget( m_next );
	hlay->addItem( new QSpacerItem( 5, 5, QSizePolicy::Expanding, QSizePolicy::Fixed ) );

	setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

	connect( m_prev, SIGNAL( clicked() ), this, SLOT( prevFrame() ) );
	connect( m_start, SIGNAL( clicked() ), this, SLOT( start() ) );
	connect( m_pause, SIGNAL( clicked() ), this, SLOT( pause() ) );
	connect( m_next, SIGNAL( clicked() ), this, SLOT( nextFrame() ) );
	connect( &m_timer, SIGNAL( timeout() ), this, SLOT( animate() ) );

	reloadImages();
}

void AnimatedMovieWidget::setPicturePath( const QString& path )
{
	//kDebug() << "AnimatedMovieWidget::setPicturePath()" << endl;
	if ( path.isEmpty() )
		return;

	// first, stop the current animation (if animating)
	pause();

	m_picpath = path;

	reloadImages();
}

void AnimatedMovieWidget::reloadImages()
{
	m_currentPictureId = 0;

	m_pics.clear();
	QDir dir( m_picpath );
	QStringList tmplist = dir.entryList();
	QRegExp imgre( "[0-9]+\\.([Jj][Pp][Ee]?[Gg]|[Pp][Nn][Gg])" );
	int i;
	for ( i = 0; i < tmplist.size(); i++ )
	{
		if ( imgre.indexIn( tmplist.at( i ) ) > -1 )
			m_pics << tmplist.at( i );
	}
	if ( m_pics.isEmpty() )
		return;
	
	uint maxlength = 0;
	for ( i = 0; i < m_pics.size(); i++ )
	{
		if ( m_pics.at( i ).length() > static_cast<int>( maxlength ) )
			maxlength = static_cast<uint>( m_pics.at( i ).length() );
	}
	for ( i = 0; i < m_pics.size(); i++ )
	{
		m_pics[i] = m_pics.at( i ).rightJustified( maxlength, '0' );
	}
	if ( m_pics.size() > 0 )
	{
		// ok, we have a valid animation inside the folder
		m_pics.sort();
		loadImage( 0 );
	}
	else
	{
		// no valid animation, cleaning the label
		m_pix->setPixmap( QPixmap() );
		m_pix->resize( 100, 100 );
	}
	resize( minimumSizeHint() );
}

void AnimatedMovieWidget::loadImage( int id )
{
	if ( ( id < 0 ) || ( id >= m_pics.size() ) )
		return;

	QPixmap tmp( m_picpath + "/" + m_pics.at( id ) );

	m_pix->setPixmap( tmp );
	m_pix->resize( tmp.size() );
}

void AnimatedMovieWidget::prevFrame()
{
	if ( m_timer.isActive() )
		return;

	if ( ( --m_currentPictureId ) < 0 )
		m_currentPictureId = m_pics.size() - 1;
	loadImage( m_currentPictureId );
}

void AnimatedMovieWidget::start()
{
	m_prev->setEnabled( false );
	m_start->setEnabled( false );
	m_next->setEnabled( false );
	m_pause->setEnabled( true );
	m_timer.start( 300 );
}

void AnimatedMovieWidget::pause()
{
	if ( !m_timer.isActive() )
		return;

	m_timer.stop();
	m_prev->setEnabled( true );
	m_start->setEnabled( true );
	m_next->setEnabled( true );
	m_pause->setEnabled( false );
}

void AnimatedMovieWidget::nextFrame()
{
	if ( m_timer.isActive() )
		return;

	animate();
}

void AnimatedMovieWidget::animate()
{
	if ( ( ++m_currentPictureId ) >= m_pics.size() )
		m_currentPictureId = 0;
	loadImage( m_currentPictureId );
}

#include "animatedmoviewidget.moc"
