/*
 * TreeView displaying a disk's layout.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.1

Rectangle
{
    id: deviceTree
    objectName: "deviceTree"
    
    width: (window.width * 3) / 4
    height: (window.height * 4) / 7
    
    border
    {
        width: 2
        color: "black"
    }
    
    ListView
    {
        id: deviceTreeView
        objectName: "deviceTreeView"
        
        model: deviceTreeModel
        highlight: SelectionRectangle {}
        delegate: deviceTreeDelegate
        
        anchors.fill: parent
        anchors.margins: 6
        
        onCurrentItemChanged: window.selectedDeviceChanged(model[currentIndex].deviceName)
    }
    
    Component
    {
        id: deviceTreeDelegate
        
        Rectangle
        {
            border.color: "#CCCCCC"
            color: "#00000000"
            width: deviceTree.width
            height: 30
            
            smooth: true
            radius: 2
            
            MouseArea
            {
                anchors.fill: parent
                onClicked: { deviceTreeView.currentIndex = index; window.selectedDeviceChanged(deviceName) }
            }
            
            Row
            {
                anchors.fill: parent
                anchors.margins: 6
                
                /*
                 * Indentation: partitions are indented from the disk, and logical partitions are even more indented
                 * to show their dependence on the extended one.
                 */
                Rectangle {
                    id: spacer1
                    color: "#00000000"
                    
                    height: 20
                    width: if (parentType == 1)
                             40
                           else if (deviceType != 0)
                             20
                           else
                             0
                }
                
                Text
                {
                    id: devname
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceDescription 
                    width: deviceTree.width / 3
                }
                
                Text
                {
                    id: devmp
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceMountPoint
                    
                    width: (deviceTree.width) / 4
                }
                
                /*
                 * This makes sure the following items are aligned correctly.
                 */
                Rectangle
                {
                    id: spacer2
                    color: "#00000000"
                    
                    height: 20
                    width: if (spacer1.width == 0)
                             40
                           else if (spacer1.width == 20)
                             20
                           else if (spacer1.width == 40)
                             0
                }
                
                Text
                {
                    id: devlabel
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceLabel
                    
                    width: deviceTree.width / 7
                }
                
                Text
                {
                    id: devfs
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceFileSystem
                    
                    width: deviceTree.width / 10
                }
                
                Text
                {
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