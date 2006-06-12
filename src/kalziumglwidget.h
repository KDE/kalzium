#ifndef KALZIUMGLWIDGET_H
#define KALZIUMGLWIDGET_H
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

#include <QGLWidget>

#define SPHERE_TESSELATE_SLICES	30
#define SPHERE_TESSELATE_STACKS	30

#include <openbabel/mol.h>

/**
 * This class displays the 3D-view of a molecule
 * 
 * @autor Benoit Jacobs
 */
class KalziumGLWidget : public QGLWidget
{
	Q_OBJECT

	protected:
		GLuint sphereDisplayList;
	
/*		struct VertexArrayEntry {
			float nx, ny, nz, vx, vy, vz;
		};
*/

		bool isDragging;
		QPoint lastDraggingPosition;
		QPoint deltaDragging;
		GLdouble RotationMatrix[16];

	public:
		/**
		 * Constructor
		 */
		KalziumGLWidget( QWidget *parent = 0 );

		/**
		 * Destructor
		 */
		virtual ~KalziumGLWidget();

	public slots:
		/**
		 * sets the molecule which will be displayed
		 * @param molecule the molecule to render
		 */
		void slotSetMolecule( OpenBabel::OBMol* molecule );
		
		/**
		 * Sets the detail-grade in a range from 0 to 2
		 *  @param detail the detail-grade of the rendering. 0 is low, 2 is high
		 */
		void slotSetDetail( int detail );

	protected:
		/**
		 * This method initilized OpenGL
		 */
		virtual void initializeGL();
		virtual void paintGL();
		virtual void resizeGL( int width, int height );
		virtual void mousePressEvent( QMouseEvent * event );
		virtual void mouseReleaseEvent( QMouseEvent * event );
		virtual void mouseMoveEvent( QMouseEvent * event );

		/**
		 * This method...
		 */
		virtual void drawGenericSphere();
		
		/**
		 * This method...
		 * @param x
		 * @param y
		 * @param z
		 * @param radius
		 * @param red
		 * @param green
		 * @param blue
		 */
		virtual void drawSphere( 
				GLdouble x, 
				GLdouble y, 
				GLdouble z, 
				GLdouble radius,
				GLfloat red, 
				GLfloat green, 
				GLfloat blue );

		/**
		 * The molecule which is displayed
		 */
		OpenBabel::OBMol* m_molecule;

		/**
		 * The detail-grade from 0 to 2
		 */
		int m_detail;
};
#endif // KALZIUMGLWIDGET_H
