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
    
    width: 260
    height: 150
    color: "#DDDDDD"
    opacity: 0
    
    smooth: true
    radius: 2
    
    border {
        color: "black"
        width: 2
    }
    
    states: [
        State {
            name: "visible"
            PropertyChanges { target: formatDialog; opacity: 1 }
        },
        
        State {
            name: "invisible"
            PropertyChanges { target: formatDialog; opacity: 0 }
        }
    ]
    state: "invisible"
    
    /* Adds a nice animation for when the dialog is shown or hidden */
    transitions: [
        Transition {
            from: "invisible"
            to: "visible"
            
            PropertyAnimation { target: formatDialog; property: "opacity"; to: 1; duration: 150 }
        },
        
        Transition {
            from: "visible"
            to: "invisible"
            
            PropertyAnimation { target: formatDialog; property: "opacity"; to: 0; duration: 150 }
        }
    ]
    
    property string partition : "none" /* this property must hold the partition considered for formatting */
    
    /* Unfortunately I haven't found a way to send the current state of all checkboxes with this signal yet. */
    signal closed(bool accepted, string filesystem, string partition)
    
    function show(partition) {
        formatDialog.partition = partition;
        formatDialog.state = "visible";
    }
    
    function hide() {
        formatDialog.state = "invisible";
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
    }
    
    /* TODO: see before. The displayed flags should change according to the current filesystem. */
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