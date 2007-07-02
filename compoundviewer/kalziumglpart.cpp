/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
    email                : cniehaus@kde.org
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

#include <kparts/genericfactory.h>


#include <avogadro/primitive.h>
#include <avogadro/toolgroup.h>

#include "openbabel2wrapper.h"

KalziumGLWidget::KalziumGLWidget(QWidget *parent) : Avogadro::GLWidget(parent)
{
    // Prevent What's this from intercepting right mouse clicks
    setContextMenuPolicy( Qt::PreventContextMenu );
    // Load the tools and set navigate as the default
    Avogadro::ToolGroup* tools = new Avogadro::ToolGroup(this);
    tools->load();
    const QList<Avogadro::Tool *> toolList = tools->tools();
    foreach( Avogadro::Tool *tool, toolList )
    {
        if (tool->name() == "Navigate")
            tools->setActiveTool(tool);
    }
    setToolGroup(tools);
}

void KalziumGLWidget::setStyle( int style )
{
    foreach( Avogadro::Engine *engine, engines() )
    {
        if( engine->name() != "Label" ) {
            engine->setEnabled(false);
        }

        if( ( style == 0 && engine->name() == "Dynamic Ball and Stick" )
         || ( style == 1 && engine->name() == "Stick" )
         || ( style == 2 && engine->name() == "Sphere" )
         || ( style == 3 && engine->name() == "Wireframe" ) ) {
            engine->setEnabled(true);
            setRenderingEngine(engine);
        }
    }
    update();
}

typedef KParts::GenericFactory<KalziumGLPart> KalziumGLPartFactory;

K_EXPORT_COMPONENT_FACTORY (libkalziumglpart, KalziumGLPartFactory)

KalziumGLPart::KalziumGLPart(QWidget* parentWidget, QObject* parent, const QStringList& args)
{
    Q_UNUSED(parent);
    Q_UNUSED(parentWidget);
    Q_UNUSED(args);
    kDebug() << "KalziumGLPart::KalziumGLPart()" << endl;
    
    // use multi-sample (anti-aliased) OpenGL if available
    QGLFormat defFormat = QGLFormat::defaultFormat();
    defFormat.setSampleBuffers(true);
    QGLFormat::setDefaultFormat(defFormat);

    m_widget = new KalziumGLWidget();
    m_widget->setObjectName("KalziumGLWidget-KPart");
}

KalziumGLPart::~KalziumGLPart()
{
    delete m_widget;
    kDebug() << "KalziumGLPart::~KalziumGLPart()" << endl;
}

KAboutData *KalziumGLPart::createAboutData()
{
    KAboutData* aboutData = new KAboutData( "kalzium", 0, ki18n("KalziumGLPart"),
            "1.1.1", ki18n("A cool thing"),
            KAboutData::License_GPL,
            ki18n("(c) 2006, Carsten Niehaus"), KLocalizedString(), "http://edu.kde.org/kalzium/index.php",
            "kalzium@kde.org");
    aboutData->addAuthor(ki18n("Carsten Niehaus."),KLocalizedString(), "cniehaus@kde.org");

    return aboutData;
}

bool KalziumGLPart::openFile()
{
	Avogadro::Molecule* mol = OpenBabel2Wrapper::readMolecule( url().path() );

	m_widget->setMolecule( mol );
	m_widget->update();
	return true;
}

#include "kalziumglpart.moc"
