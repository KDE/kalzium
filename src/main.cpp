/***************************************************************************
                          main.cpp  -
                             -------------------
    begin                : Die Dez  4 17:59:34 CET 2001
    copyright            : (C) 2001, 2002, 2003 by Carsten Niehaus
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


#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>

#include "kalzium.h"

static const char description[] =
	I18N_NOOP("Kalzium is a chemistry application designed\n"
	 "to provide information about the elements in\n"
	 "the periodic table of elements and do basic calculations.\n");

static KCmdLineOptions options[] =
{
  KCmdLineLastOption
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{
    KAboutData aboutData( "kalzium", I18N_NOOP("Kalzium"),
           KALZIUM_VERSION , description, KAboutData::License_GPL,
            "(c) 2001, 2002, 2003, Carsten Niehaus", 0, 0, "cniehaus@kde.org");
    aboutData.addAuthor("Carsten Niehaus",
            I18N_NOOP("Maintainer, Author"), "cniehaus@kde.org");
    aboutData.addCredit("Robert Gogolok",
            I18N_NOOP("Coding"), "robertgogolok@gmx.de");
    aboutData.addCredit("Anne-Marie Mahfouf",
            I18N_NOOP("Supporting"), "annma@kde.org");
    aboutData.addCredit("Dirk Moeller",
            I18N_NOOP("Helping me out"), "mail@dirk-moeller.de");
    aboutData.addCredit("Jonathan Drews",
            I18N_NOOP("Helping out, proof reading, tips"), "j.e.drews@worldnet.att.net");
    aboutData.addCredit("Matthias Messmer",
            I18N_NOOP("Additional coding and tweaking the ui's"), "bmlmessmer@web.de");
    aboutData.addCredit("Tomislav Gountchev",
            I18N_NOOP("KMol-code"), "tomi@idiom.com");
    aboutData.addCredit("Dipl.-Ing. M. Stegemann", I18N_NOOP("Pictures of the elements"), "");
    KCmdLineArgs::init( argc, argv, &aboutData );
    KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

    KApplication a;
    Kalzium *kalzium = new Kalzium();
    a.setMainWidget(kalzium);
    kalzium->show();

  return a.exec();
}
