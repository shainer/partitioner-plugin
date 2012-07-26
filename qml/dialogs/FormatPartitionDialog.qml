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
    
    /* If this is true, we only register a change in the filesystem label, without reformatting everything */
    property bool onlySetLabel: labelCheckBox.checked
    property variant supportedFilesystems
    
    signal closed(bool accepted, bool onlySetLabel, string filesystem, string label, string uid, string gid)
    
    onOnlySetLabelChanged: {
        uidInput.active = !onlySetLabel;
        gidInput.active = !onlySetLabel;
        fsComboBox.enabled = !onlySetLabel;
    }
    
    function show()
    {
        dialogSet.state = "visible";
        formatDialog.opacity = 1;
        
        /* I need to set this here because the property supportedFilesystems has undefined type before explicit setting */
        fsComboBox.items = supportedFilesystems;
        
        parent.width = 500;
        parent.height = 170;
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
        id: selectFsText
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
        
        Row
        {
            spacing: 4
            
            CheckBox
            {
                id: labelCheckBox
                width: 20
                height: 20
                initialChecked: false
                
                onCheckedChanged: formatDialog.onlySetLabel = checked;
            }
            
            Text
            {
                text: "only set a new label"
            }
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
                                        formatDialog.onlySetLabel,
                                        fsComboBox.currentText,
                                        labelInput.inputText,
                                        uidInput.inputText,
                                        gidInput.inputText);
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
                    formatDialog.closed(false, false, "", "", "", "");
                }
            }
        }
    }
}