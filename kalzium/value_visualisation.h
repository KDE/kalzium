/***************************************************************************
                          ValueVisualisation.h  -  description
                             -------------------
    begin                : Fr March 22 2002
    copyright            : (C) 2002 by Carsten Niehaus
    email                : cniehaus@gmx.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VALUEVISUALISATION_H
#define VALUEVISUALISATION_H

//KDE-Includes
#include <kconfig.h>
#include <kdialog.h>

//Qt-Includes
#include <qwidget.h>


class Kalzium;

class ValueVisualisation : public KDialog  {
    Q_OBJECT

    public: 
	/** construtor */
	ValueVisualisation(QWidget *parent, const char *name, Kalzium *kalzium_tmp);
	Kalzium *kalzium;
    private:
	KConfig *main_config;
    public slots:
};

#endif
