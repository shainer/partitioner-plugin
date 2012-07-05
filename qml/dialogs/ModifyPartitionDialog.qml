/*
 * Dialog that gets information for modifying some partition's properties.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.0

Rectangle
{
    id: modifyDialog
    objectName: "modifyDialog"
     
    anchors.fill: parent
    color: parent.color
    width: parent.width
    height: parent.height
    opacity: 0

    property string currentLabel
    property string partition /* this property must hold the partition to be modified */
    
    /* Unfortunately I haven't found a way to send the current state of all checkboxes with this signal yet. */
    signal closed(bool accepted, string label, string partition)
    
    function show(partition)
    {
        modifyDialog.partition = partition;
        dialogSet.state = "visible";
        modifyDialog.opacity = 1;
        
        parent.width = 250;
        parent.height = 150;
    }
    
    function hide()
    {
        dialogSet.state = "invisible";
        modifyDialog.opacity = 0;
    }
    
    Text
    {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 6
        anchors.topMargin: 12
        text: "Enter new label: "
    }
    
    Rectangle
    {        
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 6
        anchors.topMargin: 10
        
        width: 100
        height: 20
        color: "white"
        border.color: "black"
        border.width: 1
        
        smooth: true
        radius: 3
        
        TextInput
        {
            id: labelInput
            
            anchors.fill: parent
            anchors.margins: 3
            text: currentLabel /* initially we show the current partition's label */
        }
    }
    
    FlagsList
    {
        anchors.left: parent.left
        anchors.leftMargin: 6
        anchors.verticalCenter: parent.verticalCenter
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
                    modifyDialog.hide();
                    modifyDialog.closed(true, labelInput.text, modifyDialog.partition);
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
                    modifyDialog.hide();
                    modifyDialog.closed(false, labelInput.text, modifyDialog.partition);
                }
            }
        }
    }
}