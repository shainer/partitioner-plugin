/*
 * This is a widget that holds all the application dialogs and their common properties.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.0

Rectangle
{
    id: dialogSet
    objectName: "dialogSet"
    
    anchors.verticalCenter: parent.verticalCenter
    anchors.horizontalCenter: parent.horizontalCenter
    
    color: "#DDDDDD"
    opacity: 0
    
    smooth: true
    radius: 2
    
    border
    {
        color: "black"
        width: 2
    }
    
    states: [
        State
        {
            name: "visible"
            PropertyChanges { target: dialogSet; opacity: 1 }
        },
        
        State 
        {
            name: "invisible"
            PropertyChanges { target: dialogSet; opacity: 0 }
        }
    ]
    state: "invisible"
    
    /* Adds a nice animation for when the dialog is shown or hidden */
    transitions: [
        Transition 
        {
            from: "invisible"
            to: "visible"
            
            PropertyAnimation { target: dialogSet; property: "opacity"; to: 1; duration: 150 }
        },
        
        Transition
        {
            from: "visible"
            to: "invisible"
            
            PropertyAnimation { target: dialogSet; property: "opacity"; to: 0; duration: 150 }
        }
    ]
    
    FormatPartitionDialog {}
    ModifyPartitionDialog {}
    CreatePartitionTableDialog {}
    RemovePartitionTableDialog {}
    CreatePartitionDialog {}
    ResizePartitionDialog {}
    ApplyConfirmationDialog {}
    ApplyDialog {}
    ErrorDialog {}
    ToolWarningDialog {}
}