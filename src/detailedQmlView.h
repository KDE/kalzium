
#ifndef DETAILEDQMLVIEW_H
#define DETAILEDQMLVIEW_H

#include <QWidget>
#include <QDeclarativeContext>

#include "kalziumelementproperty.h"
#include "element.h"

class DetailedQmlView : public QWidget
{
    Q_OBJECT
public:

    DetailedQmlView(QWidget *parent);
    ~DetailedQmlView();

public slots:
    /**
     * Set @p el as the element to be drawn
     */
    void setElement(int el);

    void itemPressed();

private:

    KalziumElementProperty *m_elementProperty;
    Element *m_element;

    QDeclarativeContext *m_context;

protected:
    /**
     * Generic event handler, currently defaults to calling parent class
     * (included for future compatibility)
     */
//     bool event(QEvent *e);

    /**
     * is caled every time the view is resized.
     */
    void resizeEvent(QResizeEvent * event);
};

#endif //DETAILEDQMLVIEW_H
