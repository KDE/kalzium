/***************************************************************************

        settingsdialog.cpp  -  description
                             -------------------
    begin                : Tue Apr 2 20:43:44 2002 UTC
    copyright            : (C) 2002 by Robert Gogolok
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

#include <kaction.h>
#include <kcolorbutton.h>
#include <kdebug.h>
#include <kconfig.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kstddirs.h>
#include <kcombobox.h>
#include <kurl.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qstring.h>
#include <qvbuttongroup.h>

// locale includes
#include "kalzium.h"
#include "settingsdialog.h"

ColorsTabWidget::ColorsTabWidget(QWidget *parent, const char *name)
: QTabWidget (parent,name)
{
    KConfig *main_config=KGlobal::config();  
    main_config->setGroup("Colors");

    // ------------------------------------------------
    // LAYOUT

    // Acid Behaviour TAP
    Acid = new QWidget(this);       
    acid_layout = new QGridLayout(Acid,5,2);
    acid_layout->setAutoAdd(TRUE);
    acidic = new QLabel(i18n( "Acidic:" ), Acid);
    acidic_b = new KColorButton(Acid);
    basic = new QLabel(i18n( "Basic:" ), Acid);
    basic_b = new KColorButton(Acid);
    amphoteric = new QLabel(i18n( "Amphoteric:" ), Acid);
    amphoteric_b = new KColorButton(Acid);
    neitherofthem = new QLabel(i18n( "Neutral:" ), Acid);
    neitherofthem_b = new KColorButton(Acid);
    acid_layout->setRowStretch(4,1);

    // Blocks TAP
    Blocks = new QWidget(this);
    blocks_layout = new QGridLayout(Blocks,5,2);
    blocks_layout->setAutoAdd(TRUE);
    s = new QLabel(i18n( "S-Block", "s:" ), Blocks);
    s_b = new KColorButton(Blocks);
    p = new QLabel(i18n( "P-Block", "p:" ), Blocks);
    p_b = new KColorButton(Blocks);
    d = new QLabel(i18n( "D-Block", "d:" ), Blocks);
    d_b = new KColorButton(Blocks);
    f = new QLabel(i18n( "F-Block", "f:" ), Blocks);
    f_b = new KColorButton(Blocks);
    blocks_layout->setRowStretch(4,1);

    // Groups TAP
    Groups = new QWidget(this);
    groups_layout = new QGridLayout(Groups,9,2);
    groups_layout->setAutoAdd(TRUE);
    Group1 = new QLabel(i18n( "Group 1:" ), Groups);
    Group1_b = new KColorButton(Groups);
    Group2  = new QLabel(i18n( "Group 2:" ), Groups);
    Group2_b = new KColorButton(Groups);
    Group3 = new QLabel(i18n( "Group 3:" ), Groups);
    Group3_b = new KColorButton(Groups);
    Group4 = new QLabel(i18n( "Group 4:" ), Groups);
    Group4_b = new KColorButton(Groups);
    Group5 = new QLabel(i18n( "Group 5:" ), Groups);
    Group5_b = new KColorButton(Groups);
    Group6 = new QLabel(i18n( "Group 6:" ), Groups);
    Group6_b = new KColorButton(Groups);
    Group7 = new QLabel(i18n( "Group 7:" ), Groups);
    Group7_b = new KColorButton(Groups);
    Group8 = new QLabel(i18n( "Group 8:" ), Groups);
    Group8_b = new KColorButton(Groups);
    groups_layout->setRowStretch(8,1);

    // Stateofmatters TAP
    Stateofmatters = new QWidget(this);
    stateofmatters_layout = new QGridLayout(Stateofmatters,6,2);
    stateofmatters_layout->setAutoAdd(TRUE);
    liquid = new QLabel(i18n( "Liquid:" ), Stateofmatters);
    liquid_b = new KColorButton(Stateofmatters);
    solid = new QLabel(i18n( "Solid:" ), Stateofmatters);
    solid_b = new KColorButton(Stateofmatters);
    vapor = new QLabel(i18n( "Vapor:" ), Stateofmatters);
    vapor_b = new KColorButton(Stateofmatters);
    radioactive = new QLabel(i18n( "Radioactive:" ), Stateofmatters);
    radioactive_b = new KColorButton(Stateofmatters);
    artificial = new QLabel(i18n( "Artificial:" ), Stateofmatters);
    artificial_b = new KColorButton(Stateofmatters);
    stateofmatters_layout->setRowStretch(5,1);

    // read color entries
    acidic_b->setColor(QColor(main_config->readColorEntry("acidic")));
    basic_b->setColor(QColor(main_config->readColorEntry("basic")));
    amphoteric_b->setColor(QColor(main_config->readColorEntry("amphoteric")));
    neitherofthem_b->setColor(QColor(main_config->readColorEntry("neitherofthem")));

    s_b->setColor(QColor(main_config->readColorEntry("s")));
    p_b->setColor(QColor(main_config->readColorEntry("p")));
    d_b->setColor(QColor(main_config->readColorEntry("d")));
    f_b->setColor(QColor(main_config->readColorEntry("f")));
    
    Group1_b->setColor(QColor(main_config->readColorEntry("Group 1")));
    Group2_b->setColor(QColor(main_config->readColorEntry("Group 2")));
    Group3_b->setColor(QColor(main_config->readColorEntry("Group 3")));
    Group4_b->setColor(QColor(main_config->readColorEntry("Group 4")));
    Group5_b->setColor(QColor(main_config->readColorEntry("Group 5")));
    Group6_b->setColor(QColor(main_config->readColorEntry("Group 6")));
    Group7_b->setColor(QColor(main_config->readColorEntry("Group 7")));
    Group8_b->setColor(QColor(main_config->readColorEntry("Group 8")));
    
    liquid_b->setColor(QColor(main_config->readColorEntry("liquid")));
    solid_b->setColor(QColor(main_config->readColorEntry("solid")));
    vapor_b->setColor(QColor(main_config->readColorEntry("vapor")));
    radioactive_b->setColor(QColor(main_config->readColorEntry("radioactive")));
    artificial_b->setColor(QColor(main_config->readColorEntry("artificial")));
    
    // add Tabs
    addTab(Acid, i18n( "Acid Behavior" ));
    addTab(Groups, i18n( "Groups" )); 
    addTab(Blocks, i18n( "Blocks" ));
    addTab(Stateofmatters, i18n( "State of Matter" ));
}

void ColorsTabWidget::setDefaultColors()
{
    if(currentPage()==Stateofmatters)
    {
            liquid_b->setColor(QColor(255,80,35));
            solid_b->setColor(QColor(30,80,60));
            vapor_b->setColor(QColor(110,80,60));
            radioactive_b->setColor(QColor(190,180,180));
            artificial_b->setColor(QColor(10,80,180));
    }
    else if(currentPage()==Blocks)
    {
            s_b->setColor(QColor(255,80,35));
            p_b->setColor(QColor(30,80,60));
            d_b->setColor(QColor(10,80,180));
            f_b->setColor(QColor(130,80,255));
    }
    else if(currentPage()==Groups)
    {
            Group1_b->setColor(QColor(255,80,35));
            Group2_b->setColor(QColor(30,80,60));
            Group3_b->setColor(QColor(10,80,180));
            Group4_b->setColor(QColor(130,80,255));
            Group5_b->setColor(QColor(225,10,25));
            Group6_b->setColor(QColor(33,30,70));
            Group7_b->setColor(QColor(110,10,120));
            Group8_b->setColor(QColor(190,2,212));
    } 
    else 
    {
            acidic_b->setColor(QColor(255,80,35));
            basic_b->setColor(QColor(30,80,60));
            amphoteric_b->setColor(QColor(10,80,180));
            neitherofthem_b->setColor(QColor(130,80,255));
    }
}

void ColorsTabWidget::saveColors()
{
    KConfig *main_config=KGlobal::config();  
    main_config->setGroup("Colors");


    // Stateofmatters
    main_config->writeEntry("liquid",liquid_b->color());
    main_config->writeEntry("solid",solid_b->color());
    main_config->writeEntry("vapor",vapor_b->color());
    main_config->writeEntry("radioactive",radioactive_b->color());
    main_config->writeEntry("artificial",artificial_b->color());
    // Blocks
    main_config->writeEntry("s",s_b->color());
    main_config->writeEntry("p",p_b->color());
    main_config->writeEntry("d",d_b->color());
    main_config->writeEntry("f",f_b->color());
    // Groups
    main_config->writeEntry("Group 1",Group1_b->color());
    main_config->writeEntry("Group 2",Group2_b->color());
    main_config->writeEntry("Group 3",Group3_b->color());
    main_config->writeEntry("Group 4",Group4_b->color());
    main_config->writeEntry("Group 5",Group5_b->color());
    main_config->writeEntry("Group 6",Group6_b->color());
    main_config->writeEntry("Group 7",Group7_b->color());
    main_config->writeEntry("Group 8",Group8_b->color());
    // Acid Behaviours
    main_config->writeEntry("acidic",acidic_b->color());
    main_config->writeEntry("basic",basic_b->color());
    main_config->writeEntry("amphoteric",amphoteric_b->color());
    main_config->writeEntry("neitherofthem",neitherofthem_b->color());
    main_config->sync();

}

void ColorsTabWidget::applyColors()
{
    saveColors();
}

//********SettingsDialogg**********************************************
SettingsDialog::SettingsDialog(QWidget *parent, const char *name)
    : KDialogBase(IconList, i18n("Preferences"), Help|Default|Ok|Apply|Cancel ,Ok, parent,name, true, false)
{
    main_config=KGlobal::config();  





    // COLORSTAB WIDGET
    colorTab = addPage(i18n("Colors"), i18n("Customize Color Settings"), BarIcon("colorize", KIcon::SizeMedium));
    colorsTabWidget = new ColorsTabWidget(colorTab, "colorsTabWidget");
    QVBoxLayout *vcolorTabBox = new QVBoxLayout(colorTab);
    vcolorTabBox->addWidget(colorsTabWidget);

    // WEBLOOKUP BUTTONS
    webLookupButtons = addPage(i18n("Web Lookup"), i18n("Customize Web Lookup URL"), BarIcon ("connect_creating", KIcon::SizeMedium));
    webLookupButtonGroup = new QVButtonGroup(webLookupButtons, "weblookup ButtonGroup");
    QVBoxLayout *test = new QVBoxLayout(webLookupButtons);
    test->addWidget(webLookupButtonGroup);

    KURL enURL = "http://www.ktf-split.hr/periodni/en/";
    KURL itURL =  "http://www.ktf-split.hr/periodni/it/";
    KURL deURL =  "http://www.ktf-split.hr/periodni/de/";
    KURL frURL =  "http://www.ktf-split.hr/periodni/fr/";
    KURL huURL =  "http://www.ktf-split.hr/periodni/";
    KURL nlURL =  "http://www-woc.sci.kun.nl/cgi-bin/viewelement?";
    QPixmap flagen( locate( "locale", QString::fromLatin1( "l10n/%1/flag.png" ).arg("gb") ) );
    QPixmap flagit( locate( "locale", QString::fromLatin1( "l10n/%1/flag.png" ).arg("it") ) );
    QPixmap flagde( locate( "locale", QString::fromLatin1( "l10n/%1/flag.png" ).arg("de") ) );
    QPixmap flagfr( locate( "locale", QString::fromLatin1( "l10n/%1/flag.png" ).arg("fr") ) );
    QPixmap flaghu( locate( "locale", QString::fromLatin1( "l10n/%1/flag.png" ).arg("hu") ) );
    QPixmap flagnl( locate( "locale", QString::fromLatin1( "l10n/%1/flag.png" ).arg("nl") ) );

    urlSelector = new KComboBox( FALSE, webLookupButtonGroup, "urlSelector" );
    urlSelector->insertURL( flagen, enURL );
    urlSelector->insertURL( flagit, itURL );
    urlSelector->insertURL( flagde, deURL );
    urlSelector->insertURL( flagfr, frURL );
    urlSelector->insertURL( flaghu, huURL );
    urlSelector->insertURL( flagnl, nlURL );
    
    /*
     * the next two lines look for the currently set language and set the 
     * ComboBox to the correct value ( index )
     */
    main_config->setGroup("WLU");
    urlSelector->setCurrentItem( translateCurrentLang( main_config->readEntry( "adress") ) );

    // CONNECT
    connect(this, SIGNAL(applyClicked()), this, SLOT(slotApplySettings()));
    connect(this, SIGNAL(defaultClicked()), this, SLOT(slotDefaults()));
    connect(this, SIGNAL(okClicked()), this, SLOT(slotOkSettings()));
    
}

int SettingsDialog::translateCurrentLang( QString lang ){
    if ( lang == "http://www.ktf-split.hr/periodni/gb/")
        return 0;
    if ( lang == "http://www.ktf-split.hr/periodni/it/")
        return 1;
    if ( lang == "http://www.ktf-split.hr/periodni/de/")
        return 2;
    if ( lang == "http://www.ktf-split.hr/periodni/fr/")
        return 3;
    if ( lang == "http://www.ktf-split.hr/periodni/")
        return 4;
    if ( lang == "http://www.ktf-split.hr/periodni/nl/")
        return 5;
}


void SettingsDialog::slotApplySettings()
{
    slotOkSettings();
}


void SettingsDialog::slotOkSettings()
{
    main_config->setGroup("WLU");
    main_config->writeEntry("adress", urlSelector->currentText() );
    main_config->sync();
    colorsTabWidget->applyColors();
    ((Kalzium*)parentWidget())->changeColorScheme(((Kalzium*)parentWidget())->colorschememenu->currentItem());
}

void SettingsDialog::slotDefaults()
{
    switch (activePageIndex())
    {
        case 0:
            colorsTabWidget->setDefaultColors();
            break;
        case 1:
            rb->toggle();
            break;
            
    }
}

#include "settingsdialog.moc"
