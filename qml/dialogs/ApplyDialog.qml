/*
 * This dialog shows progress information while a list of actions is being executed.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.1
import "../widgets"

Rectangle
{
    id: applyDialog
    objectName: "applyDialog"
    
    anchors.fill: parent
    color: parent.color
    width: parent.width
    height: parent.height
    opacity: 0
    
    property int actionCount
    property string currentAction
    property int currentActionIndex: 0
    
    onCurrentActionChanged: {
        console.log("current action changed");
    }
    
    onCurrentActionIndexChanged: {
        console.log("current index changed");
    }
    
    signal closed
    
    function onProgressChanged(text, index)
    {
        applyDialog.currentAction = text;
        applyDialog.currentActionIndex = index;
    }
    
    function show()
    {
        dialogSet.state = "visible";
        applyDialog.opacity = 1;
        
        parent.width = 800;
        parent.height = 100;
    }
    
    function hide()
    {
        dialogSet.state = "invisible";
        applyDialog.opacity = 0;
    }
        
    Text
    {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        
        font.pointSize: 9
        font.bold: true
        text: applyDialog.currentAction
    }
    
    ProgressBar
    {
        id: progressBar
        value: (applyDialog.currentActionIndex / applyDialog.actionCount) * 100

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
    
    GenericButton
    {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: "Close"
        
        MouseArea
        {
            anchors.fill: parent
            onClicked: {                    
                applyDialog.hide();
                applyDialog.closed()
            }
        }
    }
}