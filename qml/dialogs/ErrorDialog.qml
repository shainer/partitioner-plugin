/*
 * Simple dialog that displays an error message

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
    id: errorDialog
    objectName: "errorDialog"
    
    anchors.fill: parent
    color: parent.color
    width: parent.width
    height: parent.height
    opacity: 0
    
    signal closed
    
    function show(message)
    {
        errorMessage.text = message;

        dialogSet.state = "visible";
        errorDialog.opacity = 1;
        
        parent.width = errorMessage.text.length * (errorMessage.font.pointSize);
        parent.height = 100;
    }
    
    function hide()
    {
        dialogSet.state = "invisible";
        errorDialog.opacity = 0;
    }
        
    Image
    {
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        source: dialogIcon.iconPath("dialog-error")
        
        Icon { id: dialogIcon }
    }
    
    Text
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        
        id: errorMessage
        font.pointSize: 9
    }
    
    GenericButton
    {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 5
        
        text: "Ok"
        
        MouseArea
        {
            anchors.fill: parent
            onClicked: {
                errorDialog.hide();
                errorDialog.closed();
            }
        }
    }
}