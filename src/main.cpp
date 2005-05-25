/*
 * Copyright (C) 2003 Carsten Niehaus <cniehaus@kde.org>
 */

#include "kalzium.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>

static const char description[] =
    I18N_NOOP("A periodic table of the elements");

static const char version[] = "1.4pre1";

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
    KAboutData about("kalzium", I18N_NOOP("Kalzium"), version, description,
                     KAboutData::License_GPL, "(C) 2002-2005 Carsten Niehaus", 0, "http://edu.kde.org/kalzium");
    about.addAuthor( "Carsten Niehaus", 0, "cniehaus@kde.org" );
    about.addCredit( "Pino Toscano", I18N_NOOP("Code contributions" ));
    about.addCredit( "Marco Martin", I18N_NOOP("Thank you for some icons and inspiration for others" ));
    about.addCredit( "Chris Luetchford", I18N_NOOP("svg icon"), "chris@os11.com");
    about.addCredit( "Anne-Marie Mahfouf", I18N_NOOP( "A lot of small things and the documentation" ));
    about.addCredit( "Paweł Budzyński", I18N_NOOP( "The pictures of the elements iodine and bromine" ));
    about.addCredit( "M. Stegemann", I18N_NOOP( "Almost all pictures of the elements" ));
    about.addCredit( "Brian Beck", I18N_NOOP( "The orbits-icon" ));
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

