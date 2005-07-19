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

#ifndef _EQCHEM_H_
#define _EQCHEM_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kapplication.h>
#include <kmainwindow.h>

#include "eqchemview.h"

class KPrinter;
class KToggleAction;
class KURL;

/**
 * This class serves as the main window for eqchem.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Thomas Nagy <tnagy2^8@yahoo.fr>
 * @version 0.1
 */
class eqchem : public KMainWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    eqchem();

    /**
     * Default Destructor
     */
    virtual ~eqchem();

private slots:
    void fileNew();
    //void optionsShowToolbar();
    //void optionsShowStatusbar();
    //void optionsConfigureKeys();
    //void optionsConfigureToolbars();
    void optionsPreferences();
    //void newToolbarConfig();

    void changeStatusbar(const QString& text);
    void changeCaption(const QString& text);

private:
    void setupAccel();
    void setupActions();

private:
    eqchemView *m_view;

    KPrinter   *m_printer;
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif // _EQCHEM_H_

