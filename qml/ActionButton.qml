/*
 * Single action button

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
    id: actionButton
    objectName: "actionButton"
    
    property string text: "Button"
    property string iconPath: "none"
    
    height: buttonbox.height
    width: buttonLabel.width + 37
    
    border
    {
        width: 2
        color: "gray"
    }
    
    color: "#99CCCC"
    smooth: true
    radius: 5
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        enabled: clickEnabled
        
        onClicked: window.actionButtonClicked(actionButton.text)
    }
    
    Column
    {
        id: layout
        anchors.fill: parent
        anchors.margins: 5
        spacing: 10
        
        Image
        {
            id: buttonImage
            source: buttonIcon.iconPath(iconName)
            anchors.horizontalCenter: parent.horizontalCenter
            
            Icon { id: buttonIcon }
        }
        
        Text
        {
            id: buttonLabel
            anchors.horizontalCenter: parent.horizontalCenter
            text: actionButton.text
            color: buttonTextColor
        }
    }
}
