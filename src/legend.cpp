/***************************************************************************

                          legend.cpp  -  description
                             -------------------
    begin                : Wed Oct 23 2002
    copyright            : (C) 2002 by Carsten Niehaus
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

#include "legend.h"

//KDE-Includes
#include <kconfig.h>
#include <kstddirs.h>
#include <klocale.h>

//QT-Includes
#include <qdialog.h>
#include <qlayout.h>
#include <qframe.h>
#include <qwidget.h>
#include <qpushbutton.h>

KalziumLegend::KalziumLegend( QWidget *parent, const char *name) : QWidget( parent, name )
{
	main_config=KGlobal::config();

	QHBoxLayout *legend_layout = new QHBoxLayout( this );
	legend_layout->setDirection(QBoxLayout::LeftToRight);
	legend_layout->addStretch();
	one = new QPushButton( this );
	two = new QPushButton( this );
	three = new QPushButton( this );
	four = new QPushButton( this );
	five = new QPushButton( this );
	six = new QPushButton( this );
	seven = new QPushButton( this );
	eight = new QPushButton( this );
	legend_layout->addWidget( one );
	legend_layout->addWidget( two );
	legend_layout->addWidget( three );
	legend_layout->addWidget( four );
	legend_layout->addWidget( five );
	legend_layout->addWidget( six );
	legend_layout->addWidget( seven );
	legend_layout->addWidget( eight );
	legend_layout->addStretch();
}

void KalziumLegend::changeLegend(int id)
{
	main_config->setGroup("Colors");
	if (id == 0) //Acid Behaviours
	{
		one->setPaletteBackgroundColor(QColor(main_config->readColorEntry("acidic")));
		two->setPaletteBackgroundColor(QColor(main_config->readColorEntry("amphoteric")));
		three->setPaletteBackgroundColor(QColor(main_config->readColorEntry("basic")));
		four->setPaletteBackgroundColor(QColor(main_config->readColorEntry("neitherofthem")));
		five->hide();
		six->hide();
		seven->hide();
		eight->hide();
		one->setText(i18n("Acidic"));
		two->setText(i18n("Amphoteric"));
		three->setText(i18n("Basic"));
		four->setText(i18n("Neutral"));
	}
	if (id == 1) //Blocks
	{
		one->setPaletteBackgroundColor(QColor(main_config->readColorEntry("s")));
		two->setPaletteBackgroundColor(QColor(main_config->readColorEntry("p")));
		three->setPaletteBackgroundColor(QColor(main_config->readColorEntry("d")));
		four->setPaletteBackgroundColor(QColor(main_config->readColorEntry("f")));
		five->hide();
		six->hide();
		seven->hide();
		eight->hide();
		one->setText(i18n("s-Block"));
		two->setText(i18n("p-Block"));
		three->setText(i18n("d-Block"));
		four->setText(i18n("f-Block"));
	}
	if (id == 2) //Groups
	{
		one->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 1")));
		two->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 2")));
		three->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 3")));
		four->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 4")));
		five->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 5")));
		six->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 6")));
		seven->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 7")));
		eight->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 8")));
		six->show();
		seven->show();
		eight->show();
		one->setText(i18n("Group 1"));
		two->setText(i18n("Group 2"));
		three->setText(i18n("Group 3"));
		four->setText(i18n("Group 4"));
		five->setText(i18n("Group 5"));
		six->setText(i18n("Group 6"));
		seven->setText(i18n("Group 7"));
		eight->setText(i18n("Group 8"));
	}
	if (id == 3) //State of Matter
	{
		one->setPaletteBackgroundColor(QColor(main_config->readColorEntry("liquid")));
		two->setPaletteBackgroundColor(QColor(main_config->readColorEntry("solid")));
		three->setPaletteBackgroundColor(QColor(main_config->readColorEntry("vapor")));
		four->setPaletteBackgroundColor(QColor(main_config->readColorEntry("artificial")));
		five->setPaletteBackgroundColor(QColor(main_config->readColorEntry("radioactive")));
		five->show();
		six->hide();
		seven->hide();
		eight->hide();
		one->setText(i18n("Liquid"));
		two->setText(i18n("Solid"));
		three->setText(i18n("Vapor"));
		four->setText(i18n("Artificial"));
		five->setText(i18n("Radioactive"));
	}
}
