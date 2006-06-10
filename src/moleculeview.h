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
	public:
		MoleculeDialog( QWidget  * parent );
		~MoleculeDialog();
};
#endif // MOLECULEVIEW_H
