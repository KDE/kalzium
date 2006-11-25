#ifndef KALZIUMGLPART_H
#define KALZIUMGLPART_H

#include <kparts/factory.h>
#include <kparts/genericfactory.h>

class KalziumGLPart : public KParts::ReadOnlyPart
{
    Q_OBJECT
    public:
        KalziumGLPart(QWidget*, QObject*, const QStringList&);
        virtual ~KalziumGLPart();

        static KAboutData* createAboutData();

    public slots:
        bool openUrl(const KUrl& url);

    protected:
        bool openFile();
};

#endif // KALZIUMGLPART_H
