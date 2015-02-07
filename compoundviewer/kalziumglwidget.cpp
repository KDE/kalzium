/***************************************************************************
 *  Copyright (C) 2006 by Carsten Niehaus <cniehaus@kde.org>
 *  Copyright (C) 2007-2008 by Marcus D. Hanwell <marcus@cryos.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kalziumglwidget.h"

#include <QSettings>

#include <avogadro/primitive.h>
#include <avogadro/pluginmanager.h>
#include <avogadro/toolgroup.h>
#include <avogadro/engine.h>
#include <QtOpenGL/QGLWidget>
#include <QWidget>
#include "openbabel2wrapper.h"

#include <config-kalzium.h>

KalziumGLWidget::KalziumGLWidget(QWidget *parent) : Avogadro::GLWidget(parent),
  m_lastEngine1(0), m_lastEngine2(0)
{
    // work around a bug in OpenBabel: the chemical data files parsing
    // is dependent on the LC_NUMERIC locale.
    m_lc_numeric = QByteArray(setlocale(LC_NUMERIC, 0));
    setlocale(LC_NUMERIC, "C");

    // Prevent What's this from intercepting right mouse clicks
    setContextMenuPolicy(Qt::PreventContextMenu);
    // Load the tools and set navigate as the default
    // first set the Avogadro plugin directory,
    // avoiding overwriting an already set envvar
    static bool s_pluginDirSet = false;
    if (!s_pluginDirSet) {
        if (qgetenv("AVOGADRO_PLUGINS").isEmpty()) {
            qputenv("AVOGADRO_PLUGINS", AVOGADRO_PLUGIN_DIR);
        }
        s_pluginDirSet = true;
    }
    Avogadro::PluginManager *manager = Avogadro::PluginManager::instance();
    manager->loadFactories();
    Avogadro::ToolGroup* tools = new Avogadro::ToolGroup(this);
    tools->append(manager->tools(this));
    tools->setActiveTool("Navigate");
    setToolGroup(tools);
    // Set the default engine to be active
    loadDefaultEngines();

    // Set the default quality level to high
    setQuality(2);

    setMolecule(new Avogadro::Molecule(this));
    update();
}

KalziumGLWidget::~KalziumGLWidget()
{
    //restore the LC_NUMERIC locale.
    setlocale(LC_NUMERIC, m_lc_numeric.constData());
}

bool KalziumGLWidget::openFile(const QString &file)
{
    Avogadro::Molecule* mol = OpenBabel2Wrapper::readMolecule(file);
    if (!mol) {
        return false;
    }
    Avogadro::Molecule* oldmol = molecule();
    if (oldmol) {
        oldmol->deleteLater();
    }
    setMolecule(mol);
    update();
    return true;
}

void KalziumGLWidget::setStyle(int style)
{
    foreach (Avogadro::Engine *engine, engines()) {
        if ((m_lastEngine1 == 0 && engine->identifier() == "Ball and Stick")
            || (m_lastEngine1 == 1 && engine->identifier() == "Stick")
            || (m_lastEngine1 == 2 && engine->identifier() == "Van der Waals Spheres")
            || (m_lastEngine1 == 3 && engine->identifier() == "Wireframe")) {
            engine->setEnabled(false);
        }
        if ((style == 0 && engine->identifier() == "Ball and Stick")
            || (style == 1 && engine->identifier() == "Stick")
            || (style == 2 && engine->identifier() == "Van der Waals Spheres")
            || (style == 3 && engine->identifier() == "Wireframe")) {
            engine->setEnabled(true);
        }
    }
    m_lastEngine1 = style;
    update();
}

void KalziumGLWidget::setStyle2(int style)
{
    foreach (Avogadro::Engine *engine, engines()) {
        if ((m_lastEngine2 == 1 && engine->identifier() == "Ribbon")
            || (m_lastEngine2 == 2 && engine->identifier() == "Ring")
            || (m_lastEngine2 == 3 && engine->identifier() == "Orbitals")) {
            engine->setEnabled(false);
        }
        if ((style == 1 && engine->identifier() == "Ribbon")
            || (style == 2 && engine->identifier() == "Ring")
            || (style == 3 && engine->identifier() == "Orbitals")) {
            engine->setEnabled(true);
        }
    }
    m_lastEngine2 = style;
    update();
}

void KalziumGLWidget::setLabels(int style)
{
    // Use the QSettings framework to configure the label engine
    foreach (Avogadro::Engine *engine, engines()) {
        if (engine->name() == "Label") {
            QSettings settings;
            int atomType = 0;
            int bondType = 0;
            bool enabled = false;
            // We need to use 
            switch(style) {
            case 0: // Display no labels
                enabled = false;
                break;
            case 1: // Display the atom symbol
                enabled = true;
                atomType = 3;
                break;
            case 2: // Display the atom name
                enabled = true;
                atomType = 2;
                break;
            default:
                engine->setEnabled(false);
            }
            settings.setValue("atomLabel", atomType);
            settings.setValue("bondLabel", bondType);
            settings.setValue("enabled", enabled);
            engine->readSettings(settings);
        }
    }
}

void KalziumGLWidget::setQuality(int quality)
{
    // Set the global quality of the GLWidget, 0=min, 2=mid, 4=max
    int q = 0;
    if (quality == 1) {
        q = 2;
    } else if (quality == 2) {
        q = 4;
    }
    GLWidget::setQuality(q);
    invalidateDLs();
    GLWidget::update();
}
//#include "kalziumglwidget.moc"
