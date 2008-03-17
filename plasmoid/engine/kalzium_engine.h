#ifndef KALZIUM_ENGINE_H
#define KALZIUM_ENGINE_H

#include <KRandomSequence>

#include "plasma/dataengine.h"

class Element;

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
    Element * m_currentElement;
    QList<Element*> m_elements;
};

K_EXPORT_PLASMA_DATAENGINE(kalzium, KalziumEngine)

#endif // KALZIUM_ENGINE_H
