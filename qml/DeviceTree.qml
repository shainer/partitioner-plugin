/*
 * TreeView displaying a disk's layout.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.1

Rectangle {
    id: deviceTree
    width: (window.width * 2) / 3
    height: (window.height * 4) / 7
    
    border {
        width: 2
        color: "black"
    }
    
    ListView {
        id: deviceTreeView
        
        model: deviceTreeModel
        delegate: deviceTreeDelegate
        highlight: SelectionRectangle {}
        
        anchors.fill: parent
        anchors.margins: 6
    }
    
    Component {
        id: deviceTreeDelegate
        
        Rectangle {
            border.color: "#CCCCCC"
            width: deviceTree.width
            height: 30
            
            smooth: true
            radius: 2
            
            MouseArea {
                anchors.fill: parent
                onClicked: { deviceTreeView.currentIndex = index; window.selectedDeviceChanged(deviceName) }
            }
            
            Row {
                anchors.fill: parent
                anchors.margins: 6
                
                /*
                 * Indentation: partitions are indented from the disk, and logical partitions are even more indented
                 * to show their dependence on the extended one.
                 */
                Rectangle {
                    id: spacer1
                    color: "white"
                    
                    height: 20
                    width: if (partitionType == 2)
                             40
                           else if (deviceType != 0)
                             20
                           else
                             0
                }
                
                Text {
                    id: devname
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceName
                    
                    width: deviceTree.width / 2
                }
                
                Text {
                    id: devmp
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceMountPoint
                    
                    width: (deviceTree.width) / 5
                }
                
                /*
                 * This makes sure the following items are aligned correctly.
                 */
                Rectangle {
                    id: spacer2
                    color: "white"
                    
                    height: 20
                    width: if (spacer1.width == 0)
                             40
                           else if (spacer1.width == 20)
                             20
                           else if (spacer1.width == 40)
                             0
                }
                
                Text {
                    id: devfs
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceFileSystem
                    
                    width: deviceTree.width / 10
                }
                
                Text {
                    id: devsize
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceSize
                    
                    width: deviceTree.width / 5
                }
            }
        }
    }
}