/***************************************************************************
 *   Copyright (C) 2004 by Thomas Nagy                                     *
 *   tnagy2^8@yahoo.fr                                                     *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "eqchem.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char description[] =
    I18N_NOOP("A chemical equation solver");

static const char version[] = "0.4.1";

static KCmdLineOptions options[] =
{
    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
    KCmdLineLastOption
};


extern "C" {
	void caml_startup(char**);
};


int main(int argc, char **argv)
{

    caml_startup(argv);
	
    KAboutData about("eqchem", I18N_NOOP("eqchem"), version, description,
                     KAboutData::License_GPL, "(C) 2004 Thomas Nagy", 0, 0, "tnagy2^8@yahoo.fr");
    about.addAuthor( "Thomas Nagy", 0, "tnagy2^8@yahoo.fr" );
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    // see if we are starting with session management
    if (app.isRestored())
    {
        RESTORE(eqchem);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
        {
            eqchem *widget = new eqchem();
            widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                eqchem *widget = new eqchem();
                widget->show();
            }
        }
        args->clear();
    }

    return app.exec();
}

