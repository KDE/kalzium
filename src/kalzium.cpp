/***************************************************************************

        kalzium.cpp  -  description
                             -------------------
    begin                : Die Dez  4 17:59:34 CET 2001
    copyright            : (C) 2001 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kalzium.h"
#include "kalzium.moc"
#include "../calculations/kmoledit.h"
#include "../calculations/kmolcalc.h"
#include "calcdialog.h"
#include "eleminfo.h"
#include "elementkp.h"
#include "settingsdialog.h"
#include "value_visualisation.h"       //this is for KDE 3.2

//KDE-Includes
#include <kaction.h>
#include <kapplication.h>
#include <kdebug.h>
#include <kconfig.h>
#include <kdialog.h>
#include <kedittoolbar.h>
#include <kglobalsettings.h>
#include <khelpmenu.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kpopupmenu.h>
#include <kpushbutton.h>
#include <ksimpleconfig.h>
#include <kstatusbar.h>
#include <kstdaction.h>
#include <kstddirs.h>
#include <kstdaction.h>
#include <kcolorbutton.h>

//QT-Includes
#include <qinputdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlcdnumber.h>
#include <qslider.h>
#include <qtooltip.h>
#include <qstring.h>
#include <qwhatsthis.h>
#include <qbutton.h>

// Table includes
#include "quizdlg.h"
#include "calcdlg.h"
#include "stateofmatterdlg.h"


Kalzium::Kalzium(const char *name) : KMainWindow( 0 ,name ), setDlg(0L)
{
    main_config=KGlobal::config();

    calculationdialog=NULL;
    templookup=NULL;

    setPlainCaption(i18n("Kalzium - v%1").arg(KALZIUM_VERSION));

    setupConfig();
    setupActions();

    QFont general = KGlobalSettings::generalFont();
    QFont general_bold = general;

    general.setPointSize(general.pointSize()-2);

    general_bold.setPointSize(general_bold.pointSize());
    general_bold.setBold(TRUE);

    QFontMetrics zahl( general );
    int zahl_height = zahl.height();
    int zahl_width= zahl.width("103");
    QFontMetrics text( general_bold );
    int text_height = text.height();
    int text_width = text.width("MD");
    

    //////////////////////////////////////
    //creation of the 118 buttons
    //
    QWidget *foo = new QWidget(this);
    QVBoxLayout *mainlayout = new QVBoxLayout( foo, 0, -1, "mainlayout" );

    maingrid = new QGridLayout( mainlayout, 18, 10, -1, "maingridlayout" );

    // Loop over all elements
    int h=0; int v=0;
    QString elementName;
    KSimpleConfig config (locate("data", "kalzium/kalziumrc"));
    ElementInfo eleminfo;
    for ( int n=0 ;n<118 ;n++ )
    {
        if (config.hasGroup(QString::number(n+1)))
        {
            config.setGroup(QString::number(n+1));
            eleminfo.Name=config.readEntry("Name", "Unknown");
            eleminfo.Symbol=config.readEntry("Symbol", "Unknown");

            eleminfo.Weight=config.readEntry("Weight","0.0");
            eleminfo.acidbeh=config.readEntry("acidbeh","0");
            eleminfo.Block=config.readEntry("Block","s");
            eleminfo.EN=config.readDoubleNumEntry("EN", -1);
            eleminfo.MP=config.readDoubleNumEntry("MP", -1);
            eleminfo.IE=config.readDoubleNumEntry("IE", -1);
            eleminfo.AR=config.readDoubleNumEntry("AR", -1);
            eleminfo.BP=config.readDoubleNumEntry("BP", -1);
            eleminfo.Density=config.readDoubleNumEntry("Density", -1);
            eleminfo.az=config.readEntry("az","0");
            eleminfo.date=config.readEntry("date","0");
            eleminfo.Group=config.readEntry("Group","1");
            eleminfo.number=(n+1);

        } else elementName="Unknown";
        position(n+1,h,v); //get position
        element[n] =  new ElementKP(eleminfo.Symbol,foo,eleminfo,elementName.latin1(),n+1,statusBar(),this);
        
        element[n]->setFixedSize(zahl_width + text_width ,
                                 zahl_height + text_height + 5);

        maingrid->addWidget(element[n], v/40+1, h/40);
        element[n]->show();

        //Now we add the WhatsThis-help for each button
        QWhatsThis::add(element[n], i18n("Click here to get information about %1.").arg(eleminfo.Symbol));
		QToolTip::add(element[n], eleminfo.Name);
    }

    //////////////////
    // feste minimal breite und höhe der gridelemente
    //////////////////
    for( int n=0; n<18; n++ ) maingrid->addColSpacing( n, 40 );
    for( int n=0; n<10; n++ ) maingrid->addRowSpacing( n, 40 );

    mainlayout->addStretch();

    QHBoxLayout *timeline_layout = new QHBoxLayout( mainlayout, -1, "timelinelayout" );
    timeline_layout->addStretch();

    dateS = new QSlider (QSlider::Horizontal, foo, "rotateSlider" );
    timeline_layout->addWidget( dateS );
    QWhatsThis::add(dateS, i18n("Use this slider to see what elements were known at a certain date."));
    dateS->setRange(1669, 2002);
    dateS->setValue(2002);
    dateS->hide();
    connect( dateS, SIGNAL(valueChanged(int)),  SLOT(timeline()));

    timeline_layout->addSpacing( 10 );

    dateLCD = new QLCDNumber( 4, foo, "dateLCD");
    timeline_layout->addWidget( dateLCD );
    QWhatsThis::add(dateLCD, i18n("This is the date which you have chosen with the slider. Currently, you are viewing the elements known in the year %1.").arg(QString::number(dateS->value())));
    dateLCD->hide();
    dateLCD->display("2002");

    timeline_layout->addStretch();

//neuer Kram heute morgen
    QHBoxLayout *legend_layout = new QHBoxLayout( mainlayout, -1, "legendlayout" );
    legend_layout->addStretch();

	main_config->setGroup( "Colors" );

	one = new KPushButton( foo );
	two = new KPushButton( foo );
	three = new KPushButton( foo );
	four = new KPushButton( foo );
	five = new KPushButton( foo );
	six = new KPushButton( foo );
	seven = new KPushButton( foo );
	eight = new KPushButton( foo );
	legend_layout->addWidget( one );
	legend_layout->addWidget( two );
	legend_layout->addWidget( three );
	legend_layout->addWidget( four );
	legend_layout->addWidget( five );
	legend_layout->addWidget( six );
	legend_layout->addWidget( seven );
	legend_layout->addWidget( eight );
	legend_layout->addStretch();
	
//neuer Kram heute morgen
    
    mainlayout->addStretch();

    for (int n = 0; n < 18; n++)
    {
        labels[n] = new QLabel (foo);
    }
    setCentralWidget(foo);
    
    updateMainWindow();
    createhelpArray();
}

Kalzium::~Kalzium()
{
    for (int n=0; n<118; n++)
        delete element[n];
}

void Kalzium::createhelpArray()
{
		for(int i=0;i<9;i++)
		{
				for(int e=0;e<18;e++)
				{
						helpArray[i][e]="leer";
				}
		}

		int ze=0,sp=0;
		for(int as=1;as<118;as++)
		{
				position(as,ze,sp);
				helpArray[sp/40][ze/40]=element[as-1]->Data.Symbol;
		}
}

bool Kalzium::queryClose() const
{
    main_config->setGroup("Menu Settings");
    main_config->writeEntry("psestylemenu",psestylemenu->currentItem()); 
    main_config->writeEntry("colorschememenu", colorschememenu->currentItem());
    main_config->writeEntry("numerationmenu", numerationmenu->currentItem());
    main_config->writeEntry("timelineshow", timelineToggleAction->isChecked());
	main_config->sync();
    return true;
}

void Kalzium::showCAS() const
{
    int h = 0, v = 0;
    for (int n = 0; n < 18; n++)
    {
        periodNrpos(n+1, h, v);
        QString name;
        periodName(n+1, name);
        maingrid->addWidget(labels[n], v/40, h/40);
        labels[n]->setText(name);
        labels[n]->setAlignment( Qt::AlignCenter );
        labels[n]->setAutoMask( true );
        QWhatsThis::add(labels[n], i18n("This period is called '%1' in the terminology used by the CAS.").arg(name));
        labels[n]->show();
    }
}

void Kalzium::showIUPAC() const
{
    int h = 0, v = 0;
    for (int n = 0; n < 18; ++n)
    {
        periodNrpos(n+1, h, v);
        maingrid->addWidget(labels[n], v/40, h/40);
        labels[n]->setText(QString::number(n+1));
        labels[n]->setAlignment(AlignCenter);
        labels[n]->setAutoMask( true );
        QWhatsThis::add(labels[n], i18n("This period is called '%1' in the terminology used by the IUPAC.").arg(QString::number(n+1)));
        labels[n]->show();
    }
}


//******** Slots *****************************************************
void Kalzium::changeColorScheme(int id) 
{
    static void (Kalzium::*funcs[])() = {
        &Kalzium::slotShowAcidBeh,
        &Kalzium::slotShowBlocks,
        &Kalzium::slotShowGroups,
        &Kalzium::slotShowStateOfMatter
    };

    (this->*funcs[id & ~3 ? 3 : id])();
}

void Kalzium::changeLegend(int id)
{
    main_config->setGroup("Colors");
    if (id == 0) //Acid Behaviours
    {
    one->setPaletteBackgroundColor(QColor(main_config->readColorEntry("acidic")));
    two->setPaletteBackgroundColor(QColor(main_config->readColorEntry("amphoteric")));
    three->setPaletteBackgroundColor(QColor(main_config->readColorEntry("basic")));
    four->setPaletteBackgroundColor(QColor(main_config->readColorEntry("neitherofthem")));
    five->hide();
    six->hide();
    seven->hide();
    eight->hide();
    one->setText(i18n("Acidic"));
    two->setText(i18n("Amphoteric"));
    three->setText(i18n("Basic"));
    four->setText(i18n("Neutral"));
    }
    if (id == 1) //Blocks
    {
    one->setPaletteBackgroundColor(QColor(main_config->readColorEntry("s")));
    two->setPaletteBackgroundColor(QColor(main_config->readColorEntry("p")));
    three->setPaletteBackgroundColor(QColor(main_config->readColorEntry("d")));
    four->setPaletteBackgroundColor(QColor(main_config->readColorEntry("f")));
    five->hide();
    six->hide();
    seven->hide();
    eight->hide();
    one->setText(i18n("s-Block"));
    two->setText(i18n("p-Block"));
    three->setText(i18n("d-Block"));
    four->setText(i18n("f-Block"));
    }
    if (id == 2) //Groups
    {
    one->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 1")));
    two->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 2")));
    three->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 3")));
    four->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 4")));
    five->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 5")));
    six->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 6")));
    seven->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 7")));
    eight->setPaletteBackgroundColor(QColor(main_config->readColorEntry("Group 8")));
    six->show();
    seven->show();
    eight->show();
    one->setText(i18n("Group 1"));
    two->setText(i18n("Group 2"));
    three->setText(i18n("Group 3"));
    four->setText(i18n("Group 4"));
    five->setText(i18n("Group 5"));
    six->setText(i18n("Group 6"));
    seven->setText(i18n("Group 7"));
    eight->setText(i18n("Group 8"));
    }
    if (id == 3) //State of Matter
    {
    one->setPaletteBackgroundColor(QColor(main_config->readColorEntry("liquid")));
    two->setPaletteBackgroundColor(QColor(main_config->readColorEntry("solid")));
    three->setPaletteBackgroundColor(QColor(main_config->readColorEntry("vapor")));
    four->setPaletteBackgroundColor(QColor(main_config->readColorEntry("artificial")));
    five->setPaletteBackgroundColor(QColor(main_config->readColorEntry("radioactive")));
    five->show();
    six->hide();
    seven->hide();
    eight->hide();
    one->setText(i18n("Liquid"));
    two->setText(i18n("Solid"));
    three->setText(i18n("Vapor"));
    four->setText(i18n("Artificial"));
    five->setText(i18n("Radioactive"));
    }
}
void Kalzium::changeNumeration(int id) const 
{
    switch (id) {
        case 0:
            showIUPAC();
            break;
        case 1:
            showCAS();
            break;
        case 2:
            for (int n = 0; n < 18; ++n)
                labels[n]->hide();
            break;
    }
}

void Kalzium::defineweights() const
{
    (new KMolEdit(0, "kmoledit", new KMolCalc))->exec();
}

void Kalzium::hideSettingsDialog()
{
    if(setDlg)
    {
        setDlg->delayedDestruct();
        setDlg=0;
    }
}

void Kalzium::showPseStyle(int i)
{
    if (i==0)
        slotShowMendelejew();
    else
        slotShowAll();
}

void Kalzium::showSettingsDialog()
{
    if(!setDlg)
    {
        setDlg = new SettingsDialog(this, "settingsdialog");
        connect (setDlg, SIGNAL(finished()), this, SLOT(hideSettingsDialog()));
        setDlg->show();
    }
    else
        setDlg->show();
}

void Kalzium::showToolbar() 
{
    if (toolbarToggleAction->isChecked())
        toolBar()->show();
    else
        toolBar()->hide();
}

void Kalzium::slotCalculations()
{
    if (!calculationdialog)
        calculationdialog = new CalcDialog(this, "Calculationsdialog");
    calculationdialog->show();
}

void Kalzium::slotKnowledge()
{
    bool ok = FALSE;
    int numofquestions = QInputDialog::getInteger(
            i18n( "How Many Questions?" ),
            i18n( "Please enter a number" ), 10, 0, 20, 1, &ok, this );
    if ( ok )
    {
        QuizDlg *quiz = new QuizDlg(this, "quizdialog", numofquestions); 
        quiz->setCaption(i18n("Test Your Knowledge"));
        quiz->show();
    }
}

void Kalzium::slotShowAcidBeh() {
    static const char *field[] = {
        "acidic", "basic", "amphoteric", "neitherofthem"
    };

    main_config->setGroup("Colors");

    for (int i = 0; i < 118; ++i)
    {
        PElementKP& b(element[i]);
        QString& s(b->Data.acidbeh);

        if (s.length() == 1 && s[0] >= '0' && s[0] <= '3')
            b->setPaletteBackgroundColor(QColor(main_config->readColorEntry(field[QChar(s[0]) - '0'])));
    }
}

void Kalzium::slotShowAll()
{
    static int nummer[14] = {2,10,18,21,28,31,32,36,43,49,54,55,72,75};

    for (int i = 0; i < 14 ; i++)
        element[nummer[i] - 1]->show();

    for (int i = 58; i < 71; i++)
        element[i]->show();

    for (int i = 83; i < 118; i++)
        element[i]->show(); 
    timelineToggleAction->setEnabled(true);
}

void Kalzium::slotShowBlocks()
{
    main_config->setGroup("Colors");
    for (int i = 0; i < 118; i++)
    {
        PElementKP& b(element[i]);
        QString& s(b->Data.Block);
        
        if (s.length() == 1)
            if (const char *p = strchr("spdf", QChar(s[0])))
                b->setPaletteBackgroundColor(QColor(main_config->readColorEntry(QChar(*p))));
    }
}

void Kalzium::slotShowGroups()
{
    main_config->setGroup("Colors");

    for (int i = 0; i < 118; ++i)
    {
        PElementKP& b(element[i]);
        QString& s(b->Data.Group);

        if (s.length() == 1 && s[0] >= '1' && s[0] <= '8')
        {
            static char group[] = { "Group %" };

            group[6] = char(QChar(s[0]));
            b->setPaletteBackgroundColor(QColor(main_config->readColorEntry(group)));
        }
    }
}

void Kalzium::slotShowMendelejew()
{
    static int nummer[14] = { 2,10,18,21,28,31,32,36,43,49,54,55,72,75 };
    static int dochda[4] = { 67,69,90,92 };

    for (int i=0; i<14 ; ++i)
        element[nummer[i] - 1]->hide();

    for (int i=83; i < 118; ++i)
        element[i]->hide();

    for (int i=58; i < 71; ++i)
        element[i]->hide();

    for (int i=0; i < 4; ++i)
        element[dochda[i] - 1]->show();
    timelineToggleAction->setEnabled(false);
}

void Kalzium::slotShowStateOfMatter()
{
    if (!templookup )
    {
           templookup  = new StateOfMatterDlg(this, "stateofmatterdlg", this);
    }
    templookup->show();
}

void Kalzium::updateColorMenu(int id) 
{
    colorschememenu->setCurrentItem(id);
}

void Kalzium::updateNumMenu(int id) 
{
    numerationmenu->setCurrentItem(id);
}

void Kalzium::slotShowTimeline(bool id)
{    
    if (id == true)
    {
        dateS->show();
        dateLCD->show();
        timeline();
        psestylemenu->setEnabled(false);
    }
    else
    {
        dateS->hide();
        dateLCD->hide();
        psestylemenu->setEnabled(true);
        for (int i =0; i<118; i++)
            element[i]->show();
        
    } 
}

void Kalzium::slotValues()
{
        ValueVisualisation *valuesDlg = new ValueVisualisation( this, "valuesDlg", this ); 
        valuesDlg->show();
}

void Kalzium::timeline()
{
    dateLCD->display(dateS->value());
    for (int i = 0; i < 118; ++i)
    {
        PElementKP& b(element[i]);
        if (b->Data.date > QString::number(dateS->value()))
            b->hide();
        else
            b->show();

        if (b->Data.date == "3333" && QString::number(dateS->value()) < "1996")
            b->hide();
    }
}


//*******SETUP ACTIONS*************************************************
void Kalzium::setupConfig()
{
	// set the default colors settings
	if (!main_config->hasGroup("Colors"))
	{
		main_config->setGroup("Colors");

		// State of Matters
		main_config->writeEntry("liquid",QColor(255,80,35));
		main_config->writeEntry("solid",QColor(30,80,60));
		main_config->writeEntry("vapor",QColor(110,80,60));
		main_config->writeEntry("radioactive",QColor(190,180,180));
		main_config->writeEntry("artificial",QColor(10,80,180));
		// Blocks
		main_config->writeEntry("s",QColor(255,80,35));
		main_config->writeEntry("p",QColor(30,80,60));
		main_config->writeEntry("d",QColor(10,80,180));
		main_config->writeEntry("f",QColor(130,80,255));
		// Groups
		main_config->writeEntry("Group 1",QColor(255,80,35));
		main_config->writeEntry("Group 2",QColor(30,80,60));
		main_config->writeEntry("Group 3",QColor(10,80,180));
		main_config->writeEntry("Group 4",QColor(130,80,255));
		main_config->writeEntry("Group 5",QColor(225,10,25));
		main_config->writeEntry("Group 6",QColor(33,30,70));
		main_config->writeEntry("Group 7",QColor(110,10,120));
		main_config->writeEntry("Group 8",QColor(190,2,212));
		// Acid Behaviours
		main_config->writeEntry("acidic",QColor(255,80,35));
		main_config->writeEntry("basic",QColor(30,80,60));
		main_config->writeEntry("amphoteric",QColor(10,80,180));
		main_config->writeEntry("neitherofthem",QColor(130,80,255));

	} 
	if (!main_config->hasGroup("WLU"))
	{
		main_config->setGroup("WLU");
		main_config->writeEntry("adress", "http://www.ktf-split.hr/periodni/en/");
	}
    if (!main_config->hasGroup("Menu Settings"))
    {
        main_config->setGroup("Menu Settings");
        main_config->writeEntry("psestylemenu", 1);
        main_config->writeEntry("colorschememenu", 1);
        main_config->writeEntry("numerationmenu", 0);
        main_config->writeEntry("timelineshow", false);
    }
	main_config->sync();
}

void Kalzium::setupActions()
{
    KIconLoader *kil = KGlobal::iconLoader();
    
    main_config->setGroup("Menu Settings");
    KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );
    KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection(), "configure");
    toolbarToggleAction = KStdAction::showToolbar(this, SLOT(showToolbar()), actionCollection());
    
    QStringList psestylelist;
    psestylelist.append( i18n("Mendeleev"));
    psestylelist.append( i18n("Complete"));
    psestylemenu = new KSelectAction(i18n("&PSE Style"),0,actionCollection(), "psestyle");
    psestylemenu->setItems(psestylelist);
    connect(psestylemenu, SIGNAL(activated(int)), this, SLOT(showPseStyle(int)));
    psestylemenu->setCurrentItem(main_config->readNumEntry("psestylemenu"));

    QStringList colorschemelist;
    colorschemelist.append( i18n("Acid Behavior"));
    colorschemelist.append( i18n("Blocks"));
    colorschemelist.append( i18n("Groups"));
    colorschemelist.append( i18n("State of Matter"));
    colorschememenu = new KSelectAction(i18n("&Color scheme"),0,actionCollection(), "colorscheme");
    colorschememenu->setItems(colorschemelist);
    connect(colorschememenu, SIGNAL(activated(int)), this, SLOT(updateColorMenu(int)));
    connect(colorschememenu, SIGNAL(activated(int)), this, SLOT(changeColorScheme(int)));
    connect(colorschememenu, SIGNAL(activated(int)), this, SLOT(changeLegend(int)));
    colorschememenu->setCurrentItem(main_config->readNumEntry("colorschememenu"));
    
    // BEGIN NUMERATIONMENU
    QStringList numerationlist;
    numerationlist.append( i18n("CAS"));
    numerationlist.append( i18n("IUPAC"));
    numerationlist.append( i18n("Off"));

    numerationmenu = new KSelectAction (i18n("&Numeration"),0,actionCollection(), "numeration");
    numerationmenu->setItems(numerationlist);

    connect(numerationmenu, SIGNAL(activated(int)), this, SLOT(updateNumMenu(int)));
    connect(numerationmenu, SIGNAL(activated(int)), this, SLOT(changeNumeration(int)));
    numerationmenu->setCurrentItem(main_config->readNumEntry("numerationmenu"));
    // END NUMERATIONMENU

    // BEGIN TIMELINE
    timelineToggleAction = new KToggleAction(i18n("Show Timeline"),kil->iconPath("timeline", KIcon::User),0,actionCollection(), "timeline");
    timelineToggleAction->setChecked(true);
    connect(timelineToggleAction, SIGNAL(toggled(bool)), this, SLOT(slotShowTimeline(bool)));

    timelineToggleAction->setChecked(main_config->readBoolEntry("timelineshow"));
    // END TIMELINEMENU
    
    (void) new KAction (i18n("Test Your &Knowledge"),0, this, SLOT(slotKnowledge()), actionCollection(), "test_your_knowledge");
//    (void) new KAction (i18n("Values"),0, this, SLOT(slotValues()), actionCollection(), "values"); // this is for KDE 3.2
    (void) new KAction (i18n("Calculations"),0, this, SLOT(slotCalculations()), actionCollection(), "calculations");
    (void) new KAction (i18n("Define Molecular Weights"),0, this, SLOT(defineweights()), actionCollection(), "defineweights");

    createGUI("kalziumui.rc");
}

void Kalzium::updateMainWindow()
{
    changeColorScheme(colorschememenu->currentItem());
    changeLegend(colorschememenu->currentItem());
    changeNumeration(numerationmenu->currentItem());
    showPseStyle(psestylemenu->currentItem());
    slotShowTimeline(timelineToggleAction->isChecked());
}
