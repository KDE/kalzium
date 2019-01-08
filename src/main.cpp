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

#include <config-kalzium.h>

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include <KAboutData>
#include <KLocalizedString>

#include "kalzium.h"

#define APP_VERSION "2.5.0"

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

    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("kalzium");

    KAboutData about(QStringLiteral("kalzium"),
                      i18n("Kalzium"),
                      version,
                      i18n("A periodic table of the elements"),
                      KAboutLicense::GPL,
                      i18n("(C) 2002-2016 Carsten Niehaus & the KDE Edu Developers"),
                      QString(),
                      QStringLiteral("http://edu.kde.org/kalzium"));

    about.addAuthor(i18n("Carsten Niehaus"),
                    QString(),
                    QStringLiteral("cniehaus@kde.org"));


    about.addCredit(i18n("Pino Toscano"),
                    i18n("Large code contributions; resident guru helping the other developers"));

    about.addCredit(i18n("Benoit Jacob"),
                    i18n("Base work on the molecular viewer, mentored Marcus during his SoC"));

    about.addCredit(i18n("Marcus Hanwell"),
                    i18n("SoC on the molecular viewer and libavogadro porting/integration"));

    about.addCredit(i18n("Kashyap R Puranik"),
                    i18n("SoC on the calculator widget and a few smaller improvements"));

    about.addCredit(i18n("Thomas Nagy"),
                    i18n("EqChem, the equation solver"));

    about.addCredit(i18n("Inge Wallin"),
                    i18n("Code cleaning, parser for the molecule weight calculator, and a lot of smaller improvements"));

    about.addCredit(i18n("Anne-Marie Mahfouf"),
                    i18n("A lot of small things and the documentation"));

    about.addCredit(i18n("Johannes Simon"),
                    i18n("Code and documentation contributions to the equation solver and molecular viewer"));

    about.addCredit(i18n("Jarle Akselsen"),
                    i18n("Many beautiful element icons"));

    about.addCredit(i18n("Noémie Scherer"),
                    i18n("Many beautiful element icons, too!"));

    about.addCredit(i18n("Danny Allen"),
                    i18n("Several icons"));

    about.addCredit(i18n("Lee Olson"),
                    i18n("Several icons in the information dialog"));

    about.addCredit(i18n("Jörg Buchwald"),
                    i18n("Contributed most isotope information"));

    about.addCredit(i18n("Marco Martin"),
                    i18n("Some icons and inspiration for others"));

    about.addCredit(i18n("Daniel Haas"),
                    i18n("The design of the information dialog"));

    about.addCredit(i18n("Brian Beck"),
                    i18n("The orbits icon"));

    about.addCredit(i18n("Paulo Cattai"),
                    i18n("New interface design and usability improvements"));

    about.addCredit(i18n("Danilo Balzaque"),
                    i18n("New interface design and usability improvements"));

    about.addCredit(i18n("Roberto Cunha"),
                    i18n("New interface design and usability improvements"));

    about.addCredit(i18n("Tadeu Araujo"),
                    i18n("New interface design and usability improvements"));

    about.addCredit(i18n("Tiago Porangaba"),
                    i18n("New interface design and usability improvements"));

    about.addCredit(i18n("Etienne Rebetez"),
                    i18n("Adding new sizable Periodic System"));


    QApplication::setApplicationName(QStringLiteral("kalzium"));
    QApplication::setApplicationVersion(version);
    QApplication::setOrganizationDomain(QStringLiteral("kde.org"));

    KAboutData::setApplicationData(about);

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();

    parser.addPositionalArgument(i18n("[molecule]"), i18n("Open the given molecule file"));

    QCommandLineOption moleculeOption(i18n("molecule"), i18n("Open the given molecule file"));
    parser.addOption(moleculeOption);

    about.setupCommandLine(&parser);
    parser.process(app);
    about.processCommandLine(&parser);

    #if defined(HAVE_OPENBABEL2) && defined(HAVE_EIGEN) && defined(HAVE_AVOGADRO)
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("m") << QStringLiteral("molecule"), i18n("Open the given molecule file"), QStringLiteral("file")));
    #endif

    Kalzium *mainWin = nullptr;

    if (app.isSessionRestored()) {
        RESTORE(Kalzium);
    } else {
        // no session.. just start up normally

        /// @todo do something with the command line args here

        mainWin = new Kalzium();
        mainWin->show();

        const QStringList molecules = parser.values(QStringLiteral("molecule"));
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
