// -*- coding: iso-8859-1 -*-
/*
 *   Author: etienne <etienne@none.org>
 *   Date: So. Jun 19 2011, 15:54:43
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *f
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 1.1
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts

 QGraphicsWidget{

    property int elementNumber: 1

    Item{

      PlasmaCore.DataSource {
	  id: dataSource
	  engine: "kalzium"
	  connectedSources: ['BlueObelisk:Element:' + elementNumber]
	  interval: 1000
      }


      MouseArea { id: mouseArea; anchors.fill: parent }


      Text {
	  anchors.leftMargin: 5
	  anchors.left: detailview.left
	  text: dataSource.data["BlueObelisk:Element:" + elementNumber]["name"]
	  font.pointSize: 24
      }

      Text {
	  id: idSymbol
	  anchors.horizontalCenter: detailview.horizontalCenter
	  anchors.verticalCenter: detailview.verticalCenter
	  text: dataSource.data["BlueObelisk:Element:" + elementNumber]["symbol"]
	  font.pointSize: 26;
	  font.bold: true
      }

      Text {
	  anchors.right: idSymbol.left
	  anchors.verticalCenter: detailview.verticalCenter
	  text: "<sub>" + elementNumber + "</sub>"
	  font.pointSize: 24;
      }

      Column {
	id: extraValues
	anchors.bottom: parent.bottom
	anchors.leftMargin: 20;
  //       anchors.rightMargin: 30;
	anchors.bottomMargin: 10;
	width: parent.width - 20

	Text {
	    anchors.right: parent.right
	    text: dataSource.data["BlueObelisk:Element:" + elementNumber]["en"]
	    font.pointSize: 16
	}

	Text {
	    anchors.right: parent.right
	    text: dataSource.data["BlueObelisk:Element:" + elementNumber]["mass"]
	    font.pointSize: 16
	}

	Text {
	    text: dataSource.data["BlueObelisk:Element:" + elementNumber]["electronconf"]
	}

      }

    }
//     Image {
// 	sourceSize.height: 150; sourceSize.width: 150
// 	height: 150; width: 150
// 
// 	anchors.right: parent.right
// 	anchors.bottom: parent.bottom
// 	visible: showSvg == true
//     	        fillMode: Image.PreserveAspectFit
//                     fillMode: Image.PreserveAspectCrop
// 	source:"../iconsets/school/" + atomicNumber + ".svg"
//     }
}

