/***************************************************************************
 *   Copyright (C) 2003-2008 Carsten Niehaus <cniehaus@kde.org>            *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <KAboutData>
#include <KLocale>
#include <KLocalizedString>
#include <config-kalzium.h>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "kalzium.h"

#define APP_VERSION "2.5.0"

static const char description[] = I18N_NOOP("A periodic table of the elements");

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

    KLocalizedString::setApplicationDomain("kalzium");

    KAboutData about("kalzium",
                      ki18n("Kalzium").toString(),
                      version,
                      ki18n(description).toString(),
                      KAboutLicense::GPL,
                      ki18n("(C) 2002-2016 Carsten Niehaus & the KDE Edu Developers").toString(),
                      QString(),
                      "http://edu.kde.org/kalzium");

    about.addAuthor(ki18n("Carsten Niehaus").toString(),
                    QString(),
                    "cniehaus@kde.org");


    about.addCredit(ki18n("Pino Toscano").toString(),
                    ki18n("Large code contributions; resident guru helping the other developers").toString());

    about.addCredit(ki18n("Benoit Jacob").toString(),
                    ki18n("Base work on the molecular viewer, mentored Marcus during his SoC").toString());

    about.addCredit(ki18n("Marcus Hanwell").toString(),
                    ki18n("SoC on the molecular viewer and libavogadro porting/integration").toString());

    about.addCredit(ki18n("Kashyap R Puranik").toString(),
                    ki18n("SoC on the calculator widget and a few smaller improvements").toString());

    about.addCredit(ki18n("Thomas Nagy").toString(),
                    ki18n("EqChem, the equation solver").toString());

    about.addCredit(ki18n("Inge Wallin").toString(),
                    ki18n("Code cleaning, parser for the molecule weight calculator, and a lot of smaller improvements").toString());

    about.addCredit(ki18n("Anne-Marie Mahfouf").toString(),
                    ki18n("A lot of small things and the documentation").toString());

    about.addCredit(ki18n("Johannes Simon").toString(),
                    ki18n("Code and documentation contributions to the equation solver and molecular viewer").toString());

    about.addCredit(ki18n("Jarle Akselsen").toString(),
                    ki18n("Many beautiful element icons").toString());

    about.addCredit(ki18n("Noémie Scherer").toString(),
                    ki18n("Many beautiful element icons, too!").toString());

    about.addCredit(ki18n("Danny Allen").toString(),
                    ki18n("Several icons").toString());

    about.addCredit(ki18n("Lee Olson").toString(),
                    ki18n("Several icons in the information dialog").toString());

    about.addCredit(ki18n("Jörg Buchwald").toString(),
                    ki18n("Contributed most isotope information").toString());

    about.addCredit(ki18n("Marco Martin").toString(),
                    ki18n("Some icons and inspiration for others").toString());

    about.addCredit(ki18n("Daniel Haas").toString(),
                    ki18n("The design of the information dialog").toString());

    about.addCredit(ki18n("Brian Beck").toString(),
                    ki18n("The orbits icon").toString());

    about.addCredit(ki18n("Paulo Cattai").toString(),
                    ki18n("New interface design and usability improvements").toString());

    about.addCredit(ki18n("Danilo Balzaque").toString(),
                    ki18n("New interface design and usability improvements").toString());

    about.addCredit(ki18n("Roberto Cunha").toString(),
                    ki18n("New interface design and usability improvements").toString());

    about.addCredit(ki18n("Tadeu Araujo").toString(),
                    ki18n("New interface design and usability improvements").toString());

    about.addCredit(ki18n("Tiago Porangaba").toString(),
                    ki18n("New interface design and usability improvements").toString());

    about.addCredit(ki18n("Etienne Rebetez").toString(),
                    ki18n("Adding new sizable Periodic System").toString());


    QApplication::setApplicationName("kalzium");
    QApplication::setApplicationVersion(version);
    QApplication::setOrganizationDomain("kde.org");
    QApplication app(argc, argv);

    KAboutData::setApplicationData(about);

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();

    parser.addPositionalArgument(ki18n("[molecule]").toString(), ki18n("Open the given molecule file").toString());

    QCommandLineOption moleculeOption(ki18n("molecule").toString(), ki18n("Open the given molecule file").toString());
    parser.addOption(moleculeOption);

    about.setupCommandLine(&parser);
    parser.process(app);
    about.processCommandLine(&parser);

    #if defined(HAVE_OPENBABEL2) && defined(HAVE_EIGEN) && defined(HAVE_AVOGADRO)
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("m") << QLatin1String("molecule"), i18n("Open the given molecule file"), QLatin1String("file")));
    #endif

    Kalzium *mainWin = 0;

    if (app.isSessionRestored()) {
        RESTORE(Kalzium);
    } else {
        // no session.. just start up normally

        /// @todo do something with the command line args here

        mainWin = new Kalzium();
        mainWin->show();

        const QStringList molecules = parser.values("molecule");
        if (molecules.count() == 1) {
            mainWin->loadMolecule(molecules[0]);
        } else if (molecules.count() > 1) {
            QTextStream ts(stderr);
            ts << i18n("Can't open more than one molecule at a time");
        }

    }

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}
