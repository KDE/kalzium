#include "moleculeview.h"

MoleculeWidget::MoleculeWidget( QWidget * parent )
	: QGLWidget( parent )
{
}

MoleculeWidget::~MoleculeWidget()
{
}
		
void MoleculeWidget::initializeGL()
{
//X 	qglClearColor( Qt::white );
//X 	glShadeModel( GL_FLAT );
//X 	glEnable( GL_DEPTH_TEST );
//X 	glEnable( GL_CULL_FACE );
}

void MoleculeWidget::paintGL()
{
//X 	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//X 	glLoadIdentity();
//X 
}

void MoleculeWidget::resizeGL( int width, int height )
{
//X 	int side = qMin( width, height );
//X 	glViewport( ( width - side ) / 2, ( height - side ) / 2, side, side );
//X 
//X 	glMatrixMode( GL_PROJECTION );
//X 	glLoadIdentity();
//X 	glOrtho( -0.5, +0.5, +0.5, -0.5, 4.0, 15.0 );
//X 	glMatrixMode( GL_MODELVIEW );
//X 
}

MoleculeDialog::MoleculeDialog( QWidget * parent )
	: KDialog( parent )
{
}

MoleculeDialog::~MoleculeDialog( )
{
}

#include "moleculeview.moc"
