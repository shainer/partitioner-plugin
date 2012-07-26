/*
 * Dialog for resizing an existing partition.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.0
import "../widgets"

Rectangle
{
    id: resizeDialog
    objectName: "resizePartitionDialog"
    
    anchors.fill: parent
    color: parent.color
    width: parent.width
    height: parent.height
    opacity: 0
    
    property real size
    property real before
    property real after
    
    /* Unfortunately I haven't found a way to send the current state of all checkboxes with this signal yet. */
    signal closed(bool accepted, real newSize, real spaceBefore)
    
    function show()
    {
        dialogSet.state = "visible";
        resizeDialog.opacity = 1;
        
        partitionSizeBox.init(before, size, after)
        
        parent.width = 350;
        parent.height = 150;
    }
    
    function hide()
    {
        dialogSet.state = "invisible";
        resizeDialog.opacity = 0;
    }
    
    PartitionSizeBox
    {
        id: partitionSizeBox
        
        anchors.top: parent.top
        anchors.verticalCenter: parent.verticalCenter
        anchors.topMargin: 6
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
                    resizeDialog.hide();
                    resizeDialog.closed(true, partitionSizeBox.pSize, partitionSizeBox.bSize);
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
                    resizeDialog.hide();
                    resizeDialog.closed(false, 0.0, 0.0);
                }
            }
        }
    }
}