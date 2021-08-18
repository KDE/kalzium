/*
    SPDX-FileCopyrightText: 2007 Johannes Simon
    email                : johannes.simon@gmail.com
*/
/***************************************************************************
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 *                                                                         *
 ***************************************************************************/

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QListWidget>

#include <element.h>
#include "kalziumdataobject.h"
#include "ui_exportdialog.h"


class ElementListEntry : public QListWidgetItem
{
public:
    explicit ElementListEntry(Element *element);
    ~ElementListEntry();

    int       m_atomicNum;
    QString   m_name;
    Element  *m_element;
};

class PropertyListEntry : public QListWidgetItem
{
public:
    PropertyListEntry(const QString & name, ChemicalDataObject::BlueObelisk type);
    ~PropertyListEntry();

    ChemicalDataObject::BlueObelisk m_type;
};

/**
* @author: Johannes Simon
*/
class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent);
    ~ExportDialog();

    void populateElementList();
    void exportToHtml();
    void exportToXml();
    void exportToCsv();

private:
    Ui::exportDialogForm ui;
    QTextStream *m_outputStream;

public slots:
    void slotOkClicked();
    /**
     * Open help page
     */
    void slotHelpRequested();

};

#endif // EXPORTDIALOG_H
