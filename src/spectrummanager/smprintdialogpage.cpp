#include <q3buttongroup.h>

#include <QRadioButton>
#include <QLayout>
#include <QGridLayout>

#include <klocale.h>

#include "smprintdialogpage.h"

SMPrintDialogPage::SMPrintDialogPage(QWidget *parent )
 : KPrintDialogPage( parent )
{
//X   setTitle(i18n("Vocabulary Options"));
//X   
//X   QGridLayout * l = new QGridLayout( this, 1, 1, 11, 6);
//X 
//X   g = new Q3ButtonGroup(i18n("Select Type of Printout"), this );
//X   g->setColumnLayout(0, Qt::Vertical );
//X   g->layout()->setSpacing( 6 );
//X   g->layout()->setMargin( 11 );
//X   QGridLayout * v = new QGridLayout( g->layout() );
//X   v->setAlignment( Qt::AlignTop );
//X 
//X   rb0 = new QRadioButton(i18n("Vocabulary &list"), g );
//X   v->addWidget( rb0, 0, 0 );
//X   rb1 = new QRadioButton(i18n("Vocabulary e&xam"), g);
//X   v->addWidget( rb1, 1, 0 );
//X   rb2 = new QRadioButton(i18n("&Flashcards"), g);
//X   v->addWidget( rb2, 2, 0 );
//X   l->addWidget( g, 0, 0 );
//X   
//X   g->setWhatsThis( i18n("Specify type of printout to make"));
//X   rb0->setWhatsThis( i18n("Select to print the vocabulary as displayed in the editor"));
//X   rb1->setWhatsThis( i18n("Select to print the vocabulary as a vocabulary exam"));
//X   rb2->setWhatsThis( i18n("Select to print flashcards"));
}


SMPrintDialogPage::~SMPrintDialogPage()
{
}

void SMPrintDialogPage::getOptions(QMap< QString, QString > & opts, bool incldef)
{
//X   opts[ "kde-kwordquiz-type" ] = QString::number(g->selectedId());
}

void SMPrintDialogPage::setOptions(const QMap< QString, QString > & opts )
{
//X   QString type = opts[ "kde-kwordquiz-type" ];
//X   switch (type.toInt())
//X   {
//X     case 0: rb0->setChecked(true); break;
//X     case 1: rb1->setChecked(true); break;
//X     case 2: rb2->setChecked(true); break;
//X   }
}

bool SMPrintDialogPage::isValid(const QString & msg )
{
  return true;
}


#include "smprintdialogpage.moc"
