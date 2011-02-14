/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
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

#include "gradientwidget_impl.h"

#include <QTimer>

#include <kdebug.h>
#include <klocale.h>

#include <element.h>
#include <tempunit.h>
#include <math.h>

#include "kalziumdataobject.h"
#include "prefs.h"

// used to convert the double variables to int's. (slider <-> spinbox)
#define MULTIPLIKATOR 1000

GradientWidgetImpl::GradientWidgetImpl( QWidget *parent )
        : QWidget( parent ), m_play(false)
{
    setupUi( this );

    m_list = KalziumDataObject::instance()->ElementList;

    scheme_combo->addItems( KalziumElementProperty::instance()->schemeList() );
    gradient_combo->addItems( KalziumElementProperty::instance()->gradientList() );

    m_prevUnit = TempUnit::Kelvin;

    connect( gradient_spinbox, SIGNAL( valueChanged(double)), this, SLOT( doubleToSlider(double)));
    connect( gradient_slider, SIGNAL( valueChanged(int)), this, SLOT( intToSpinbox(int)));

    m_timer = new QTimer( this );
    connect( Play, SIGNAL (clicked()), this, SLOT( play()));
    connect( m_timer, SIGNAL(timeout()), this, SLOT( tick()) );

    Play->setIcon( KIcon( "media-playback-start" ) );
//X
//X     m_list = KalziumDataObject::instance()->ElementList;
//X
//X     m_htmlBegin = "";
//X     m_htmlEnd = "";
//X
//X     connect( time_box, SIGNAL( valueChanged( int ) ),
//X             this, SLOT( setNewTime( int ) ) );
}

GradientWidgetImpl::~GradientWidgetImpl()
{
    delete m_timer;
}

void GradientWidgetImpl::slotGradientChanged()
{
    if ( !gradient_slider->isEnabled() ) {
        gradient_spinbox->setEnabled(true);
        gradient_slider->setEnabled(true);
        Play->setEnabled(true);
    }

    KalziumElementProperty *elementProperty = KalziumElementProperty::instance();
    double dblMax = elementProperty->gradient()->maxValue();
    double dblMin = elementProperty->gradient()->minValue();

    // TODO a nice global unittype-settings assossiaton class.     // TODO make a universal unit class or lets chemicaldataobject have all units.
    // TODO Energy is also a settingoption. eV, kJ/mol
    QString unitSymbol = elementProperty->gradient()->unit();
    if (unitSymbol == QString("K")) {
        unitSymbol = TempUnit::unitListSymbol( Prefs::temperatureUnit() );
        dblMax = TempUnit::convert( dblMax, (int)TempUnit::Kelvin, Prefs::temperatureUnit());
        dblMin = TempUnit::convert( dblMin, (int)TempUnit::Kelvin, Prefs::temperatureUnit());
    }

    // setting the Slider
    const int intMax = dblMax * MULTIPLIKATOR;
    const int intMin = dblMin * MULTIPLIKATOR;

    // now we have the slider numbers, so put the speed to a adequate value.
    Speed->setMaximum( intMax / 100 );
    Speed->setValue( ( intMax / 100 ) / 2 );

    gradient_slider->setMaximum(intMax);
    gradient_slider->setMinimum(intMin);

    lblUnit->setText(unitSymbol);

    gradient_spinbox->setMaximum(dblMax);
    gradient_spinbox->setMinimum(dblMin);
    gradient_spinbox->setDecimals(elementProperty->gradient()->decimals());

    switch ( elementProperty->gradientId() ) {
    case KalziumElementProperty::DISCOVERYDATE:
        gradient_spinbox->setValue(dblMax);
        break;

    case KalziumElementProperty::SOMGradientType:
        gradient_spinbox->setValue(dblMin + 293);
        break;

    default:
        gradient_spinbox->setValue(dblMin);
        break;
    }

    // Disable Gradient widgets if no gradient is selected.
    if ( gradient_combo->currentIndex() == KalziumElementProperty::NOGRADIENT) {
        gradient_spinbox->setEnabled(false);
        gradient_slider->setEnabled(false);
        Play->setEnabled(false);
        text->clear();
    }
}



void GradientWidgetImpl::doubleToSlider(double var)
{
    //the signals need to be blocked as both will return to this slot. But no
    //matter which UI elements (slider oder spinbox) was changed, the other
    //has to be set to the same value

    gradient_slider->blockSignals( true );

    int intvar = var * MULTIPLIKATOR;

    gradient_slider->setValue(intvar);

    gradient_slider->blockSignals( false );


    QString unitSymbol = KalziumElementProperty::instance()->gradient()->unit();
    // converting back Temperature values to Kelvin.
    if (unitSymbol == QString("K")) {
        var = TempUnit::convert( var, Prefs::temperatureUnit(), (int)TempUnit::Kelvin);
    }

    emit gradientValueChanged( var );

    setNewValue( var );
}


void GradientWidgetImpl::intToSpinbox(int var)
{
    gradient_spinbox->blockSignals( true );

    double doublevar = var;
    doublevar = doublevar / MULTIPLIKATOR;

    gradient_spinbox->setValue(doublevar);

    gradient_spinbox->blockSignals( false );

    QString unitSymbol = KalziumElementProperty::instance()->gradient()->unit();
    // converting back Temperature values to Kelvin.
    if (unitSymbol == QString("K")) {
        doublevar = TempUnit::convert( doublevar, Prefs::temperatureUnit(), (int)TempUnit::Kelvin );
    }

    emit gradientValueChanged( doublevar );

    setNewValue( doublevar );
}


void GradientWidgetImpl::setNewValue( double newValue )
{
    // Info text currently only for State of mater typ available.
    if (gradient_combo->currentIndex() != KalziumElementProperty::SOMGradientType) {
        text->clear();
        return;
    }

    static const int threshold = 25;

    const QString unitSymbol = lblUnit->text();

    QStringList listMeltingPoint;
    QStringList listBoilingPoint;
    QStringList listBoilingPointValue;
    QStringList listMeltingPointValue;

    foreach (Element * element, m_list) {
        double melting = element->dataAsVariant( ChemicalDataObject::meltingpoint ).toDouble();
        if ( ( melting > 0.0 ) && fabs( melting - newValue ) <= threshold ) {
            listMeltingPoint << element->dataAsString( ChemicalDataObject::name );
            listMeltingPointValue << QString::number(TempUnit::convert(melting,(int)TempUnit::Kelvin,Prefs::temperatureUnit()));
        }

        double boiling = element->dataAsVariant( ChemicalDataObject::boilingpoint ).toDouble();
        if ( ( boiling > 0.0 ) && fabs( boiling - newValue ) <= threshold ) {
            listBoilingPoint << element->dataAsString( ChemicalDataObject::name );
            listBoilingPointValue << QString::number(TempUnit::convert(boiling,(int)TempUnit::Kelvin,Prefs::temperatureUnit()));
        }
    }
    QString htmlcode;
    if ( listMeltingPoint.count() > 0 ) {
        htmlcode += i18n( "Elements with melting point around this temperature:" ) + '\n';
        for ( int i = 0; i < listMeltingPoint.count(); i++ ) {
            htmlcode += " - " + i18nc( "For example: Carbon (300K)", "%1 (%2%3)",
                                       listMeltingPoint.at( i ), listMeltingPointValue.at( i ), unitSymbol ) + '\n';
        }
        htmlcode += '\n';
    } else {
        htmlcode += i18n( "No elements with a melting point around this temperature" );
        htmlcode += "\n\n";
    }
    if ( listBoilingPoint.count() > 0 ) {
        htmlcode += i18n( "Elements with boiling point around this temperature:" ) + '\n';
        for ( int i = 0; i < listBoilingPoint.count(); i++ ) {
            htmlcode += " - " + i18nc( "For example: Carbon (300K)", "%1 (%2%3)",
                                       listBoilingPoint.at( i ), listBoilingPointValue.at( i ), unitSymbol )  + '\n';
        }
        htmlcode += '\n';
    } else {
        htmlcode += i18n( "No elements with a boiling point around this temperature" );
        htmlcode += '\n';
    }

    text->setText( /*m_htmlBegin +*/ htmlcode /*+ m_htmlEnd*/ );
}



//X void TimeWidgetImpl::setNewTime( int newtime )
//X {
//this method is currently unused. That is because I have not
//yet entered the data for a contect-widget.
//I would like to have something like: Slidervalue on 1934, the
//html says "in 1994 ... happened"
//X     m_htmlBegin = "In the year ";
//X     m_htmlEnd = "!";
//X
//X     text->setText( m_htmlBegin + QString::number(newtime) + m_htmlEnd );
//X }



void GradientWidgetImpl::play(void)
{
    if ( m_play) {   //Currently playing
        //The Mode is 'Play' so stop
        stop();
        return;
    }

    //The mode is not 'play'
    //If the slider is at the maximum position bring it to the minimum
    if ((gradient_slider)->value() >= gradient_slider->maximum()) {
        gradient_slider->setValue ( gradient_slider->minimum () );
    }
    //start the timer at 200 milisecond time interval with single shot disabled
    m_timer->start( 200 );

    m_play = true;          //start playing
    Play->setIcon( KIcon( "media-playback-pause" ) );
}

void GradientWidgetImpl::stop(void)
{
    //Currently playing, stop the timer.
    m_timer -> stop();
    Play->setIcon( KIcon( "media-playback-start" ) );
    m_play = false;         //Stop
}

void GradientWidgetImpl::tick(void)
{
    int increment = Speed->value();
    int temp = gradient_slider->value();
    int max = gradient_slider->maximum();
    if (temp + increment > max)
        stop();
    gradient_slider -> setValue ( temp + increment );
}
// #include "gradientwidget_impl.moc"
