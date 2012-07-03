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

    property int minimumWidth: 200
    property int minimumHeight: 25

    property int leftMargin : 8
    property int topMargin: 8
    property int rightMargin: 8
    property int bottomMargin: 8

    property Component background:
    Component {
        id: defaultBackground
        Item {
            Rectangle {
                x: 1
                y: 1
                width: parent.width-2
                height: parent.height-2
                color: backgroundColor
                radius: 5
            }

            BorderImage {
                anchors.fill: parent
                id: backgroundimage
                smooth: true
                source: "../../images/lineedit_normal.png"
                border.left: 6; border.top: 6
                border.right: 50; border.bottom: 6
            }
        }
    }

    property Component up:
    Component {
        id: defaultUp
        Item {
            anchors.right: parent.right
            anchors.top: parent.top
            width: 24
            height: parent.height/2
            Image {
                anchors.left: parent.left;
                anchors.top: parent.top;
                anchors.topMargin: 7
                opacity: (upEnabled && enabled) ? (upPressed ? 1 : 0.8) : 0.3
                source: "../../images/spinbox_up.png"
            }
        }
    }

    property Component down:
    Component {
        id: defaultDown
        Item {
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: 24
            height: parent.height/2
            Image {
                anchors.left: parent.left;
                anchors.bottom: parent.bottom;
                anchors.bottomMargin: 7
                opacity: (downEnabled && enabled) ? (downPressed ? 1 : 0.8) : 0.3
                source: "../../images/spinbox_down.png"
            }
        }
    }
}
