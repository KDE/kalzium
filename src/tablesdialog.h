/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus <cniehaus@kde.org>
                           (C) 2007 by Ian Monroe <ian@monroe.nu>
 **************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TABLESDIALOG_H
#define TABLESDIALOG_H

#include <QTableWidget>
#include <QTableWidgetItem>

#include <KPageDialog>

/**
 * @author Carsten Niehaus
 */
class TablesDialog : public KPageDialog
{
    Q_OBJECT

public:
    explicit TablesDialog(QWidget *parent = nullptr);
    virtual ~TablesDialog();

    void createNumbersTable();
    void createGreekSymbolTable();
};

/**
 * Disallows the table widget item from being edited.
 * @author Ian Monroe
 */
class MyWidgetItem : public QTableWidgetItem
{
public:
    MyWidgetItem(const QString& s) : QTableWidgetItem(s)
    {
        setFlags(Qt::ItemIsEnabled);
    }

};

/**
 * Adds a context menu which copies to the clipboard the current cell.
 * @author Ian Monroe
 */
class MyTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit MyTableWidget(QWidget* parent);
protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
private slots:
    void copyToClipboard();
};

#endif // TABLESDIALOG_H
