#include "informationdialog_impl.h"

#include <qbuttongroup.h>
#include <qdom.h>
#include <qfile.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qpoint.h>
#include <qradiobutton.h>
#include <qslider.h>
#include <qtabwidget.h>
#include <qtextbrowser.h>

#include <kdebug.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include <math.h>

#include "pse.h"
#include "informationdialog.h"
#include "element.h"

InformationWidget::InformationWidget( PSE *pse )
    : KDialogBase( Swallow, i18n( "Learn More About the Table of Elements" ), Help|Close, Close, 0L, "learn-dialog", false )
{
	m_pse = pse;
	m_infoDialog = new InformationDialog( this );
	setMainWidget( m_infoDialog );

	connect( m_infoDialog->time_slider, SIGNAL( sliderMoved(int) ), m_infoDialog->lCDNumber1, SLOT( display(int) ) );
	connect( m_infoDialog->time_slider, SIGNAL( valueChanged(int) ), this, SLOT( slotDate(int) ) );
	connect( m_infoDialog->time_slider, SIGNAL( valueChanged(int) ), m_infoDialog->lCDNumber1, SLOT( display(int) ) );
	connect( m_infoDialog->temp_slider, SIGNAL( sliderMoved(int) ), m_infoDialog->lCDNumber1_2, SLOT( display(int) ) );
	connect( m_infoDialog->temp_slider, SIGNAL( valueChanged(int) ), this, SLOT( slotTemp(int) ) );
	connect( m_infoDialog->temp_slider, SIGNAL( valueChanged(int) ), m_infoDialog->lCDNumber1_2, SLOT( display(int) ) );
	connect( m_infoDialog->tabWidget, SIGNAL( currentChanged(QWidget*) ), this , SLOT( tabSelected(QWidget*) ) );

	tabSelected( 0L );

	resize( 550, 400 );
}

void InformationWidget::slotClose()
{
	m_pse->setFullDraw();
	m_pse->setTimeline( false );
	m_pse->activateSOMMode( false );
	emit closed();
	accept();
}

QuizXMLParser::QuizXMLParser()
{
}

void InformationWidget::tabSelected( QWidget* /*w*/ )
{
	if ( m_infoDialog->tabWidget->currentPageIndex() == 0 )
		m_pse->setTimeline( true );
	else
		m_pse->setTimeline( false );

	if ( m_infoDialog->tabWidget->currentPageIndex() == 1 )
		m_pse->activateSOMMode( true );
	else
		m_pse->activateSOMMode( false );

	m_infoDialog->time_slider->setValue( 2000 );
	m_infoDialog->temp_slider->setValue( 295 );
	m_pse->setFullDraw();
	m_pse->update();
}

void InformationWidget::slotTemp( int temp )
{
	m_pse->setTemperature( temp );

	QString appBaseDir = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
	appBaseDir.append("kalzium/data/");
	appBaseDir.append("bg.jpg");
	QString htmlcode = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body background=\"" ;
	htmlcode += appBaseDir + "\">";

	static const int threshold = 25;

	EList kdo = m_pse->data()->ElementList;
	EList::Iterator it = kdo.begin();
	const EList::Iterator itEnd = kdo.end();

	QStringList listMeltingPoint;
	QStringList listBoilingPoint;
	QStringList listBoilingPointValue;
	QStringList listMeltingPointValue;
	for ( ; it != itEnd; ++it )
	{
		if ( fabs( ( *it )->melting() - temp ) <= threshold )
		{
			listMeltingPoint << ( *it )->elname();
			listMeltingPointValue << QString::number( ( *it )->melting() );
		}
		else if ( fabs( ( *it )->boiling() - temp ) <= threshold )
		{
			listBoilingPoint << ( *it )->elname().utf8();
			listBoilingPointValue << QString::number( ( *it )->boiling() );
		}
	}
	if ( listMeltingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with melting point around this temperature:" ) + "<br><ul type=\"disc\">";
		for ( uint i = 0; i < listMeltingPoint.count(); i++ )
		{
			htmlcode += i18n( "<li>%1</li> (%2K)" ).arg( listMeltingPoint[i] ).arg( listMeltingPointValue[i]);
		}
		htmlcode += "</ul><br>";
	}
	if ( listBoilingPoint.count() > 0 )
	{
		htmlcode += i18n( "Elements with boiling point around this temperature:" ) + "<br><ul type=\"disc\">";
		for ( uint i = 0; i < listBoilingPoint.count(); i++ )
		{
			htmlcode += i18n( "<li>%1</li> (%2K)" ).arg( listBoilingPoint[i] ).arg( listBoilingPointValue[i]);
		}
		htmlcode += "</ul><br>";
	}

	htmlcode += "</body></html>";

	m_infoDialog->m_explanation_3->setText( htmlcode );
}

void InformationWidget::slotDate(int date)
{
	m_pse->setDate( date );
}

void InformationWidget::showSOM()
{
	m_infoDialog->tabWidget->setCurrentPage( 0 );
}

void InformationWidget::showTimeline()
{
	m_infoDialog->tabWidget->setCurrentPage( 1 );
}

#include "informationdialog_impl.moc"
