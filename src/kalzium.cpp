/***************************************************************************

        kalzium.cpp  -  description
                             -------------------
    begin                : Die Dez  4 17:59:34 CET 2001
    copyright            : (C) 2001, 2002, 2003 by Carsten Niehaus
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

//KDE-Includes
#include <kapplication.h>
#include <kdebug.h>
#include <kaction.h>
#include <kedittoolbar.h>
#include <kkeydialog.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <klocale.h>
#include <ksimpleconfig.h>
#include <kstandarddirs.h>

//QT-Includes
#include <qinputdialog.h>
#include <qlayout.h>
#include <qlcdnumber.h>
#include <qslider.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

// Table includes
#include "quizdlg.h"
#include "stateofmatterdlg.h"
#include "kalzium.h"
#include "kalzium.moc"
#include "../calculations/kmoledit.h"
#include "calcdialog.h"
#include "eleminfo.h"
#include "settingsdialog.h"
#include "legend.h"
#include "searchdlg.h"

#include "kalziumplotdialogimpl.h"


Kalzium::Kalzium() : KMainWindow( 0 ), setDlg(0L)
{
    connect (kapp, SIGNAL(kdisplayFontChanged()), this, SLOT (setFont()));
    setFont();
    main_config=KGlobal::config();

    calculationdialog=NULL;
    templookup=NULL;

    setPlainCaption(i18n("Kalzium - v%1").arg(KALZIUM_VERSION));

    setupConfig();
    setupActions();

    main_window = new QWidget(this, "main_window");

    mainlayout = new QVBoxLayout( main_window, 0, -1, "mainlayout" );

    setupAllElementKPButtons();

    setupTimeline();

    setupCaption();

    setCentralWidget(main_window);

    updateMainWindow();
    createhelpArray();
}

Kalzium::~Kalzium()
{
    for (int n=0; n<109; n++)
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
	for(int as=1;as<109;as++)
	{
		position(as,ze,sp);
		helpArray[sp/40][ze/40]=element[as-1]->Data.Symbol;
	}
}

bool Kalzium::queryClose()
{
    main_config->setGroup("Menu Settings");
    main_config->writeEntry("psestylemenu",psestylemenu->currentItem());
    main_config->writeEntry("colorschememenu", colorschememenu->currentItem());
    main_config->writeEntry("numerationmenu", numerationmenu->currentItem());
    main_config->writeEntry("timelineshow", timelineToggleAction->isChecked());
    main_config->writeEntry("quickinfo",quickinfoToggleAction->isChecked());
    main_config->writeEntry("legend",legendToggleAction->isChecked());
  	main_config->sync();
    return true;
}

void Kalzium::setupAllElementKPButtons()
{
    //////////////////////////////////////
    //creation of the 109 buttons
    //
    maingrid = new QGridLayout( mainlayout, 18, 10, -1, "maingridlayout" );
    maingrid->setOrigin(QGridLayout::TopLeft);

    // Loop over all elements
    int h=0; int v=0;
    QString elementName;
    KSimpleConfig config (locate("data", "kalzium/kalziumrc"));
    ElementInfo eleminfo;
    for ( int n=0 ;n<109 ;n++ )
	{
		if (config.hasGroup(QString::number(n+1)))
		{
			config.setGroup(QString::number(n+1));
			eleminfo.Name=config.readEntry("Name", "Unknown");
			eleminfo.Symbol=config.readEntry("Symbol", "Unknown");

			eleminfo.oxstage=config.readEntry("Ox","0");
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
			eleminfo.orbits=config.readEntry("Orbits","0");
			eleminfo.biological=config.readNumEntry( "biological" , -1 );
			eleminfo.number=(n+1);

		} else elementName="Unknown";
		position( n+1,h,v ); //get position
		element[n] =  new ElementKP ( main_window, eleminfo, elementName.latin1(), n+1, statusBar(),this );


		maingrid->addWidget( element[n], v/40+1, h/40 );
		element[n]->show();

		//Now we add the WhatsThis-help for each button
		QWhatsThis::add( element[n], i18n("Click here to get information about %1.").arg(eleminfo.Symbol) );
		QToolTip::add( element[n], eleminfo.Name.utf8() );
	}

    //////////////////
    // fixed minimal wide and high of the gridelements
    //////////////////
    for( int n=0; n<18; n++ ) maingrid->addColSpacing( n, 40 );
    for( int n=0; n<10; n++ ) maingrid->addRowSpacing( n, 40 );

    mainlayout->addStretch();

	//////////////////
	QWidget *SmallWidget = new QWidget( this );
	QVBoxLayout *dtabLayout = new QVBoxLayout( SmallWidget , 5 );
	dtab = new DetailedTab( SmallWidget );
	dtabLayout->addWidget( dtab );
	maingrid->addMultiCellWidget( SmallWidget ,2,3,3,6 );
}

void Kalzium::setupCaption()
{
	legend = new KalziumLegend(main_window);
    mainlayout->addWidget(legend);

    for (int n = 0; n < 18; n++)
    {
        labels[n] = new QLabel (main_window);
    }
}

void Kalzium::setupTimeline()
{
    QHBoxLayout *timeline_layout = new QHBoxLayout( mainlayout, -1, "timelinelayout" );
    timeline_layout->addStretch();

    dateS = new QSlider (QSlider::Horizontal, main_window, "rotateSlider" );
    timeline_layout->addWidget( dateS );
    QWhatsThis::add(dateS, i18n("Use this slider to see what elements were known at a certain date."));
    dateS->setRange(1669, 2003);
    dateS->setValue(2003);
    dateS->hide();
    connect( dateS, SIGNAL(valueChanged(int)),  SLOT(timeline()));

    timeline_layout->addSpacing( 10 );

    dateLCD = new QLCDNumber( 4, main_window, "dateLCD");
    timeline_layout->addWidget( dateLCD );

	QWhatsThis::add(dateLCD, i18n("This is the date you have chosen using the slider." ) );
    dateLCD->hide();
    dateLCD->display("2002");

    timeline_layout->addStretch();
}

void Kalzium::showIUPAC() const
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
        QWhatsThis::add(labels[n], i18n("This period is called '%1' in the terminology used by the IUPAC.").arg(name));
        labels[n]->show();
    }
}

void Kalzium::showOldIUPAC() const
{
    int h = 0, v = 0;
    for (int n = 0; n < 18; n++)
    {
        periodNrpos(n+1, h, v);
        QString name;
        periodOldName(n+1, name);
        maingrid->addWidget(labels[n], v/40, h/40);
        labels[n]->setText(name);
        labels[n]->setAlignment( Qt::AlignCenter );
        labels[n]->setAutoMask( true );
        QWhatsThis::add(labels[n], i18n("This period is called '%1' in the terminology of the old IUPAC-recommendation.").arg(name));
        labels[n]->show();
    }
}

void Kalzium::showCAS() const
{
    int h = 0, v = 0;
    for (int n = 0; n < 18; ++n)
    {
        periodNrpos(n+1, h, v);
        maingrid->addWidget(labels[n], v/40, h/40);
        labels[n]->setText(QString::number(n+1));
        labels[n]->setAlignment(AlignCenter);
        labels[n]->setAutoMask( true );
        QWhatsThis::add(labels[n], i18n("This period is called '%1' in the terminology used by the CAS.").arg(QString::number(n+1)));
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

void Kalzium::changeNumeration(const int id) const
{
    switch (id) {
	case 0:
	    showIUPAC();
	    break;
	case 1:
	    showCAS();
	    break;
	case 2:
	    showOldIUPAC();
	    break;
	case 3:
	    for (int n = 0; n < 18; ++n)
		labels[n]->hide();
	    break;
    }
}

void Kalzium::defineWeights() const
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

void Kalzium::newToolbarConfig()
{
    createGUI();
    applyMainWindowSettings( KGlobal::config(), autoSaveGroup() );
}

void Kalzium::optionsConfigureKeys()
{
  KKeyDialog::configure(actionCollection());
}

void Kalzium::optionsConfigureToolbars( )
{
    saveMainWindowSettings( KGlobal::config(), autoSaveGroup() );
    KEditToolbar dlg(actionCollection());
    connect(&dlg, SIGNAL(newToolbarConfig()), this, SLOT(newToolbarConfig()));
    dlg.exec();
}

void Kalzium::setFont()
{
  generalKPFont = KGlobalSettings::generalFont();
  generalKPBoldFont = generalKPFont;

  generalKPBoldFont.setBold(TRUE);
  generalKPFont.setPointSize(generalKPFont.pointSize()-2);
  generalKPBoldFont.setPointSize(generalKPBoldFont.pointSize()+1);
}

void Kalzium::showPseStyle(const int i)
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

void Kalzium::slotCalculations()
{
    if (!calculationdialog)
	{
		calculationdialog = new CalcDialog(this, "Calculationsdialog");
    	calculationdialog->show();
	}
}

void Kalzium::slotKnowledge()
{
    bool ok = FALSE;
    int numofquestions = QInputDialog::getInteger(
            i18n( "How Many Questions?" ),
            i18n( "Please enter a number:" ), 10, 0, 20, 1, &ok, this );
    if ( ok )
    {
        QuizDlg *quiz = new QuizDlg(this, "quizdialog", numofquestions);
        quiz->setCaption(i18n("Test Your Knowledge"));
        quiz->show();
    }
}

void Kalzium::slotShowAcidBeh() {
    delete templookup; templookup =NULL;
    static const char *field[] = {
        "acidic", "basic", "amphoteric", "neitherofthem"
    };

    main_config->setGroup("Colors");

    for (int i = 0; i < 109; ++i)
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

    for (int i = 83; i < 109; i++)
        element[i]->show();
    timelineToggleAction->setEnabled(true);
}

void Kalzium::slotShowBlocks()
{
    delete templookup; templookup =NULL;
    main_config->setGroup("Colors");
    for (int i = 0; i < 109; i++)
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
    delete templookup; templookup =NULL;
    main_config->setGroup("Colors");

    for (int i = 0; i < 109; ++i)
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
    static const int nummer[14] = { 2,10,18,21,28,31,32,36,43,49,54,55,72,75 };
    static const int dochda[4] = { 67,69,90,92 };

    for (int i=0; i<14 ; ++i)
        element[nummer[i] - 1]->hide();

    for (int i=83; i < 109; ++i)
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

    templookup->setButtonsColor();
    templookup->tempbeh();
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
		for (int i =0; i<109; i++)
			element[i]->show();

	}
}

void Kalzium::slotShowQuickinfo( bool id )
{
	if ( id == true )
	{
		showFastInfo = true;
		dtab->show();
	}
	else
	{
		showFastInfo = false;
		dtab->hide();
	}
}

void Kalzium::slotSearchData()
{
	SearchDlg *searchData = new SearchDlg(this, "SerachDlg");
	searchData->show();
}

void Kalzium::slotPlotData()
{
	KalziumPlotDialogImpl *PlotDialog = new KalziumPlotDialogImpl( this, "PlotDialog" );
	PlotDialog->show();
}

void Kalzium::slotShowLegend( bool id )
{
	if ( id == true )
		legend->show();
	else legend->hide();
}

void Kalzium::timeline()
{
    int currentDate = dateS->value();
	QString currentDateString = QString::number( currentDate );
	dateLCD->display( currentDate );
    for (int i = 0; i < 109; ++i)
    {
		PElementKP& b(element[i]);

		if (i+1 == 6 ||
			i+1 == 16 ||
			i+1 == 26 ||
			i+1 == 29 ||
			i+1 == 33 ||
			i+1 == 47 ||
			i+1 == 50 ||
			i+1 == 51 ||
			i+1 == 79 ||
			i+1 == 80 ||
			i+1 == 82 ||
			i+1 == 83
			)
		{
            b->show();
			continue;
		}

        if (b->Data.date > currentDateString)
            b->hide();
        else
            b->show();
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
	main_config->writeEntry("address", "http://www.ktf-split.hr/periodni/en/");
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
    const KIconLoader *kil = KGlobal::iconLoader();

    main_config->setGroup("Menu Settings");
    KStdAction::quit( kapp, SLOT (closeAllWindows()),actionCollection() );
    KStdAction::preferences(this, SLOT(showSettingsDialog()), actionCollection());
    KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()), actionCollection());
    KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()), actionCollection());
#if KDE_VERSION_MINOR >1 && KDE_VERSION_MAJOR >= 3
	createStandardStatusBarAction(); //post-KDE 3.1
#else
    setStandardToolBarMenuEnabled(true); //KDE 3.1
#endif

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
    colorschememenu = new KSelectAction(i18n("&Color Scheme"),0,actionCollection(), "colorscheme");
    colorschememenu->setItems(colorschemelist);
    connect(colorschememenu, SIGNAL(activated(int)), this, SLOT(updateColorMenu(int)));
    connect(colorschememenu, SIGNAL(activated(int)), this, SLOT(changeColorScheme(int)));
    connect(colorschememenu, SIGNAL(activated(int)), this, SLOT(changeTheLegend(int)));
    colorschememenu->setCurrentItem(main_config->readNumEntry("colorschememenu"));

    // BEGIN NUMERATIONMENU
    QStringList numerationlist;
    numerationlist.append( i18n("CAS"));
    numerationlist.append( i18n("IUPAC"));
    numerationlist.append( i18n("old IUPAC"));
    numerationlist.append( i18n("Off"));

    numerationmenu = new KSelectAction (i18n("&Numeration"),0,actionCollection(), "numeration");
    numerationmenu->setItems(numerationlist);

    connect(numerationmenu, SIGNAL(activated(int)), this, SLOT(updateNumMenu(int)));
    connect(numerationmenu, SIGNAL(activated(int)), this, SLOT(changeNumeration(int)));
    numerationmenu->setCurrentItem(main_config->readNumEntry("numerationmenu"));
    // END NUMERATIONMENU

    // BEGIN TIMELINE
    timelineToggleAction = new KToggleAction(i18n("Show &Timeline"),kil->iconPath("timeline", KIcon::User),0,actionCollection(), "timeline");
    timelineToggleAction->setChecked(true);
    connect(timelineToggleAction, SIGNAL(toggled(bool)), this, SLOT(slotShowTimeline(bool)));

    timelineToggleAction->setChecked(main_config->readBoolEntry("timelineshow"));
    // END TIMELINEMENU

	//BEGIN QUICKINFO
	quickinfoToggleAction = new KToggleAction( i18n( "Show &Quickinfo" ),kil->iconPath( "quickinfo",KIcon::User ),0,actionCollection(), "quickinfo" );
	quickinfoToggleAction->setChecked( true );
	connect( quickinfoToggleAction, SIGNAL( toggled( bool ) ),this, SLOT( slotShowQuickinfo( bool ) ) );
	quickinfoToggleAction->setChecked( main_config->readBoolEntry( "quickinfo" ) );
	//END QUICKINFO

	//BEGIN LEGEND
	legendToggleAction = new KToggleAction( i18n( "Show &Legend" ),kil->iconPath( "legend",KIcon::User ),0,actionCollection(), "legend" );
	legendToggleAction->setChecked( true );
	connect( legendToggleAction, SIGNAL( toggled( bool ) ),this, SLOT( slotShowLegend( bool ) ) );
	legendToggleAction->setChecked( main_config->readBoolEntry( "legend" ) );
	//END LEGEND

    (void) new KAction (i18n("Test Your &Knowledge"),0, this, SLOT(slotKnowledge()), actionCollection(), "test_your_knowledge");
    (void) new KAction (i18n("&Plot"),0, this, SLOT(slotPlotData()), actionCollection(), "startplotting");
    (void) new KAction (i18n("&Search Data"),0, this, SLOT(slotSearchData()), actionCollection(), "searchdata");
    (void) new KAction (i18n("&Calculations"),0, this, SLOT(slotCalculations()), actionCollection(), "calculations");
    (void) new KAction (i18n("Define Molecular &Weights"),0, this, SLOT(defineWeights()), actionCollection(), "defineweights");

    createGUI("kalziumui.rc");
}

void Kalzium::updateMainWindow()
{
	changeColorScheme(colorschememenu->currentItem());
	legend->changeLegend(colorschememenu->currentItem());
	changeNumeration(numerationmenu->currentItem());
	showPseStyle(psestylemenu->currentItem());
	slotShowTimeline(timelineToggleAction->isChecked());
	slotShowQuickinfo( quickinfoToggleAction->isChecked() );
	slotShowLegend( legendToggleAction->isChecked() );
}

void Kalzium::changeTheLegend(int tempId)
{
	legend->changeLegend(tempId);
}


