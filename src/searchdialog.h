/****************************************************************************
** Form interface generated from reading ui file './searchdialog.ui'
**
** Created: Sam Jun 14 14:56:20 2003
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SEARCHDIALOGUI_H
#define SEARCHDIALOGUI_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QTable;
class QCheckBox;
class KPushButton;
class QPushButton;
class QLabel;
class KComboBox;
class KLineEdit;

class SearchDialogUI : public QDialog
{
    Q_OBJECT

public:
    SearchDialogUI( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~SearchDialogUI();

    QTable* DataTable;
    QCheckBox* checkBox1;
    QCheckBox* checkBox2;
    QCheckBox* checkBox3;
    QCheckBox* checkBox4;
    QCheckBox* checkBox5;
    QCheckBox* checkBox6;
    QCheckBox* checkBox7;
    QCheckBox* checkBox8;
    QCheckBox* checkBox9;
    QCheckBox* checkBox10;
    QCheckBox* checkBox11;
    QCheckBox* checkBox12;
    QCheckBox* checkBox13;
    QCheckBox* checkBox14;
    QCheckBox* checkBox15;
    QCheckBox* checkBox16;
    KPushButton* ApplyButton;
    QPushButton* buttonOk;
    QPushButton* buttonExport;
    QLabel* textLabel1;
    KComboBox* KindCB;
    KComboBox* WhatCB;
    KLineEdit* ValueLE;
    KPushButton* kPushButton2;

public slots:
    virtual void slotExportData();
    virtual void slotApplyChanges();
    virtual void slotFilterData();

protected:
    QGridLayout* SearchDialogUILayout;
    QHBoxLayout* layout9;
    QVBoxLayout* layout8;
    QVBoxLayout* layout6;
    QHBoxLayout* Layout1;
    QHBoxLayout* layout6_2;

protected slots:
    virtual void languageChange();

};

#endif // SEARCHDIALOGUI_H
