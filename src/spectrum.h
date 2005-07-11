#include <qwidget.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qimage.h>
#include <qstring.h>
#include <qvaluelist.h>

#include <kimageeffect.h>
#include <kdebug.h>
#include <kpixmapeffect.h>


class SpectrumWidget : public QWidget
{
	Q_OBJECT

	public:
		SpectrumWidget( QWidget *parent, const char* name = 0 );
		~SpectrumWidget();

		void setSpectra( QValueList<double> l ){
			m_spectra = l;
		}
	
	private:
		QValueList<double> m_spectra;

		/**
		 * draws the spectra-lines
		 */
		void drawLines( QPainter *p );

		/**
		 * @returns the color of a line
		 * @param spectrum the value of the spectrum
		 */
		QColor linecolor( double spectrum );

	protected:
		virtual void paintEvent( QPaintEvent *e );
};
