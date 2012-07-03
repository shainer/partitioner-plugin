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

    property int minimumWidth: 32
    property int minimumHeight: 32

    property Component background:
    Component {
        Rectangle{
            border.color:"#333"
            width: 32
            height: 32
            anchors.centerIn: parent
            color: backgroundColor
            radius: width/2
        }
    }

    property Component checkmark: Component {
        Item {
            anchors.centerIn: parent
            Rectangle {
                width: 10
                height: 10
                anchors.centerIn: parent
                border.color:"#333"
                color: !parent.enabled ? "gray" : "black"
                radius: width/2
            }
        }
    }
}
