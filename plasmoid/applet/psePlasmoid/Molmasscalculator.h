/***********************************************************************************
* Mass calculator: Plasmoid to calculate mass of a Molecule.
* Copyright (C) 2009, 2010 Etienne Rebetez, etienne.rebetez@oberwallis.ch
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
***********************************************************************************/

// Here we avoid loading the header multiple times
#ifndef periodicTable_HEADER
#define periodicTable_HEADER

#include <QGraphicsGridLayout>
#include <QColor>

// We need the Plasma Applet headers
#include <plasma/widgets/label.h>
#include <plasma/widgets/lineedit.h>
#include <plasma/widgets/iconwidget.h>
#include <plasma/popupapplet.h>

#include "ui_Molmassconfig.h"

#include "Periodictable.h"

// Define our plasma Applet
class Molmasscalculator : public Plasma::PopupApplet
{
    Q_OBJECT
public:
    // Basic Create/Destroy
    Molmasscalculator ( QObject *parent, const QVariantList &args );
    ~Molmasscalculator();

    /// Returns the GraphicsWidget used by the plasmoid
    QGraphicsWidget *graphicsWidget ();

    /// loads the settings.
    void init();

protected:
    void createConfigurationInterface (KConfigDialog* parent);
    void configChanged();

public slots:
    /// Appens the given String to the lineedit text and let it parse.
    void appendElement ( QString ElementSymbol );

private slots:
    void ParseMolecule( QString input);
    void ParseMolecule();
    void toggleTable();

private:
    void managePeriodSystem();
    void newCalculatedMass();
    void saveConfig();
    void loadConfig();

    // Configuration
    bool m_showPeriodicTable;
    bool m_copyToClipboard;
    int  m_tableType;

    // Widgets
    QGraphicsWidget *m_widget;
    PeriodicGrid *m_PeriodWidget;

    Plasma::LineEdit *m_lineedit;
    Plasma::Label *m_MassLabel;
    Plasma::IconWidget *m_switchButton;

    Plasma::DataEngine::Data m_molecule;

    Ui::periodicConfig m_ui;
};

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET ( molmassCalculator, Molmasscalculator )
#endif
