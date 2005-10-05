#ifndef ANIMATEDMOVIEWIDGET_H
#define ANIMATEDMOVIEWIDGET_H
/***************************************************************************
 *   Copyright (C) 2005 by Pino Toscano     toscano.pino@tiscali.it        *
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

#include <qstringlist.h>
#include <qtimer.h>
#include <qwidget.h>

class QLabel;
class KPushButton;

/**
 * This widget serves as a "mini-viewer" for animations.
 *
 * @author Pino Toscano
 */
class AnimatedMovieWidget : public QWidget
{
	Q_OBJECT

	public:
		/**
		 * Constructor. Set the picture path to '.'.
		 *
		 * @param parent parent widget
		 * @param name name of this widget
		 */
		AnimatedMovieWidget( QWidget *parent = 0, const char *name = 0 );
		/**
		 * Constructor.
		 *
		 * @param picturePath the path of the images
		 * @param parent parent widget
		 * @param name name of this widget
		 */
		AnimatedMovieWidget( const QString& picturePath, QWidget *parent = 0, const char *name = 0 );

		/**
		 * Destructor.
		 */
		~AnimatedMovieWidget();

		/**
		 * Set the path of the images to @p path and reload the
		 * animation for that directory
		 */
		void setPicturePath( const QString& path );

	protected:
		/**
		 * Init method.
		 */
		void init();

		void reloadImages();

	private slots:
		void prevFrame();
		void start();
		void pause();
		void nextFrame();
		void animate();

	private:
		void loadImage( int id );

		QTimer m_timer;
		QString m_picpath;
		QStringList m_pics;
		int m_currentPictureId;

		QLabel *m_pix;
		KPushButton *m_start;
		KPushButton *m_pause;
		KPushButton *m_prev;
		KPushButton *m_next;
};

#endif // ANIMATEDMOVIEWIDGET_H
