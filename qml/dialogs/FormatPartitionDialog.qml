/*
 * Dialog to get information for formatting a partition.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

import QtQuick 1.0
import ComboBox 1.0

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
    
    /* Unfortunately I haven't found a way to send the current state of all checkboxes with this signal yet. */
    signal closed(bool accepted, string filesystem, string partition)
    
    function show(partition) {
        formatDialog.partition = partition;
        dialogSet.state = "visible";
        formatDialog.opacity = 1;
        
        parent.width = 250;
        parent.height = 150;
    }
    
    function hide() {
        dialogSet.state = "invisible";
        formatDialog.opacity = 0;
    }
    
    Text {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 6
        
        text: "Select filesystem: "
        font {
            family: "Helvetica"
            pointSize: 12
        }
    }
    
    /* TODO: for now it holds dummy data. I'll connect a model later. */
    ComboBox {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        
        id: combobox
        objectName: "fsComboBox"
    }
    
    /* TODO: see before. The displayed flags/label edit should change according to the current filesystem. */
    FlagsList {
        id: flagsList
        objectName: "flagsList"
        
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
    }
        
    Row {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        spacing: 10
        
        GenericButton {
            text: "Ok"
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    formatDialog.hide();
                    formatDialog.closed(true, combobox.currentText, formatDialog.partition);
                }
            }
        }
        
        GenericButton {
            text: "Cancel"
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    formatDialog.hide();
                    formatDialog.closed(false, combobox.currentText, formatDialog.partition);
                }
            }
        }
    }
}