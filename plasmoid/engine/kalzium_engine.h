#ifndef KALZIUM_ENGINE_H
#define KALZIUM_ENGINE_H

#include <KRandomSequence>

#include "plasma/dataengine.h"

class KEduVocDocument;

class KalziumEngine : public Plasma::DataEngine
{
Q_OBJECT

public:
    KalziumEngine(QObject* parent, const QVariantList& args);
    ~KalziumEngine();
    QStringList sources() const;

protected:
    bool sourceRequested(const QString &name);
    bool updateSource(const QString& source);

private:
    void openDocument(const QString& file);
    int m_current;
};

K_EXPORT_PLASMA_DATAENGINE(kalzium, KalziumEngine)

#endif // KALZIUM_ENGINE_H
