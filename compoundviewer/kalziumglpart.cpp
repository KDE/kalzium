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

#include <QStringList>

#include <avogadro/primitive.h>

#include "openbabel2wrapper.h"

KalziumGLWidget::KalziumGLWidget(QWidget *parent) : Avogadro::GLWidget(parent) {}

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
        }
    }
    update();
}

typedef KParts::GenericFactory<KalziumGLPart> KalziumGLPartFactory;

K_EXPORT_COMPONENT_FACTORY (libkalziumglpart, KalziumGLPartFactory)

KalziumGLPart::KalziumGLPart(QWidget* parentWidget, QObject* parent, const QStringList& args)
{
    kDebug() << "KalziumGLPart::KalziumGLPart()" << endl;

    m_widget = new KalziumGLWidget();
    m_widget->setObjectName("KalziumGLWidget-KPart");
}

KalziumGLPart::~KalziumGLPart()
{
    kDebug() << "KalziumGLPart::~KalziumGLPart()" << endl;
}

KAboutData *KalziumGLPart::createAboutData()
{
    KAboutData* aboutData = new KAboutData( "kalzium", I18N_NOOP("KalziumGLPart"),
            "1.1.1", I18N_NOOP("A cool thing"),
            KAboutData::License_GPL,
            "(c) 2006, Carsten Niehaus", 0, "http://edu.kde.org/kalzium/index.php",
            "kalzium@kde.org");
    aboutData->addAuthor("Carsten Niehaus.",0, "cniehaus@kde.org");

    return aboutData;
}

bool KalziumGLPart::openFile()
{
	Avogadro::Molecule* mol = OpenBabel2Wrapper::readMolecule( url().path() );

	m_widget->setMolecule( mol );
	m_widget->update();
}

#include "kalziumglpart.moc"
