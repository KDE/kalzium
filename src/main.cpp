/*
 *   Copyright (C) 2003-2005 Carsten Niehaus <cniehaus@kde.org>
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>

#include "config.h"
#include "kalzium.h"

#define APP_VERSION "1.4.6"

static const char description[] =
    I18N_NOOP("A periodic table of the elements");

static const char version[] = APP_VERSION;

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
    KCmdLineLastOption
};

#ifdef HAVE_FACILE
extern "C" {
        void caml_startup(char**);
};
#endif

int main(int argc, char **argv)
{
#ifdef HAVE_FACILE
    caml_startup(argv);
#endif

    KAboutData about("kalzium", I18N_NOOP("Kalzium"), version, description,
                     KAboutData::License_GPL, "(C) 2002-2005 Carsten Niehaus", 0, "http://edu.kde.org/kalzium");
    about.addAuthor( "Carsten Niehaus", 0, "cniehaus@kde.org" );
    about.addCredit( "Pino Toscano", I18N_NOOP("Code contributions" ));
    about.addCredit( "Martin Pfeiffer", I18N_NOOP("Tooltip, some other small things" ));
    about.addCredit( "Thomas Nagy", I18N_NOOP("Contributed EqChem, the equation solver" ));
    about.addCredit( "Jörg Buchwald", I18N_NOOP("Contributed most isotope information" ));
    about.addCredit( "Marco Martin", I18N_NOOP("Thank you for some icons and inspiration for others" ));
    about.addCredit( "Chris Luetchford", I18N_NOOP("SVG icon"), "chris@os11.com");
    about.addCredit( "Anne-Marie Mahfouf", I18N_NOOP( "A lot of small things and the documentation" ));
    about.addCredit( "Paweł Budzyński", I18N_NOOP( "The pictures of the elements iodine and bromine" ));
    about.addCredit( "M. Stegemann", I18N_NOOP( "Almost all pictures of the elements" ));
    about.addCredit( "Daniel Haas", I18N_NOOP( "The design of the information dialog" ));
    about.addCredit( "Brian Beck", I18N_NOOP( "The orbits-icon" ));
    about.addCredit( "Lee Olson", I18N_NOOP( "Several icons in the information dialog" ));
    about.addCredit( "Inge Wallin", I18N_NOOP( "Code cleaning, the molecule parser and a lot of smaller improvements" ));
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;
    Kalzium *mainWin = 0;

    if (app.isRestored())
    {
        RESTORE(Kalzium);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        /// @todo do something with the command line args here

        mainWin = new Kalzium();
        app.setMainWidget( mainWin );
        mainWin->show();

        args->clear();
    }

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}

