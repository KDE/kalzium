/***************************************************************************

        settingsdialog.h  -  description
                             -------------------
    begin                : Tue Apr 2 20:43:44 2002 UTC
    copyright            : (C) 2002, 2003 by Robert Gogolok
    email                : mail@robert-gogolok.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _KALZIUM_SETTINGSDIALOG_H_
#define _KALZIUM_SETTINGSDIALOG_H_

#include <kdialogbase.h>
#include <qtabwidget.h>

class KColorButton;
class QVButtonGroup;
class QComboBox;
class QGridLayout;
class QLabel;
class QRadioButton;
class QString;
class KComboBox;

class ColorsTabWidget : public QTabWidget
{
    Q_OBJECT
    public:
        ColorsTabWidget(QWidget *parent = 0, const char *name = 0);
        QString indexnumber;
        
    private:
        QWidget *Stateofmatters, *Blocks, *Groups,*Acid;

        QLabel 
            *liquid, *solid, *vapor, *radioactive, *artificial,
        *s,*p,*d,*f, 
        *Group1, *Group2, *Group3, *Group4, *Group5, *Group6, *Group7, *Group8, 
        *acidic, *basic, *amphoteric, *neitherofthem;

        KColorButton 
            *liquid_b, *solid_b, *vapor_b, *radioactive_b , *artificial_b,
        *s_b,*p_b,*d_b,*f_b, 
        *Group1_b, *Group2_b, *Group3_b, *Group4_b, *Group5_b, *Group6_b, *Group7_b, *Group8_b, 
        *acidic_b, *basic_b, *amphoteric_b, *neitherofthem_b; 

        QComboBox *dropdown;
        QGridLayout *stateofmatters_layout, *blocks_layout, *groups_layout, *acid_layout;

    public slots:
        void setDefaultColors();
        void applyColors();
    private slots:        
        void saveColors();
};

class SettingsDialog : public KDialogBase
{
    Q_OBJECT
    public:
        SettingsDialog(QWidget *parent=0, const char *name=0);
        KComboBox *urlSelector;
        int translateCurrentLang( QString );
    private:
        ColorsTabWidget *colorsTabWidget;
        
        QVButtonGroup *webLookupButtonGroup;
        QFrame *colorTab, *webLookupButtons;
        QRadioButton *rb;
        KConfig *main_config;

    private slots:
        /**
        * This method is called when clicking on Apply Button. 
        * It saves the settings the user did.
        */
        void slotApplySettings();
        
        /**
        * This method called when clicking on Ok Button. 
        * It saves the settings the user did and quits.
        */
        void slotOkSettings();

    protected slots:
        void slotDefaults();
        
};

#endif
