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

#include <avogadro/rendering/primitive.h>
#include <avogadro/qtplugins/pluginmanager.h>
#include <avogadro/qtgui/molecule.h>
#include <avogadro/qtgui/sceneplugin.h>
// #include <avogadro/toolgroup.h>//FIXME:Avogadro2 port away from toolgroup
#include <QWidget>
#include "iowrapper.h"
#include <avogadro/qtopengl/glwidget.h>
#include <config-kalzium.h>

KalziumGLWidget::KalziumGLWidget(QWidget *parent)
    : Avogadro::QtOpenGL::GLWidget(parent)
    , m_lastEngine1(0)
    , m_lastEngine2(0)
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
    Avogadro::QtPlugins::PluginManager *manager = Avogadro::QtPlugins::PluginManager::instance();
    manager->load();
    QList<Avogadro::QtGui::ScenePluginFactory*> scenePluginFactories =
        manager->pluginFactories<Avogadro::QtGui::ScenePluginFactory>();
    foreach (auto *factory, scenePluginFactories) {
        auto *scenePlugin = factory->createInstance();
        // enable Ball-and-Sticks
        if (scenePlugin->objectName() == "BallStick") {
            scenePlugin->setEnabled(true);
        }
        sceneModel().addItem(scenePlugin);
    }

//     Avogadro::ToolGroup* tools = new Avogadro::ToolGroup(this);//FIXME:Avogadro2 port away from ToolGroup
//     tools->append(manager->tools(this));
//     tools->setActiveTool("Navigate");
//     setToolGroup(tools);

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
    auto tmpMol = IoWrapper::readMolecule(file);
    auto mol = new Avogadro::QtGui::Molecule(*tmpMol);
    delete tmpMol;
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

void KalziumGLWidget::setLabels(int style)
{
    //FIXME:Avogadro2
//     // Use the QSettings framework to configure the label engine
//     foreach (Avogadro::Engine *engine, engines()) {
//         if (engine->name() == "Label") {
//             QSettings settings;
//             int atomType = 0;
//             int bondType = 0;
//             bool enabled = false;
//             // We need to use
//             switch(style) {
//             case 0: // Display no labels
//                 enabled = false;
//                 break;
//             case 1: // Display the atom symbol
//                 enabled = true;
//                 atomType = 3;
//                 break;
//             case 2: // Display the atom name
//                 enabled = true;
//                 atomType = 2;
//                 break;
//             default:
//                 engine->setEnabled(false);
//             }
//             settings.setValue("atomLabel", atomType);
//             settings.setValue("bondLabel", bondType);
//             settings.setValue("enabled", enabled);
//             engine->readSettings(settings);
//         }
//     }
}
