/**********************************************************************
  GlslPainter - painter using GLSL extensions

  Copyright (C) 2009 Marcus D. Hanwell

  This file is part of the Avogadro molecular editor project.
  For more information, see <http://avogadro.openmolecules.net/>

  Avogadro is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Avogadro is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
 **********************************************************************/

#ifndef GLSLPAINTER_H
#define GLSLPAINTER_H

#include "glpainter.h"

namespace Avogadro
{

  /**
   * @class GlslPainter glslpainter.h
   * @brief Implementation of the Painter class using OpenGL.
   * @author Marcus D. Hanwell
   *
   * This class implements the base Painter class using OpenGL. It is intended
   * to be used with the GLWidget to render molecules and other constructs to
   * an OpenGL context.
   *
   * @sa Painter
   */
  class GLWidget;

  class GlslPainter : public GLPainter
  {
    friend class GLWidget;
    public:

      /**
       * Constructor.
       * @param quality defaults to -1, valid range from 0 to 4.
       */
      explicit GlslPainter (int quality=-1);

      /**
       * Destructor.
       */
      ~GlslPainter();

      /**
       * Set the color to paint the OpenGL primitives with.
       * @param color the color to be used for painting.
      */
      void setColor (const Color *color);

      /**
       * Set the color to paint the OpenGL primitives with.
       * @param color the color to be used for painting.
      */
      void setColor (const QColor *color);

      /**
       * Set the color to paint elements with where 0.0 is the minimum and 1.0
       * is the maximum.
       * @param red component of the color.
       * @param green component of the color.
       * @param blue component of the color.
       * @param alpha component of the color.
      */
      void setColor (float red, float green, float blue, float alpha = 1.0);

      /**
       * Draws a sphere, leaving the Painter choose the appropriate detail level based on the
       * apparent radius (ratio of radius over distance) and the global quality setting.
       * @param center the position of the center of the sphere.
       * @param radius the radius of the sphere.
       */
      void drawSphere (const Eigen::Vector3d *center, float radius);

      /**
       * Draws a cylinder, leaving the Painter choose the appropriate detail level based on the
       * apparent radius (ratio of radius over distance) and the global quality setting.
       * @param end1 the position of the first end of the cylinder.
       * @param end2 the position of the second end of the cylinder.
       * @param radius the radius, i.e. half-width of the cylinder.
       */
      void drawCylinder (const Eigen::Vector3d &end1, const Eigen::Vector3d &end2,
                         double radius);

      /**
       * Draws a multiple cylinder (see below), leaving the Painter choose the appropriate
       * detail level based on the apparent radius (ratio of radius over distance) and the
       * global quality setting.
       *
       * What is a "multiple cylinder"? Think bond of order two or more between two atoms.
       * This function is here to allow drawing multiple bonds in a single call.
       *
       * This function takes care of rendering multiple bonds in such a way that the individual
       * bonds avoid hiding each other, at least in the defaut viewpoint of a molecule.
       * To achieves that, it asks the GLWidget for the the normal vector of the
       * molecule's best-fitting plane.
       *
       * @param end1 the position of the first end of the bond.
       * @param end2 the position of the second end of the bond.
       * @param radius the radius, i.e. half-width of each cylinder.
       * @param order the multiplicity order of the bond, e.g. 2 for a double bond.
       *              When this parameter equals 1, this function is equivalent to
       *              drawCylinder().
       * @param shift how far away from the central axis the cylinders are shifted.
       *              In other words this influences the total width of multiple bonds.
       */
      void drawMultiCylinder (const Eigen::Vector3d &end1, const Eigen::Vector3d &end2,
                               double radius, int order, double shift);

      /**
       * Set the Painter up for painting onto a GLWidget, should be called
       * before any painting.
       */
      void begin(GLWidget *widget);

      /**
       * End painting, should be called when all painting is complete.
       */
      void end();

      /**
       * @return true if the Painter is active.
       */
      bool isActive();

      /**
       * @return true if the Painter is being shared between multiple GLWidgets.
       */
      bool isShared();

      /**
       * @return the default quality level of the Painter.
       */
      static int defaultQuality();

      /**
       * @return the maximum quality level of the Painter.
       */
      static int maxQuality();

      /**
       * Set to true to turn dynamic object scaling on, false for off.
       */
      void setDynamicScaling(bool scaling);

    private:
      GLPainterPrivate * const d;

      bool m_dynamicScaling;

      /**
       * Increment the number of widgets the Painter is being shared by.
       */
      void incrementShare();

      /**
       * Decrement the number of widgets the Painter is being shared by.
       */
      void decrementShare();

      /**
       * Push the GL name and type, called internally before GL objects are
       * painted on the GLWidget.
       */
      void pushName();

      /**
       * Pop the GL name and type, called internally after GL objects are
       * painted on the GLWidget.
       */
      void popName();

      /**
       * Reset the GL name and type, called internally in popName() and also
       * whenever frustum culling determines that a GL object must not be
       * painted.
       */
      void resetName();
  };
} // end namespace Avogadro

#endif
