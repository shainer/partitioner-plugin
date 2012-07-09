/*
 * This component is used to align text with other widgets in a Grid layout.
 * If no text is set it behaves like a normal spacer.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.0

Column
{
    property string text: ""
    property int spacerHeight
    property int spacerWidth: 1
    
    Rectangle
    {
        height: parent.spacerHeight
        width: parent.spacerWidth
        color: "#00000000" // transparency
    }
    
    Text
    {
        text: parent.text
    }
}