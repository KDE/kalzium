#ifndef KALZIUMGLWIDGET_H
#define KALZIUMGLWIDGET_H
/***************************************************************************
    copyright            : (C) 2006 by Benoit Jacob
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kalziumglhelperclasses.h"

#include <QGLWidget>
#include <QList>
#include <QFont>

using namespace KalziumGLHelpers;

/**
 * This class displays the 3D-view of a molecule
 * 
 * @author Benoit Jacob
 */
class KalziumGLWidget : public QGLWidget
{
	Q_OBJECT

	protected:
		GLuint m_displayList;
		bool m_haveToRecompileDisplayList;

		TextRenderer m_textRenderer;

		/**
		 * The geometric model of the sphere (used for atoms).
		 */
		Sphere m_sphere;

		/**
		 * The geometric model of the cylinder (used for bonds).
		 */
		Cylinder m_cylinder;
		
		/**
		 * equals true if the user is currently dragging (rotating)
		 * the view
		 */
		bool m_isDragging;

		QPoint m_lastDraggingPosition;

		/**
		 * Stores the rotation that is applied to the model.
		 */
		GLdouble m_RotationMatrix[16];

		/**
		 * The molecule which is displayed
		 */
		OpenBabel::OBMol* m_molecule;

		/**
		 * approximate radius of the molecule,
		 * without the electrons.
		 */
		GLdouble m_molRadiusWithoutElectrons;

		/**
		 * The detail-grade from 0 to 2.
		 */
		int m_detail;

		/**
		 * Set this to true to enable the fog effect
		 */
		bool m_useFog;

		/**
		 * The selected atoms
		 */
		QList<OpenBabel::OBAtom*> m_selectedAtoms;

		/**
		 * The style in which the molecule is rendered
		 */
		MolStyle m_molStyle;

	public:
		/**
		 * Constructor
		 */
		KalziumGLWidget( QWidget *parent = 0 );

		/**
		 * Destructor
		 */
		virtual ~KalziumGLWidget();

		/**
		 * @return Returns a pointer to the molecule being worked on
		 */
		OpenBabel::OBMol* molecule () const { 
			return m_molecule; 
		}
	
	signals:
		/**
		 * the atoms in  @p atoms have been selected by the user
		 */
		void atomsSelected( QList<OpenBabel::OBAtom*> atoms );

	public slots:
		/**
		 * The autorotation timer ended so we have to move the molecule a bit more
		 * not needed I guess
		 */
		void rotate();
		
		/**
		 * sets the molecule which will be displayed
		 * @param molecule the molecule to render
		 */
		void slotSetMolecule( OpenBabel::OBMol* molecule );

		/**
		 * zoom in by 10%
		 */
		void slotZoomIn();
		
		/**
		 * zoom out by 10%
		 */
		void slotZoomOut();

		
		/**
		 * Sets the detail-grade in a range from 0 to 2
		 *  @param detail the detail-grade of the rendering. 0 is low, 2 is high
		 */
		void slotSetDetail( int detail );

		/**
		 * Sets the molecule style
		 * @param style the wanted molecule style
		 */
		void slotSetMolStyle( int style );

		/**
		 * The atoms @p atoms was selected by the user
		 */
		void slotAtomsSelected( QList<OpenBabel::OBAtom*> atoms );

		/**
		 * Activates the zoommode if @p zoom is true
		 */
		void slotZoom( bool zoom ){
			m_inZoom = zoom;
			if ( m_inZoom )
				m_inMeasure = false;
		}
		
		/**
		 * Activates the measuremode if @p measure is true
		 */
		void slotMeasure( bool measure ){
			m_inMeasure = measure;

			if ( m_inMeasure )
				m_inZoom = false;
		}

	protected:
		///if true the widget is in zoommode
		bool m_inZoom;
		
		///if true the widget is in zoommode
		bool m_inMeasure;
		
		/**
		 * This method initializes OpenGL. Automatically called by Qt
		 */
		virtual void initializeGL();

		/**
		 * This method does the painting. Automatically called by Qt
		 */
		virtual void paintGL();
		virtual void renderAtoms();
		virtual void renderBonds();
		virtual void renderSelection();
		virtual void FPSCounter();

		/**
		 * This method is called by Qt whenever the widget is resized.
		 */
		virtual void resizeGL( int width, int height );

		virtual void mousePressEvent( QMouseEvent * event );
		virtual void mouseReleaseEvent( QMouseEvent * event );
		virtual void mouseMoveEvent( QMouseEvent * event );

		/**
		 * This method is called by slotSetMolecule. It prepares the
		 * molecule for rendering, and computes some useful data about
		 * it.
		 */
		void prepareMoleculeData();

		double getMolRadius();

		void drawAtom( OpenBabel::OBAtom *atom );

		void drawBond( OpenBabel::OBBond *bond );

		/**
		 * recomputes the geometry of the geometric objects ( sphere,
		 * cylinder ).
		 */
		virtual void setupObjects();

		/**
		 * Sets the molecule style, but contrary to slotSetMolStyle,
		 * doesn't call setupObjects and updateGL. Useful for
		 * setting the initial style when the GL widget is created.
		 * Called by slotSetMolStyle.
		 * @param style the wanted molecule style
		 */
		void setMolStyle( int style );
};
#endif // KALZIUMGLWIDGET_H

