/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/
import Qt 4.7

QtObject {

    property variant minimumWidth: 100
    property variant minimumHeight: 32

    property Component groove: Component {
        Item {
            Rectangle {
                x: 1
                y: 1
                width: parent.width-2
                height: parent.height-2
                radius: 5
                color: backgroundColor
            }
            BorderImage {
                anchors.fill: parent
                id: backgroundimage
                smooth: true
                source: "../../images/lineedit_normal.png"
                border.left: 6; border.top: 3
                border.right: 6; border.bottom: 3
            }
            Text {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.Center
                width: parent.width/2
                font.pixelSize: 14
                font.bold: true
                color: "#aaa"
                text: "OFF"
                style: "Sunken"
                styleColor: "white"
                opacity: checked ? 0 : (enabled ? 1 : 0.5)
                Behavior on opacity { NumberAnimation { duration: 60 } }
            }
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width/2
                horizontalAlignment: Text.Center
                font.pixelSize: 14
                font.bold: true
                color: "#aaa"
                text: "ON"
                style: "Sunken"
                styleColor: "white"
                opacity: checked ? (enabled ? 1 : 0.5) : 0
                Behavior on opacity { NumberAnimation { duration: 60 } }
            }
        }
    }

    property Component handle: Component {
        BorderImage {
            width: 50
            height: 32
            smooth: true
            source: pressed ? "../../images/switch_pressed.png" : "../../images/switch_normal.png"

            border.left: 4; border.top: 4
            border.right: 4; border.bottom: 4
            Behavior on x {NumberAnimation{easing.type: Easing.OutCubic; duration:100}}
        }
    }
}
