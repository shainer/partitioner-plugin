/*
 * Asking for confirmation when applying actions.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.0
import ApplicationWidgets 1.0

Rectangle
{
    id: applyConfirmationDialog
    objectName: "applyConfirmationDialog"
    
    anchors.fill: parent
    color: parent.color
    width: parent.width
    height: parent.height
    opacity: 0
    
    signal closed(bool confirmed)
    
    function show(unused)
    {
        dialogSet.state = "visible";
        applyConfirmationDialog.opacity = 1;
        
        parent.width = 800;
        parent.height = 200;
    }
    
    function hide()
    {
        dialogSet.state = "invisible";
        applyConfirmationDialog.opacity = 0;
    }
    
    Row
    {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 6
        spacing: 5
        
        Image
        {
            source: icon.iconPath("dialog-warning")
            Icon { id: icon }
        }
        
        Text
        {
            text: "Applying the following actions will change permanently your disks.\nAre you sure you want to continue?"
            font.pointSize: 9
        }
    }
    
    Rectangle
    {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 6
        
        color: "white"
        border.color: "black"
        border.width: 2
        
        width: parent.width - 12
        height: 100
        
        /* Displays the actions that will be applied */
        ListView
        {
            id: listView
            anchors.fill: parent
            anchors.margins: 6
            
            model: registeredActions
            delegate: Text { text: modelData }
        }
    }
    
    Row
    {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        spacing: 10
        
        GenericButton
        {
            text: "Apply"
            
            MouseArea
            {
                anchors.fill: parent
                onClicked: {                    
                    applyConfirmationDialog.hide();
                    applyConfirmationDialog.closed(true);
                }
            }
        }
        
        GenericButton
        {
            text: "Cancel"
            
            MouseArea
            {
                anchors.fill: parent
                onClicked: {
                    applyConfirmationDialog.hide();
                    applyConfirmationDialog.closed(false);
                }
            }
        }
    }
}