/***************************************************************************
    copyright            : (C) 2008 by Carsten Niehaus
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
#include "didyouknow.h"

#include <QDebug>
#include <QPainter>
#include <QPen>

KalziumDidyouknow::KalziumDidyouknow(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_theme("widgets/chalkboard", this),
    m_size(512,256)
{
    // init random sequence
    m_random = new KRandomSequence( QDateTime::currentDateTime().toTime_t() );

    m_engine = dataEngine("kalzium");
    m_label1 = 0;
    setHasConfigurationInterface(false);
    setAcceptDrops(false);
    setAcceptsHoverEvents(true);
    setDrawStandardBackground(false);
    
    m_theme.resize();
}

void KalziumDidyouknow::setContentSize(const QSizeF& size)
{
    m_size = size;
}

QSizeF KalziumDidyouknow::contentSizeHint() const
{
    return m_size;
}

void KalziumDidyouknow::init()
{
    qDebug() << "initializing DidYouKnow-Applet";

    m_engine->connectSource( "BlueObelisk:RandomElement" , this, 1000);

    m_theme.setContentType(Plasma::Svg::SingleImage);

    m_label1 = new Plasma::Label(this);
    m_label1->setPos( m_theme.elementRect( "canvas" ).topLeft() );
    m_label1->setPen( QPen( Qt::white ) );
}

void KalziumDidyouknow::constraintsUpdated(Plasma::Constraints constraints)
{
    setDrawStandardBackground(false);
    prepareGeometryChange();
    if (constraints & Plasma::SizeConstraint) {
        m_theme.resize(contentSize().toSize());
    }
    
    m_label1->setPos( m_theme.elementRect( "canvas" ).topLeft() );
}

KalziumDidyouknow::~KalziumDidyouknow()
{
    delete m_random;
}

void KalziumDidyouknow::dataUpdated(const QString& source, const Plasma::DataEngine::Data &data)
{
    qDebug() << "entering dataUpdated()";
    Q_UNUSED(source);
    

    if (m_label1)  {
	m_label1->setAlignment(Qt::AlignLeft);
        m_label1->setText(getFact( data ));
    }
}

QString KalziumDidyouknow::getFact( const Plasma::DataEngine::Data& data )
{
    QString symbol = data["symbol"].toString();
    QString name = data["name"].toString();
    QString bp = data["bp"].toString();
    QString mp = data["mp"].toString();
    QString mass = data["mass"].toString();
    
    int rand = m_random->getLong(3);
    qDebug() << "Randrom number is: " << rand;

    switch (rand) {
        case 0:
            qDebug() << "0";
            return i18n( "Did you know that\n the element %1 has the symbol %2?", name, symbol );
        case 1:
            qDebug() << "1";
            return i18n( "Did you know that\n %1 (%2) weights %3 u?", name, symbol, mass );
        case 2:
            qDebug() << "2";
            return getPreselectedFact();
        default:
            qDebug() << "default in switch";
            return i18n( "Did you know that\n the element %1 has the symbol %2?", name, symbol );
    }

    return i18n( "An error occured." );
}

QString KalziumDidyouknow::getPreselectedFact()
{
    QStringList facts;

    facts << i18n("Did you know that\n Test 1");
    facts << i18n("Did you know that\n Test 2");
    facts << i18n("Did you know that\n Test 3");
    int rand = m_random->getLong(facts.size());

    return facts.at( rand );
    
}

void KalziumDidyouknow::paintInterface(QPainter *p,
                       const QStyleOptionGraphicsItem *option,
                       const QRect &contentsRect)
{
    Q_UNUSED(option);

    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);

    // Now we draw the applet, starting with our svg
    m_theme.resize((int)contentsRect.width(), (int)contentsRect.height());
    m_theme.paint(p, (int)contentsRect.left(), (int)contentsRect.top());
}

#include "didyouknow.moc"
