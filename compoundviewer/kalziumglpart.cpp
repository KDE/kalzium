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


typedef KParts::GenericFactory<KalziumGLPart> KalziumGLPartFactory;

K_EXPORT_COMPONENT_FACTORY (libkalziumglpart, KalziumGLPartFactory)

KalziumGLPart::KalziumGLPart(QWidget* parentWidget, QObject* parent, const QStringList& args)
{
    kDebug() << "KalziumGLPart::KalziumGLPart()" << endl;

    m_widget = new Avogadro::GLWidget();
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
	mol->Center();

	m_widget->setMolecule( mol );
	m_widget->update();
}

#include "kalziumglpart.moc" 
