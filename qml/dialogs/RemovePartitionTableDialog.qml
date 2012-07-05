/*
 * Confirmation dialog for when you remove a partition table.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.1
import ApplicationWidgets 1.0

Rectangle
{
    id: removeTableDialog
    objectName: "removeTableDialog"
    
    anchors.fill: parent
    color: parent.color
    width: parent.width
    height: parent.height
    opacity: 0
    
    property string disk
    signal closed(bool accepted, string disk)
    
    function show(disk)
    {
        removeTableDialog.disk = disk;
        dialogSet.state = "visible";
        removeTableDialog.opacity = 1;
        
        parent.width = 400;
        parent.height = 100;
    }
    
    function hide()
    {
        dialogSet.state = "invisible";
        removeTableDialog.opacity = 0;
    }
    
    Image
    {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        source: dialogIcon.iconPath("dialog-warning")
        
        Icon { id: dialogIcon }
    }
    
    Text
    {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        
        text: "Warning! This will erase all partitions and data on your disk.\nDo you want to continue?"
    }
    
    Row
    {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        spacing: 10
        
        GenericButton
        {
            text: "Ok"
            
            MouseArea
            {
                anchors.fill: parent
                onClicked: {                    
                    removeTableDialog.hide();
                    removeTableDialog.closed(true, removeTableDialog.disk);
                }
            }
        }
        
        GenericButton
        {
            text: "Cancel"
            
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    removeTableDialog.hide();
                    removeTableDialog.closed(false, removeTableDialog.disk);
                }
            }
        }
    }
}