/*
 * Copyright (C) 2003 Carsten Niehaus <cniehaus@kde.org>
 */

#include "kalzium.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char description[] =
    I18N_NOOP("A Periodic Table of the Elements");

static const char version[] = "1.1.90";

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open." ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
    KAboutData about("kalzium", I18N_NOOP("Kalzium"), version, description,
                     KAboutData::License_GPL, "(C) 2002-2004 Carsten Niehaus", 0, "http://edu.kde.org/kalzium");
    about.addAuthor( "Carsten Niehaus", 0, "cniehaus@kde.org" );
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

