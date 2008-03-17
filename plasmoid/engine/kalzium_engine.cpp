
#include "kalzium_engine.h"

#include <QStringList>

#include <KDebug>
#include <KConfigGroup>


#include "plasma/datacontainer.h"

KalziumEngine::KalziumEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent)
{
    Q_UNUSED(args)
    setMinimumUpdateInterval(1000);

    kDebug() << "KalziumEngine::KalziumEngine";
//X     m_doc = new KEduVocDocument(this);
//X     m_current = 0;
//X 
//X     KConfig parleyConfig("parleyrc");
//X     kDebug() << parleyConfig.groupList();
//X     KConfigGroup recentFilesGroup( &parleyConfig, "Recent Files" );
//X     // take the last file, but there are File1..n and Name1..n entries..
//X     QString file = recentFilesGroup.readEntry( recentFilesGroup.keyList().value(recentFilesGroup.keyList().count()/2-1), QString() );
//X     openDocument(file);
//X     m_random = new KRandomSequence( QDateTime::currentDateTime().toTime_t() );

}

KalziumEngine::~KalziumEngine()
{
}

void KalziumEngine::openDocument(const QString& file)
{
//X     kDebug() << "Open vocabulary file: '" << file << "'";
//X     if (!file.isEmpty()) {
//X         m_doc->open(file);
//X //         m_vocabularyCount = m_doc->lesson()->entries(KEduVocContainer::Recursive).count();
//X     }
}

QStringList KalziumEngine::sources() const
{
//X    QStringList list;
//X    list << QLatin1String("Random");
//X    return list;
}

bool KalziumEngine::sourceRequested(const QString &source)
{
//X     KEduVocExpression *expression = m_doc->lesson()->entries(KEduVocContainer::Recursive).value(m_current);
//X 
//X     if (!expression) {
//X         return false;
//X     }
//X     
//X     kDebug() << "updateSource:" << source;
//X     kDebug() << expression->translation(0)->text();
//X     
//X //     kDebug() << "KalziumEngine::sourceRequested " << source;
//X     if (source.startsWith("lang:")) {
//X         if (expression) {
//X             int lang = source.right(source.size() - 5).toInt();
//X             setData(source, expression->translation(lang)->text());
//X             return true;
//X         }
//X     }
//X 
//X     if (source == QLatin1String("Random")) {
//X         if (expression) {
//X             QString text;
//X             foreach (int index, expression->translationIndices()) {
//X                 text += "\n" + expression->translation(index)->text();
//X             }
//X             setData(QLatin1String("Random"), text);
//X             return true;
//X         }
//X     }
    return false;
}

bool KalziumEngine::updateSource(const QString &source)
{
//X     if (!m_doc) {
//X         setData(source, i18n("No document set."));
//X         return false;
//X     }
//X 
//X     int vocabularyCount = m_doc->lesson()->entries(KEduVocContainer::Recursive).count();
//X     m_current = m_random->getLong(vocabularyCount);
//X     KEduVocExpression *expression = m_doc->lesson()->entries(KEduVocContainer::Recursive).value(m_current);
//X 
//X     setData("lang:0", expression->translation(0)->text());
//X 
//X     setData("lang:1", expression->translation(1)->text());
//X //     kDebug() << "KalziumEngine::updateSource()" << source;
//X 
//X     // other sources
    return true;
}

#include "kalzium_engine.moc"
