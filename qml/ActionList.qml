/*
 * ListView for registered actions.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.0

Rectangle
{
    id: actionListContainer
    width: window.width
    height: (window.height / 7) * 2
    
    border
    {
        width: 2
        color: "black"
    }
    
    ListView
    {
        id: actionListView
        model: actionModel
        
        height: actionListContainer.height
        width: actionListContainer.width - 100
        
        anchors.margins: 6
        
        delegate:
            Text
            {
                text: modelData
                font.pointSize: 10
            }
    }
}