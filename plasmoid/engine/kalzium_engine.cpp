
#include "kalzium_engine.h"

#include <element.h>
#include <elementparser.h>

#include <QFile>
#include <QStringList>

#include <KLocale>
#include <KUrl>
#include <KStandardDirs>
#include <KConfigGroup>

#include "plasma/datacontainer.h"

KalziumEngine::KalziumEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent)
{
    Q_UNUSED(args)
    setMinimumUpdateInterval(1000);

    // reading elements
    ElementSaxParser * parser = new ElementSaxParser();

    QFile xmlFile( KStandardDirs::locate( "data", "libkdeedu/data/elements.xml" ) );
    QXmlInputSource source(&xmlFile);
    QXmlSimpleReader reader;

    reader.setContentHandler(parser);
    reader.parse(source);

    m_elements = parser->getElements();
    delete parser;
    
    m_currentElement = m_elements.at(4);

    qDebug() << "KalziumEngine::KalziumEngine";
}

KalziumEngine::~KalziumEngine()
{
}

QStringList KalziumEngine::sources() const
{
    QStringList list;
    list << QLatin1String("BlueObeslisk");
    return list;
}

bool KalziumEngine::sourceRequested(const QString &source)
{
    if (source == QLatin1String("BlueObelisk")) {
        setData(QLatin1String("BlueObelisk"), "testdaten");
        return true;
    }
    return false;
}

bool KalziumEngine::updateSource(const QString &source)
{
    if (!m_currentElement) {
        setData(source, i18n("No element set."));
        return false;
    }
    setData("lang:0", "testdaten1");
    setData("lang:2", "testdaten2");
    
    qDebug() << "KalziumEngine::updateSource()" << source;
    return true;
}

#include "kalzium_engine.moc"
