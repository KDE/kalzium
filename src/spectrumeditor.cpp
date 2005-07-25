#include "spectrumeditor.h"
#include "spectrum.h"
#include "spectrumwidget.h"
#include "exporter.h"
#include "bandeditor.h"

#include <qlayout.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qpopupmenu.h>

#include <kaction.h>
#include <kactioncollection.h>
#include <kglobal.h>
#include <kguiitem.h>
#include <kpushbutton.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kdebug.h>
#include <knuminput.h>
#include <klineedit.h>

SpectrumEditor::SpectrumEditor( QWidget *parent, const char* name )
	: KDialogBase(parent, name, true, i18n( "Spectrum Editor" ),
			KDialogBase::User1|KDialogBase::Close|KDialogBase::Help, KDialogBase::NoDefault, true, KGuiItem(i18n( "Export Spectrum..." )) )
{
	QWidget *page = new QWidget( this, "page" );
	setMainWidget( page );
	
	QVBoxLayout *spectrumLayout = new QVBoxLayout(  page , 0, KDialogBase::spacingHint() );
	
	QHBoxLayout *hbox = new QHBoxLayout(  page, 0, KDialog::spacingHint(), "hbox" );
	QHBoxLayout *hbox1 = new QHBoxLayout( page, 0, KDialog::spacingHint(), "hbox1" );
	QHBoxLayout *hbox2 = new QHBoxLayout( page, 0, KDialog::spacingHint(), "hbox2" );
	
	m_spinbox_left = new QSpinBox( 100, 1000, 1,  page );
	m_spinbox_right = new QSpinBox( 100, 1000, 1, page );

	m_bandEditor = new bandEditor( page, "m_bandEditor" );
	
	m_spectrumWidget = new SpectrumWidget( page, "sw" );
	spectrumLayout->addWidget( m_spectrumWidget );
	Spectrum *spectrum = new Spectrum();
	m_spectrumWidget->setSpectrum( spectrum );
	
	m_spinbox_left->setValue( 100 );
	m_spinbox_right->setValue( 1000 );
	m_spectrumWidget->setBorders( 100, 1000 );
	
	hbox->addWidget( new QLabel( i18n( "Minimum Value:" ), page ) );
	hbox->addWidget( m_spinbox_left );
	hbox->addWidget( new QLabel( i18n( "Maximum Value:" ), page ) );
	hbox->addWidget( m_spinbox_right );
	
	spectrumLayout->addWidget( m_bandEditor );
	
	spectrumLayout->addLayout( hbox );
	spectrumLayout->addLayout( hbox1 );
	spectrumLayout->addLayout( hbox2 );

	setMinimumSize( 500, 450 );
	
	connect( m_spinbox_right, SIGNAL( valueChanged( int ) ), m_spectrumWidget, SLOT( setRightBorder( int ) ) );
	connect( m_spinbox_left, SIGNAL( valueChanged( int ) ), m_spectrumWidget, SLOT( setLeftBorder( int ) ) );
	
	connect( m_bandEditor->addButton, SIGNAL( clicked() ), this, SLOT( slotAddBand( ) ) );
}

void SpectrumEditor::slotAddBand()
{
	double value = m_bandEditor->wl->value();

	if ( !value > 0.0 ) return; //invalid wavelength

	double e1 = m_bandEditor->e1->value();
	double e2 = m_bandEditor->e2->value();
	double aki = m_bandEditor->aki->value();

	QString J1 = m_bandEditor->j1->text();
	QString J2 = m_bandEditor->j2->text();
	QString t1 = m_bandEditor->t1->text();
	QString t2 = m_bandEditor->t2->text();
	int intensity = m_bandEditor->intensity->value();
	
	Spectrum::band band;

	band.wavelength = value;
	band.aki = aki;
	band.energy2= e2;
	band.energy1= e1;
	band.intensity = intensity;
	band.term2 = t2;
	band.term1 = t1;
	band.J1 = J1;
	band.J2 = J2;
	band.intensity = intensity;
	
	m_spectrumWidget->spectrum()->addBand( band );
	m_spectrumWidget->update();
}

void SpectrumEditor::slotUser1()
{
	Exporter* exporter = new Exporter();
	QString fileName = KFileDialog::getSaveFileName(
	                        QString::null, exporter->supportedImageFormats(),
	                        this, i18n( "Save Spectrum" ) );
	if( !fileName.isEmpty() )
	{
		if ( !exporter->saveAsImage( &m_spectrumWidget->pixmap(), fileName ) )
			KMessageBox::error( this, i18n( "The spectrum could not be saved" ), i18n( "Image could not be saved") );
	}	
	delete exporter;
}

#include "spectrumeditor.moc"
