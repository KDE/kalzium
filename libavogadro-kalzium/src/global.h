/**********************************************************************
  global.h - Setup some default defines.

  Copyright (C) 2007 by Donald Ephraim Curtis
  Copyright (C) 2008 by Marcus D. Hanwell

  This file is part of the Avogadro molecular editor project.
  For more information, see <http://avogadro.sourceforge.net/>

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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QTranslator>
#include <math.h>
#ifdef WIN32
# ifndef NOMINMAX
#  define NOMINMAX 1
# endif
#endif

// If we are using a recent GCC version with visibility support use it
#ifdef HAVE_GCC_VISIBILITY
  #define A_DECL_IMPORT __attribute__ ((visibility("default")))
  #define A_DECL_EXPORT __attribute__ ((visibility("default")))
  #define A_DECL_HIDDEN __attribute__ ((visibility("hidden")))
#elif defined(WIN32)
  #define A_DECL_IMPORT __declspec(dllimport)
  #define A_DECL_EXPORT __declspec(dllexport)
  #define A_DECL_HIDDEN
#else
  #define A_DECL_IMPORT
  #define A_DECL_EXPORT
  #define A_DECL_HIDDEN
#endif

// This macro should be used to export parts of the API
#ifndef A_EXPORT
  #ifdef MAKE_AVOGADRO_KALZIUM_LIB
    #define A_EXPORT A_DECL_EXPORT
  #else
    #define A_EXPORT A_DECL_IMPORT
  #endif
#endif

// This macro allows the selective hiding of parts of our exposed API
#ifndef A_HIDE
  #define A_HIDE A_DECL_HIDDEN
#endif

#ifndef GL_RESCALE_NORMAL
# define GL_RESCALE_NORMAL 0x803A
#endif

#ifndef GL_COLOR_SUM_EXT
# define GL_COLOR_SUM_EXT 0x8458
#endif

#ifndef GL_LIGHT_MODEL_COLOR_CONTROL_EXT
# define GL_LIGHT_MODEL_COLOR_CONTROL_EXT 0x81F8
#endif

#ifndef GL_SEPARATE_SPECULAR_COLOR_EXT
# define GL_SEPARATE_SPECULAR_COLOR_EXT 0x81FA
#endif

#ifndef GL_TEXTURE_RECTANGLE_ARB
# define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#endif

const double   ROTATION_SPEED                        = 0.005;
const double   ZOOM_SPEED                            = 0.02;
const double   MOUSE_WHEEL_SPEED                     = 0.1;
const double   CAMERA_MOL_RADIUS_MARGIN              = 10.0;
const double   CAMERA_NEAR_DISTANCE                  = 2.0;
const int      SEL_BUF_MAX_SIZE                      = 262144;
const int      SEL_BUF_MARGIN                        = 128;
const int      SEL_BOX_HALF_SIZE                     = 4;
const int      SEL_BOX_SIZE                          = 2 * SEL_BOX_HALF_SIZE + 1;
const double   SEL_ATOM_EXTRA_RADIUS                 = 0.18;
const double   SEL_BOND_EXTRA_RADIUS                 = 0.07;

const float    LIGHT_AMBIENT[4]                     = { 0.2f, 0.2f, 0.2f, 1.0f };

const float    LIGHT0_DIFFUSE[4]                     = { 1.0f, 1.0f, 1.0f, 1.0f };
const float    LIGHT0_SPECULAR[4]                    = { 1.0f, 1.0f, 1.0f, 1.0f };
const float    LIGHT0_POSITION[4]                    = { 0.8f, 0.7f, 1.0f, 0.0f };

const float    LIGHT1_DIFFUSE[4]                     = { 0.3f, 0.3f, 0.3f, 1.0f };
const float    LIGHT1_SPECULAR[4]                    = { 0.5f, 0.5f, 0.5f, 1.0f };
const float    LIGHT1_POSITION[4]                    = { -0.8f, 0.7f, -0.5f, 0.0f };


namespace Avogadro
{
  const int      PAINTER_GLOBAL_QUALITY_SETTINGS       = 5;
  const int      DEFAULT_GLOBAL_QUALITY_SETTING        = PAINTER_GLOBAL_QUALITY_SETTINGS - 3;
  const int      PAINTER_DETAIL_LEVELS                 = 10;
  // Sphere detail level array. Each row is a detail level.
  // The first column is the sphere detail level at the furthest
  // point and the last column is the detail level at the closest
  // point.
  const int      PAINTER_SPHERES_LEVELS_ARRAY[5][10]
  =
    { {0, 0, 1, 1, 2, 2, 3, 3, 4, 4},
      {0, 1, 2, 3, 4, 4, 5, 5, 6, 6},
      {1, 2, 3, 4, 5, 6, 7, 8, 9, 9},
      {1, 2, 3, 4, 6, 7, 8, 9, 11, 12},
      {2, 3, 4, 5, 7, 9, 12, 15, 18, 22}
    };
  const double   PAINTER_SPHERES_LIMIT_MIN_LEVEL       = 0.005;
  const double   PAINTER_SPHERES_LIMIT_MAX_LEVEL       = 0.15;

  // Cylinder detail level array. Each row is a detail level.
  // The first column is the cylinder detail level at the furthest
  // point and the last column is the detail level at the closest
  // point.
  const int      PAINTER_CYLINDERS_LEVELS_ARRAY[5][10]
  =
    { {0, 3, 5, 5, 8, 8, 12, 12, 16, 16},
      {0, 4, 6, 9, 12, 12, 16, 16, 20, 20},
      {0, 4, 6, 10, 14, 18, 22, 26, 32, 40},
      {0, 4, 6, 12, 16, 20, 24, 28, 34, 42},
      {0, 5, 10, 15, 20, 25, 30, 35, 40, 45}
    };
  const double   PAINTER_CYLINDERS_LIMIT_MIN_LEVEL     = 0.001;
  const double   PAINTER_CYLINDERS_LIMIT_MAX_LEVEL     = 0.03;
  const int      PAINTER_MAX_DETAIL_LEVEL = PAINTER_DETAIL_LEVELS - 1;
  const double   PAINTER_SPHERES_SQRT_LIMIT_MIN_LEVEL
  = sqrt ( PAINTER_SPHERES_LIMIT_MIN_LEVEL );
  const double   PAINTER_SPHERES_SQRT_LIMIT_MAX_LEVEL
  = sqrt ( PAINTER_SPHERES_LIMIT_MAX_LEVEL );
  const double   PAINTER_SPHERES_DETAIL_COEFF
  = static_cast<double> ( PAINTER_MAX_DETAIL_LEVEL - 1 )
    / ( PAINTER_SPHERES_SQRT_LIMIT_MAX_LEVEL - PAINTER_SPHERES_SQRT_LIMIT_MIN_LEVEL );
  const double   PAINTER_CYLINDERS_SQRT_LIMIT_MIN_LEVEL
  = sqrt ( PAINTER_CYLINDERS_LIMIT_MIN_LEVEL );
  const double   PAINTER_CYLINDERS_SQRT_LIMIT_MAX_LEVEL
  = sqrt ( PAINTER_CYLINDERS_LIMIT_MAX_LEVEL );
  const double   PAINTER_CYLINDERS_DETAIL_COEFF
  = static_cast<double> ( PAINTER_MAX_DETAIL_LEVEL - 1 )
    / ( PAINTER_CYLINDERS_SQRT_LIMIT_MAX_LEVEL - PAINTER_CYLINDERS_SQRT_LIMIT_MIN_LEVEL );
  const double   PAINTER_FRUSTUM_CULL_TRESHOLD = -0.8;

  class A_EXPORT Library
  {
    public:
      static QTranslator *createTranslator();
      static QString version();
      static QString svnRevision();
      static QString prefix();
      static bool threadedGL();
  };
}

#endif  // __GLOBAL_H
