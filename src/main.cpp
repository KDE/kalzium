/*
 *   Copyright (C) 2003-2006 Carsten Niehaus <cniehaus@kde.org>
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

#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kapplication.h>

#include "kalzium.h"

#include <config-kalzium.h>

#define APP_VERSION "1.9.91"

static const char description[] =
    I18N_NOOP("A periodic table of the elements");

static const char version[] = APP_VERSION;

#ifdef HAVE_FACILE
extern "C" {
         void caml_startup(char**);
}
#endif

int main(int argc, char **argv)
{
#ifdef HAVE_FACILE
   caml_startup(argv);
#endif

    KAboutData about("kalzium", 0, ki18n("Kalzium"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2002-2006 Carsten Niehaus"), KLocalizedString(), "http://edu.kde.org/kalzium");
    about.addAuthor( ki18n("Carsten Niehaus"), KLocalizedString(), "cniehaus@kde.org" );
    about.addCredit( ki18n("Pino Toscano"), ki18n("Large code contributions; resident guru helping the other developers"));
    about.addCredit( ki18n("Benoit Jacob"), ki18n( "Base work on the molecular viewer, mentored Marcus during his SoC" ));
    about.addCredit( ki18n("Marcus Hanwell"), ki18n( "SoC on the molecular viewer and libavogadro porting/integration" ));
    about.addCredit( ki18n("Thomas Nagy"), ki18n("EqChem, the equation solver" ));
    about.addCredit( ki18n("Johannes Simon"), ki18n( "Improvements to the equation solver and Help button of the molecular viewer" ));
    about.addCredit( ki18n("Jarle Akselsen"), ki18n("Many beautiful element icons" ));
    about.addCredit( ki18n("Noémie Scherer"), ki18n("Many beautiful element icons, too!" ));
    about.addCredit( ki18n("Danny Allen"), ki18n("Several icons" ));
    about.addCredit( ki18n("Lee Olson"), ki18n( "Several icons in the information dialog" ));
    about.addCredit( ki18n("Anne-Marie Mahfouf"), ki18n( "A lot of small things and the documentation" ));
    about.addCredit( ki18n("Inge Wallin"), ki18n( "Code cleaning, and a lot of smaller improvements" ));
    about.addCredit( ki18n("Jörg Buchwald"), ki18n("Contributed most isotope information" ));
    about.addCredit( ki18n("Marco Martin"), ki18n("Some icons and inspiration for others" ));
    about.addCredit( ki18n("Daniel Haas"), ki18n( "The design of the information dialog" ));
    about.addCredit( ki18n("Brian Beck"), ki18n( "The orbits icon" ));
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;
    Kalzium *mainWin = 0;

    if (app.isSessionRestored())
    {
        RESTORE(Kalzium);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        /// @todo do something with the command line args here

        mainWin = new Kalzium();
        mainWin->show();

        args->clear();
    }

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}

