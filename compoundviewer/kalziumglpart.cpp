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
#include "kalziumglpart.h"

#include <QSettings>

#include <kparts/genericfactory.h>

#include <avogadro/primitive.h>
#include <avogadro/toolgroup.h>

#include "openbabel2wrapper.h"

KalziumGLWidget::KalziumGLWidget(QWidget *parent) : Avogadro::GLWidget(parent)
{
    // work around a bug in OpenBabel: the chemical data files parsing
    // is dependent on the LC_NUMERIC locale.
    lc_numeric = QString::fromAscii(setlocale(LC_NUMERIC, 0));
    setlocale(LC_NUMERIC, "C");

    // Prevent What's this from intercepting right mouse clicks
    setContextMenuPolicy(Qt::PreventContextMenu);
    // Load the tools and set navigate as the default
    Avogadro::ToolGroup* tools = new Avogadro::ToolGroup(this);
    tools->load();
    const QList<Avogadro::Tool *> toolList = tools->tools();
    foreach(Avogadro::Tool *tool, toolList)
    {
        if (tool->name() == "Navigate")
            tools->setActiveTool(tool);
    }
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
    setlocale(LC_NUMERIC, lc_numeric.toAscii());
}

void KalziumGLWidget::setStyle( int style )
{
    foreach(Avogadro::Engine *engine, engines())
    {
        if((style == 0 && engine->name() == "Ball and Stick")
           || (style == 1 && engine->name() == "Stick")
           || (style == 2 && engine->name() == "Van der Waals Spheres")
           || (style == 3 && engine->name() == "Wireframe"))
        {
            engine->setEnabled(true);
        }
        else if(engine->name() != "Label")
            engine->setEnabled(false);
    }
    update();
}

void KalziumGLWidget::setLabels(int style)
{
  // Use the QSettings framework to configure the label engine
  foreach(Avogadro::Engine *engine, engines())
  {
    if(engine->name() == "Label")
    {
      QSettings settings;
      int atomType = 0;
      int bondType = 2;
      bool enabled = false;
      // We need to use 
      switch(style)
      {
        case 0: // Display no labels
          enabled = false;
          break;
        case 1: // Display the atom index
          enabled = true;
          atomType = 0;
          break;
        case 2: // Display the atom symbol
          enabled = true;
          atomType = 1;
          break;
        case 3: // Display the atom name
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
    if(quality == 1) q = 2;
    else if(quality == 2) q = 4;
    GLWidget::setQuality(q);
    invalidateDLs();
    GLWidget::update();
}

void KalziumGLWidget::setNavigate()
{
    const QList<Avogadro::Tool *> toolList = toolGroup()->tools();
    foreach(Avogadro::Tool *tool, toolList)
        if (tool->name() == "Navigate")
        {
            toolGroup()->setActiveTool(tool);
            kDebug() << "Navigate tool selected";
            return;
        }
}

void KalziumGLWidget::setEdit()
{
    const QList<Avogadro::Tool *> toolList = toolGroup()->tools();
    foreach(Avogadro::Tool *tool, toolList)
    {
        if (tool->name() == "Draw")
        {
            toolGroup()->setActiveTool(tool);
            kDebug() << "Draw tool selected.";
            return;
        }
    }
    if (!molecule())
    {
      setMolecule(new Avogadro::Molecule(this));
      update();
    }
}

typedef KParts::GenericFactory<KalziumGLPart> KalziumGLPartFactory;

K_EXPORT_COMPONENT_FACTORY (libkalziumglpart, KalziumGLPartFactory)

KalziumGLPart::KalziumGLPart(QWidget* parentWidget, QObject* parent, const QStringList& args)
{
    Q_UNUSED(parent);
    Q_UNUSED(parentWidget);
    Q_UNUSED(args);
    kDebug() << "KalziumGLPart::KalziumGLPart()";
    
    m_widget = new KalziumGLWidget();
    m_widget->setObjectName("KalziumGLWidget-KPart");
}

KalziumGLPart::~KalziumGLPart()
{
    delete m_widget;
    kDebug() << "KalziumGLPart::~KalziumGLPart()";
}

KAboutData *KalziumGLPart::createAboutData()
{
    KAboutData* aboutData = new KAboutData( "kalzium", 0, ki18n("KalziumGLPart"),
            "1.1.1", ki18n("A cool thing"),
            KAboutData::License_GPL,
            ki18n("(c) 2006, Carsten Niehaus"), KLocalizedString(), "http://edu.kde.org/kalzium/index.php",
            "kalzium@kde.org");
    aboutData->addAuthor(ki18n("Carsten Niehaus."),KLocalizedString(), "cniehaus@kde.org");
    aboutData->addAuthor(ki18n("Marcus D. Hanwell."),KLocalizedString(), "marcus@cryos.org");

    return aboutData;
}

bool KalziumGLPart::openFile()
{
  Avogadro::Molecule* mol = OpenBabel2Wrapper::readMolecule( url().path() );
  if(!mol) return false;
  Avogadro::Molecule* oldmol = m_widget->molecule();
  if(oldmol) oldmol->deleteLater();
  m_widget->setMolecule( mol );
  m_widget->update();
  return true;
}

#include "kalziumglpart.moc"
