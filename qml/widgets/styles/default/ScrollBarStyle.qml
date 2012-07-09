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
    property Component content:
    Component {
        id: defaultContent
        Item {
            id: track
            Item {
                id: thumb
                x: horizontal ? Math.min (Math.max ((scrollItem.visibleArea.xPosition * scrollItem.width), 0), (track.width - width)) : 0
                y: horizontal ? 0 : Math.min (Math.max ((scrollItem.visibleArea.yPosition * scrollItem.height), 0), (track.height - height))
                width: horizontal ? scrollItem.visibleArea.widthRatio * scrollItem.width : 6
                height: horizontal ? 6 : (scrollItem.visibleArea.heightRatio * scrollItem.height) - 8
                Rectangle {
                    id: shadow
                    anchors.fill: thumb
                    radius: 3
                    color: "#000000"
                    opacity: 0.6
                }

                Rectangle {
                    height: thumb.height - 2
                    width: thumb.width - 2
                    anchors.centerIn: thumb
                    radius: 2
                    color: "#FFFFFF"
                }
            }
        }
    }
}
