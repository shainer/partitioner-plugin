/*
 * Dialog for showing a warning about the absence of necessary filesystem tools.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.1
import ApplicationWidgets 1.0
import "../widgets"

Rectangle
{
    id: toolWarningDialog
    objectName: "toolWarningDialog"
    
    anchors.fill: parent
    color: parent.color
    width: parent.width
    height: parent.height
    opacity: 0
    
    function show()
    {
        dialogSet.state = "visible";
        toolWarningDialog.opacity = 1;
        
        parent.width = 450;
        parent.height = 130;
    }
    
    function hide()
    {
        dialogSet.state = "invisible";
        toolWarningDialog.opacity = 0;
    }
    
    Image
    {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5
        source: dialogIcon.iconPath("dialog-warning")
        
        Icon { id: dialogIcon }
    }
    
    Text
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        
        font.pointSize: 9
        text: "Some filesystem tools aren't installed. It is strongly suggested\nyou install them to be able to access all the features of this application."
    }
    
    GenericButton
    {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 5
        
        text: "Close"
        
        MouseArea
        {
            anchors.fill: parent
            
            onClicked: {
                toolWarningDialog.hide();
            }
        }
    }
}