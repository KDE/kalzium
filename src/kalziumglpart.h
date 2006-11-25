#ifndef KALZIUMGLPART_H
#define KALZIUMGLPART_H

#include <kparts/factory.h>
#include <kparts/genericfactory.h>

class KalziumGLPart : public KParts::ReadOnlyPart
{
    Q_OBJECT
    public:
        KalziumGLPart(QWidget*, const char*, QObject*, const QStringList&);
        virtual ~KalziumGLPart();

        static KAboutData* createAboutData();

    public slots:
        bool openURL(const KUrl& url);
};

#endif // KALZIUMGLPART_H
