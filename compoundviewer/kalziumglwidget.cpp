/***************************************************************************
 *  Copyright (C) 2006 by Carsten Niehaus <cniehaus@kde.org>
 *  Copyright (C) 2007-2008 by Marcus D. Hanwell <marcus@cryos.org>
 *  Copyright (C) 2016 by Andreas Cord-Landwehr <cordlandwehr@kde.org>
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
#include "iowrapper.h"

#include <avogadro/qtgui/molecule.h>
#include <avogadro/qtgui/sceneplugin.h>
#include <avogadro/qtgui/toolplugin.h>
#include <avogadro/qtopengl/glwidget.h>
#include <avogadro/qtplugins/pluginmanager.h>
#include <avogadro/rendering/primitive.h>

#include <QDebug>
#include <QSettings>
#include <QWidget>

#include <config-kalzium.h>

KalziumGLWidget::KalziumGLWidget(QWidget *parent)
    : Avogadro::QtOpenGL::GLWidget(parent)
{
    // work around a bug in OpenBabel: the chemical data files parsing
    // is dependent on the LC_NUMERIC locale.
    m_lc_numeric = QByteArray(setlocale(LC_NUMERIC, nullptr));
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
    Avogadro::QtPlugins::PluginManager *manager = Avogadro::QtPlugins::PluginManager::instance();
    manager->load();

    // load render engines
    QList<Avogadro::QtGui::ScenePluginFactory*> scenePluginFactories =
        manager->pluginFactories<Avogadro::QtGui::ScenePluginFactory>();
    foreach (auto *factory, scenePluginFactories) {
        auto *scenePlugin = factory->createInstance();
        // enable Ball-and-Sticks
        if (scenePlugin->objectName() == QLatin1String("BallStick")) {
            scenePlugin->setEnabled(true);
        }
        sceneModel().addItem(scenePlugin);
    }

    // load tools
    if (!tools().isEmpty()) {
        qCritical() << "Updating non-empty toolset, erasing first.";
        qDeleteAll(tools());
    }
    auto toolPluginFactories =
        manager->pluginFactories<Avogadro::QtGui::ToolPluginFactory>();
    foreach (auto *factory, toolPluginFactories) {
        auto *tool = factory->createInstance();
        if (tool) {
            addTool(tool);
            if (factory->identifier() == QStringLiteral("Navigator")) {
                setDefaultTool(tool);
                setActiveTool(tool);
            }
        }
    }

    setMolecule(new Avogadro::QtGui::Molecule(this));
    update();
}

KalziumGLWidget::~KalziumGLWidget()
{
    //restore the LC_NUMERIC locale.
    setlocale(LC_NUMERIC, m_lc_numeric.constData());
}

bool KalziumGLWidget::openFile(const QString &file)
{
    // workaround for missing copy-constructor: fixed in Avogadra2 > 0.9
    Avogadro::QtGui::Molecule temp;
    temp = *IoWrapper::readMolecule(file);
    auto mol = new Avogadro::QtGui::Molecule(temp);

    if (!mol) {
        return false;
    }
    Avogadro::QtGui::Molecule* oldmol = molecule();
    if (oldmol) {
        oldmol->deleteLater();
    }
    setMolecule(mol);
    update();
    return true;
}
