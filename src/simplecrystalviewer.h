#ifndef SIMPLECRYSTALVIEWER_H
#define SIMPLECRYSTALVIEWER_H
/***************************************************************************
 *   Copyright (C) 2005 by Pino Toscano                                    *
 *   toscano.pino@tiscali.it                                               *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ***************************************************************************/

#include <kdialogbase.h>

class AnimatedMovieWidget;

class SimpleCrystalViewer : public KDialogBase
{
	Q_OBJECT
	public:
		SimpleCrystalViewer( QWidget* parent );
		~SimpleCrystalViewer(){};

	private slots:
		void slotCrystalChanged( const QString& );

	private:
		QString m_basePath;
		AnimatedMovieWidget *m_movie;

};


#endif // SIMPLECRYSTALVIEWER_H
