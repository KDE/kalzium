#ifndef DATAEXPORTER_H
#define DATAEXPORTER_H

#include <QListWidget>
#include <QFile>
#include <element.h>

#include "kalziumdataobject.h"
#include "ui_exportdialog.h"

class KDialog;

class ElementListEntry : public QListWidgetItem
{
    public:
        ElementListEntry( Element * element );
        ~ElementListEntry();

        int       m_atomicNum;
        QString   m_name;
        Element * m_element;
};

class PropertyListEntry : public QListWidgetItem
{
    public:
        PropertyListEntry( const QString & name, ChemicalDataObject::BlueObelisk type );
        ~PropertyListEntry();

        ChemicalDataObject::BlueObelisk m_type;
};

/**
* @author: Johannes Simon
*/
class ExportDialog : public KDialog
{
    Q_OBJECT

    public:
        ExportDialog( QWidget * parent );
        ~ExportDialog();

        void populateElementList();
        void exportToHtml();
        void exportToXml();
        void exportToCsv();

    private:
        Ui::exportDialogForm ui;
        QTextStream * m_outputStream;
        

    public slots:
        void slotOkClicked();
        void slotHelpClicked();
};

#endif // DATAEXPORTER_H
