/*
 * ListView for the disk list.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.1

Rectangle
{
    id: diskList
    objectName: "diskList"
    
    width: (window.width * 1) / 4
    height: (window.height * 4) / 7
    
    border
    {
        width: 2
        color: "black"
    }
    
    ListView
    {
        id: diskListView
        objectName: "diskListView"
        
        model: diskModel
        highlight: SelectionRectangle {}
        
        anchors.fill: parent
        anchors.margins: 6
        
        delegate:
            Text
            {
                text: modelData
                font.family: "Helvetica"
                font.pointSize: 11
                lineHeight: 1.3
            
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: { diskListView.currentIndex = index; window.selectedDiskChanged(modelData) }
                }
            }
    }
}
