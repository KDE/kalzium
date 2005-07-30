#include "timewidget_impl.h"

#include <qslider.h>
#include <qtextedit.h>
#include <qvaluelist.h>

#include <kdebug.h>
#include <knuminput.h>
#include <klocale.h>

#include <math.h>

#include "element.h"
#include "kalziumdataobject.h"

TimeWidgetIMPL::TimeWidgetIMPL( QWidget *parent, const char* name )
	: TimeWidget( parent, name )
{
}

#include "timewidget_impl.moc"
