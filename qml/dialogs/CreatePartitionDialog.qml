/*
 * Dialog for creating a new partition.

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
    id: createPartitionDialog
    objectName: "createPartitionDialog"
    
    anchors.fill: parent
    color: parent.color
    width: parent.width
    height: parent.height
    opacity: 0
    
    property string disk
    property variant acceptedPartitionTypes
    property variant supportedFilesystems
    property real freespace /* how many space is available for the partition in megabytes */
    
    /* TODO: this still isn't caught by anything so many parameters are missing */
    signal closed(bool accepted, string disk)
    
    function show(disk)
    {
        createPartitionDialog.disk = disk;
        dialogSet.state = "visible";
        createPartitionDialog.opacity = 1;
                
        /* Sets the items in the two comboboxes */
        typesComboBox.items = acceptedPartitionTypes;
        fsComboBox.items = supportedFilesystems;
        partitionSize.init(createPartitionDialog.freespace); /* sets the initial values for the spinboxes */
        
        parent.width = 450;
        parent.height = 270;
    }
    
    function hide()
    {
        dialogSet.state = "invisible";
        createPartitionDialog.opacity = 0;
    }
    
    /*
     * We cannot use a Grid here otherwise it would conflict with the PartitionSizeBox's layout.
     */
    Column
    {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        
        spacing: 5
        
        Row
        {
            spacing: 110 /* align with other rows */
            
            SpacedText { text: "Label: "; spacerHeight: 5 }
            
            Rectangle
            {            
                width: 150
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
                }
            }
        
        }
        
        PartitionSizeBox { id: partitionSize }
        
        Row
        {
            SpacedText { spacerHeight: 5 }
            SpacedText { spacerHeight: 5 }
        }
        
        Row
        {
            spacing: 5
            
            SpacedText { text: "Partition type: "; spacerHeight: 5 }
            ComboBox   { id: typesComboBox; width: 120 }

            SpacedText { text: "Filesystem: "; spacerHeight: 5 }
            ComboBox   { id: fsComboBox; width: 100 }
        }
    }

    /*
     * For some reason, probably due to how a delegate is displayed by QML, we cannot put this inside the previous
     * layout, as it doesn't fit properly. The model is set by the application before showing this dialog.
     */
    FlagsList
    {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottomMargin: 80
    }
    
    Row
    {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        spacing: 10
        
        GenericButton
        {
            text: "Ok"
            
            MouseArea {
                anchors.fill: parent
                onClicked: {                    
                    createPartitionDialog.hide();
                    createPartitionDialog.closed(true, createPartitionDialog.disk);
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
                    createPartitionDialog.hide();
                    createPartitionDialog.closed(false, createPartitionDialog.disk);
                }
            }
        }
    }
}