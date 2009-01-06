/**********************************************************************
  painterengine - Class Description

  Copyright (C) 2007  <>

  This file is part of the Software Architecture Description Language
  (SADL) project.  For more information, see <http://milkbox.net/sadl/>

  SADL is free software; you can redistribute it and/or modify it under
  the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  SADL is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
  for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
 **********************************************************************/

#include "painterengine.h"
#include <avogadro/sphere.h>
#include <avogadro/cylinder.h>
#include <avogadro/textrenderer.h>
#include <avogadro/glwidget.h>
#include "global.h"
#include <QDebug>

namespace Avogadro
{


  class PainterEnginePrivate
  {
    public:
      PainterEnginePrivate() : widget ( 0 ), quality ( 0 ), spheres ( 0 ), cylinders ( 0 ),
          textRenderer ( new TextRenderer ), initialized ( false ), qualityChanged ( false ), sharing ( 0 )  {};
      ~PainterEnginePrivate()
      {
        deleteObjects();
        delete textRenderer;
      }

      GLWidget *widget;

      int newQuality;
      int quality;

      /** array of pointers to Spheres. You might ask, why not have
       * a plain array of Spheres. The idea is that more than one global detail level
       * may use a given sphere detail level. It is therefore interesting to be able
       * to share that sphere, instead of having redundant spheres in memory.
       */
      Sphere **spheres;
      /** array of pointers to Cylinders. You might ask, why not have
       * a plain array of Cylinders. The idea is that more than one global detail level
       * may use a given cylinder detail level. It is therefore interesting to be able
       * to share that cylinder, instead of having redundant cylinder in memory.
       */
      Cylinder **cylinders;

      TextRenderer *textRenderer;

      bool initialized;
      bool qualityChanged;

      void updateObjects();
      void deleteObjects();
      void createObjects();

      /**
       * PainterEngines can be shared, we must keep track of this.
       */
      int sharing;

  };

  void PainterEnginePrivate::deleteObjects()
  {
    int level, lastLevel, n;
    // delete the spheres. One has to be wary that more than one sphere
    // pointer may have the same value. One wants to avoid deleting twice the same sphere.
    if ( spheres )
    {
      lastLevel = -1;
      for ( n = 0; n < PAINTER_DETAIL_LEVELS; n++ )
      {
        level = PAINTER_SPHERES_LEVELS_ARRAY[quality][n];
        if ( level != lastLevel )
        {
          lastLevel = level;
          if ( spheres[n] )
          {
            delete spheres[n];
            spheres[n] = 0;
          }
        }
      }
      delete[] spheres;
      spheres = 0;
    }

    // delete the cylinders. One has to be wary that more than one cylinder
    // pointer may have the same value. One wants to avoid deleting twice the same cylinder.
    if ( cylinders )
    {
      lastLevel = -1;
      for ( n = 0; n < PAINTER_DETAIL_LEVELS; n++ )
      {
        level = PAINTER_CYLINDERS_LEVELS_ARRAY[quality][n];
        if ( level != lastLevel )
        {
          lastLevel = level;
          if ( cylinders[n] )
          {
            delete cylinders[n];
            cylinders[n] = 0;
          }
        }
      }
      delete[] cylinders;
      cylinders = 0;
    }
  }

  void PainterEnginePrivate::createObjects()
  {
    // create the spheres. More than one sphere detail level may have the same value.
    // in that case we want to reuse the corresponding sphere by just copying the pointer,
    // instead of creating redundant spheres.
    if ( spheres == 0 )
    {
      spheres = new Sphere*[PAINTER_DETAIL_LEVELS];
      int level, lastLevel;
      lastLevel = PAINTER_SPHERES_LEVELS_ARRAY[quality][0];
      spheres[0] = new Sphere ( lastLevel );
      for ( int n = 1; n < PAINTER_DETAIL_LEVELS; n++ )
      {
        level = PAINTER_SPHERES_LEVELS_ARRAY[quality][n];
        if ( level == lastLevel )
        {
          spheres[n] = spheres[n-1];
        }
        else
        {
          lastLevel = level;
          spheres[n] = new Sphere ( level );
        }
      }
    }

    // create the cylinders. More than one cylinder detail level may have the same value.
    // in that case we want to reuse the corresponding cylinder by just copying the pointer,
    // instead of creating redundant cylinders.
    if ( cylinders == 0 )
    {
      cylinders = new Cylinder*[PAINTER_DETAIL_LEVELS];
      int level, lastLevel;
      lastLevel = PAINTER_SPHERES_LEVELS_ARRAY[quality][0];
      cylinders[0] = new Cylinder ( lastLevel );
      for ( int n = 1; n < PAINTER_DETAIL_LEVELS; n++ )
      {
        level = PAINTER_CYLINDERS_LEVELS_ARRAY[quality][n];
        if ( level == lastLevel )
        {
          cylinders[n] = cylinders[n-1];
        }
        else
        {
          lastLevel = level;
          cylinders[n] = new Cylinder ( level );
        }
      }
    }
  }

  void PainterEnginePrivate::updateObjects()
  {
    if(newQuality != quality)
    {
      deleteObjects();
      quality = newQuality;
      createObjects();
    }
  }

  PainterEngine::PainterEngine( int quality ) : d ( new PainterEnginePrivate )
  {
    if ( quality < 0 || quality >= PAINTER_MAX_DETAIL_LEVEL )
    {
      quality = DEFAULT_GLOBAL_QUALITY_SETTING;
    }
    d->quality = quality;
  }

  PainterEngine::~PainterEngine()
  {
    delete d;
  }

//   void PainterEngine::setGLWidget( GLWidget * widget )
//   {
//     d->widget = widget;
//     d->textRenderer->setGLWidget(d->widget);
//   }
  //
  void PainterEngine::setQuality ( int quality )
  {
    assert ( quality >= 0 && quality < PAINTER_GLOBAL_QUALITY_SETTINGS );
    d->deleteObjects();
    d->quality = quality;
    d->qualityChanged = true;
  }

  int PainterEngine::quality() const
  {
    return d->quality;
  }

//   void PainterEngine::initialize( GLWidget * widget, int quality )
//   {
//     if(quality == -1) {
//       quality = DEFAULT_GLOBAL_QUALITY_SETTING;
//     }
//     else {
//       assert( quality >= 0 && quality < PAINTER_GLOBAL_QUALITY_SETTINGS );
//     }
//     d->initialized = true;
//     setGLWidget(widget);
//     setQuality(quality);
//   }

  void PainterEngine::drawSphere ( const Eigen::Vector3d & center, double radius, int detailLevel ) const
  {
    if ( d->textRenderer->isActive() )
    {
      d->textRenderer->end();
    }
    assert ( d->widget );
    assert ( detailLevel >= 0 && detailLevel <= PAINTER_MAX_DETAIL_LEVEL );
    d->spheres[detailLevel]->draw ( center, radius );
  }

  void PainterEngine::drawSphere ( const Eigen::Vector3d & center, double radius ) const
  {
    if ( d->textRenderer->isActive() )
    {
      d->textRenderer->end();
    }
    assert ( d->widget );
    Eigen::Vector3d transformedCenter = d->widget->camera()->modelview() * center;
    double distance = transformedCenter.norm();

    // perform a rough form of frustum culling
    double dot = transformedCenter.z() / distance;
    if ( dot > PAINTER_FRUSTUM_CULL_TRESHOLD ) return;

    double apparentRadius = radius / distance;

    int detailLevel = 1 + static_cast<int> ( floor (
                        PAINTER_SPHERES_DETAIL_COEFF * ( sqrt ( apparentRadius ) - PAINTER_SPHERES_SQRT_LIMIT_MIN_LEVEL )
                      ) );
    if ( detailLevel < 0 )
    {
      detailLevel = 0;
    }
    if ( detailLevel > PAINTER_MAX_DETAIL_LEVEL )
    {
      detailLevel = PAINTER_MAX_DETAIL_LEVEL;
    }
    d->spheres[detailLevel]->draw ( center, radius );
  }

  void PainterEngine::drawCylinder ( const Eigen::Vector3d &end1, const Eigen::Vector3d &end2,
                               double radius, int detailLevel ) const
  {
    if ( d->textRenderer->isActive() )
    {
      d->textRenderer->end();
    }
    assert ( d->widget );
    assert ( detailLevel >= 0 && detailLevel <= PAINTER_MAX_DETAIL_LEVEL );
    d->cylinders[detailLevel]->draw ( end1, end2, radius );
  }

  void PainterEngine::drawCylinder ( const Eigen::Vector3d &end1, const Eigen::Vector3d &end2,
                               double radius ) const
  {
    if ( d->textRenderer->isActive() )
    {
      d->textRenderer->end();
    }
    assert ( d->widget );
    Eigen::Vector3d transformedEnd1 = d->widget->camera()->modelview() * end1;
    double distance = transformedEnd1.norm();

    // perform a rough form of frustum culling
    double dot = transformedEnd1.z() / distance;
    if ( dot > PAINTER_FRUSTUM_CULL_TRESHOLD ) return;

    double apparentRadius = radius / distance;
    int detailLevel = 1 + static_cast<int> ( floor (
                        PAINTER_CYLINDERS_DETAIL_COEFF
                        * ( sqrt ( apparentRadius ) - PAINTER_CYLINDERS_SQRT_LIMIT_MIN_LEVEL )
                      ) );
    if ( detailLevel < 0 )
    {
      detailLevel = 0;
    }
    if ( detailLevel > PAINTER_MAX_DETAIL_LEVEL )
    {
      detailLevel = PAINTER_MAX_DETAIL_LEVEL;
    }
    d->cylinders[detailLevel]->draw ( end1, end2, radius );
  }

  void PainterEngine::drawMultiCylinder ( const Eigen::Vector3d &end1, const Eigen::Vector3d &end2,
                                    double radius, int order, double shift, int detailLevel ) const
  {
    if ( d->textRenderer->isActive() )
    {
      d->textRenderer->end();
    }
    assert ( d->widget );
    assert ( detailLevel >= 0 && detailLevel <= PAINTER_MAX_DETAIL_LEVEL );
    d->cylinders[detailLevel]->drawMulti ( end1, end2, radius, order,
                                           shift, d->widget->normalVector() );
  }

  void PainterEngine::drawMultiCylinder ( const Eigen::Vector3d &end1, const Eigen::Vector3d &end2,
                                    double radius, int order, double shift ) const
  {
    if ( d->textRenderer->isActive() )
    {
      d->textRenderer->end();
    }
    assert ( d->widget );
    Eigen::Vector3d transformedEnd1 = d->widget->camera()->modelview() * end1;
    double distance = transformedEnd1.norm();

    // perform a rough form of frustum culling
    double dot = transformedEnd1.z() / distance;
    if ( dot > PAINTER_FRUSTUM_CULL_TRESHOLD ) return;

    double apparentRadius = radius / distance;
    int detailLevel = 1 + static_cast<int> ( floor (
                        PAINTER_CYLINDERS_DETAIL_COEFF
                        * ( sqrt ( apparentRadius ) - PAINTER_CYLINDERS_SQRT_LIMIT_MIN_LEVEL )
                      ) );
    if ( detailLevel < 0 )
    {
      detailLevel = 0;
    }
    if ( detailLevel > PAINTER_MAX_DETAIL_LEVEL )
    {
      detailLevel = PAINTER_MAX_DETAIL_LEVEL;
    }
    d->cylinders[detailLevel]->drawMulti ( end1, end2, radius, order,
                                           shift, d->widget->normalVector() );
  }

  int PainterEngine::drawText ( int x, int y, const QString &string ) const
  {
    if ( !d->textRenderer->isActive() )
    {
      d->textRenderer->begin ( d->widget );
    }
    return d->textRenderer->draw ( x, y, string );
  }

  int PainterEngine::drawText ( const QPoint& pos, const QString &string ) const
  {
    if ( !d->textRenderer->isActive() )
    {
      d->textRenderer->begin( d->widget );
    }
    return d->textRenderer->draw ( pos.x(), pos.y(), string );
  }

  int PainterEngine::drawText ( const Eigen::Vector3d &pos, const QString &string ) const
  {
    if ( !d->textRenderer->isActive() )
    {
      d->textRenderer->begin ( d->widget );
    }
    Eigen::Vector3d transformedPos = d->widget->camera()->modelview() * pos;

    // perform a rough form of frustum culling
    double dot = transformedPos.z() / transformedPos.norm();
    if ( dot > PAINTER_FRUSTUM_CULL_TRESHOLD ) return 0;

    return d->textRenderer->draw ( pos, string );
  }

  void PainterEngine::begin ( GLWidget *widget )
  {
    if(!d->initialized)
    {
      qDebug() << "createObjects()";
      d->createObjects();
      d->initialized = true;
    }
    else if (d->qualityChanged)
    {
      qDebug() << "updateObjects()";
      d->updateObjects();
      d->qualityChanged = false;
    }

    d->widget = widget;
  }

  void PainterEngine::end()
  {
    if ( d->textRenderer->isActive() )
    {
      d->textRenderer->end();
    }
    d->widget = 0;
  }

  int PainterEngine::defaultQuality()
  {
    return DEFAULT_GLOBAL_QUALITY_SETTING;
  }

  int PainterEngine::maxQuality()
  {
    return PAINTER_GLOBAL_QUALITY_SETTINGS-1;
  }

  bool PainterEngine::isShared()
  {
    return d->sharing > 1;
  }

  void PainterEngine::incrementShare()
  {
    d->sharing++;
  }

  void PainterEngine::decrementShare()
  {
    d->sharing--;
  }

} // end namespace Avogadro

#include "painterengine.moc"
