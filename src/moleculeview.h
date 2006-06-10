#ifndef MOLECULEVIEW_H
#define MOLECULEVIEW_H

#include <kdialog.h>
#include <QGLWidget>

class MoleculeWidget : public QGLWidget
{
	Q_OBJECT

	public:
		MoleculeWidget( QWidget *parent = 0 );
		virtual ~MoleculeWidget();

	protected:
		void initializeGL();
		void paintGL();
		void resizeGL( int width, int height );

	private:
		  GLuint object;
};

class MoleculeDialog : public KDialog
{
	MoleculeDialog( QWidget  * parent );
	~MoleculeDialog();
};
#endif // MOLECULEVIEW_H
