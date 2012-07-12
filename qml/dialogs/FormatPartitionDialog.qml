/*
 * Dialog to get information for formatting a partition.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

import QtQuick 1.0
import ApplicationWidgets 1.0
import "../widgets"

Rectangle
{
    id: formatDialog
    objectName: "formatDialog"
    
    anchors.fill: parent
    color: parent.color
    width: parent.width
    height: parent.height
    opacity: 0
    
    property string partition /* this property must hold the partition considered for formatting */
    property variant supportedFilesystems
    
    /* Unfortunately I haven't found a way to send the current state of all checkboxes with this signal yet. */
    signal closed(bool accepted, string filesystem, string label, string uid, string gid, string partition)
    
    function show(partition)
    {
        formatDialog.partition = partition;
        dialogSet.state = "visible";
        formatDialog.opacity = 1;
        
        /* I need to set this here because the property supportedFilesystems has undefined type before explicit setting */
        fsComboBox.items = supportedFilesystems;
        
        parent.width = 500;
        parent.height = 150;
    }
    
    function hide()
    {
        dialogSet.state = "invisible";
        formatDialog.opacity = 0;
    }
    
    function activateInputs(labelActive, ownerActive)
    {
        labelInput.active = labelActive;
        uidInput.active = ownerActive;
        gidInput.active = ownerActive;
    }
            
    Text
    {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 6
        text: "Select filesystem: "
        
        font
        {
            family: "Helvetica"
            pointSize: 12
        }
    }
    
    ComboBox
    {
        id: fsComboBox
        objectName: "fsComboBox"
        
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 6
        anchors.topMargin: 30
        
        width: 130
    }
    
    Rectangle
    {
        width: 1
        height: 130
        color: "black"
        
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }
    
    Column
    {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 6
        spacing: 6
        
        Text
        {
            font
            {
                family: "Helvetica"
                pointSize: 12
            }
            
            text: "Filesystem properties: "
        }

        InputLine
        {
            id: labelInput
            displayedText: "Label: "
        }
        
        InputLine
        {
            id: uidInput
            displayedText: "Owner's UID: "
        }
        
        InputLine
        {
            id: gidInput
            displayedText: "Owner's GID: "
        }
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
                    formatDialog.hide();
                    formatDialog.closed(true,
                                        fsComboBox.currentText,
                                        labelInput.inputText,
                                        uidInput.inputText,
                                        gidInput.inputText,
                                        formatDialog.partition);
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
                    formatDialog.hide();
                    formatDialog.closed(false, "", "", "", "", formatDialog.partition);
                }
            }
        }
    }
}