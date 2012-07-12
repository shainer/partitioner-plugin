/*
 * This widget displays a text input next to some text.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.1

Row
{
    spacing: 6
    id: filesystemProperty
    property string displayedText
    property string inputText: input.text
    property bool active: false
    
    Text
    {
        text: filesystemProperty.displayedText
        color: (filesystemProperty.active) ? "black" : "gray"
    }
    
    Rectangle
    {
        height: 18
        width: 150
        color: "white"
        
        border
        {
            color: "black"
            width: 1
        }
        
        TextInput
        {
            id: input
            anchors.fill: parent
            anchors.margins: 3
            enabled: filesystemProperty.active
        }
    }
}