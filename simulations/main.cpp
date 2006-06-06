/***************************************************************************
 *   Copyright (C) 2006      by Carsten Niehaus,    cniehaus@kde.org       *
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

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include "maindialog_impl.h"

#include "config.h"

#include "kalzium.h"

#define APP_VERSION "0.1"

static const char description[] =
    I18N_NOOP("Simulations" );

static const char version[] = APP_VERSION;

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
    KAboutData about("kalzium", I18N_NOOP("Simulations"), version, description,
                     KAboutData::License_GPL, "(C) 2005-2006 Carsten Niehaus", 0, "http://edu.kde.org/kalzium");
    about.addAuthor( "Carsten Niehaus", 0, "cniehaus@kde.org" );
    about.addCredit( "Pino Toscano", I18N_NOOP("Code contributions" ));
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;
    MainDialog_Impl *mainWin = 0;

#warning MainDialog_Impl has to be a KMainWindow if you want to call RESTORE on it
/*    
    if (app.isSessionRestored())
    {
        RESTORE(MainDialog_Impl);
    }
    else
    {*/
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        /// @todo do something with the command line args here

        mainWin = new MainDialog_Impl();
        app.setMainWidget( mainWin );
        mainWin->show();

        args->clear();
//    }

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}


