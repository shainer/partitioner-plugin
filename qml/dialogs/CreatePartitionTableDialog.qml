/*
 * Dialog for creating a new partition table on a disk.

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
    id: createTableDialog
    objectName: "createTableDialog"
    
    anchors.fill: parent
    color: parent.color
    width: parent.width
    height: parent.height
    opacity: 0
    
    property string disk
    property string currentScheme
    property string newScheme
    signal closed(bool accepted, string scheme, string disk)
    
    function show(disk)
    {
        createTableDialog.disk = disk;
        dialogSet.state = "visible";
        createTableDialog.opacity = 1;
        
        parent.width = 370;
        parent.height = 150;
        
        if (createTableDialog.currentScheme == "gpt") {
            gptButton.checked = true;
        }
        else {
            mbrButton.checked = true;
        }
    }
    
    function hide()
    {
        dialogSet.state = "invisible";
        createTableDialog.opacity = 0;
    }
    
    Row
    {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        spacing: 5

        Image
        {
            source: dialogIcon.iconPath("dialog-warning")
            
            Icon { id: dialogIcon }
        }
        
        Text
        {        
            text: "Choose a new partition table scheme for this disk.\n!Warning! this will delete any partition or data."
        }
    }
    
   Grid
   {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        columns: 2
        rows: 2
        spacing: 5
        
        RadioButton
        {
            id: mbrButton
            onCheckedChanged: if (checked) { gptButton.checked = false; createTableDialog.newScheme = "mbr"; }
        }
        
        Text { text: "mbr" }
        
        RadioButton
        {
            id: gptButton
            onCheckedChanged: if (checked) { mbrButton.checked = false; createTableDialog.newScheme = "gpt"; }
        }
        
        Text { text: "gpt" }
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
                    createTableDialog.hide();
                    createTableDialog.closed(true, createTableDialog.newScheme, createTableDialog.disk);
                }
            }
        }
        
        GenericButton
        {
            text: "Cancel"
            
            /* We don't really care about the info we sent if the user clicked Cancel */
            MouseArea
            {
                anchors.fill: parent
                onClicked: {
                    createTableDialog.hide();
                    createTableDialog.closed(false, createTableDialog.newScheme, createTableDialog.disk);
                }
            }
        }
    }
}