/*
 * Simple dialog that displays a waiting message while the partitioning library is detecting devices.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.1

Rectangle
{
    width: 350
    height: 90
    color: "#DDDDDD"
    
    Text
    {
        anchors.fill: parent
        anchors.margins: 7
        
        color: "black"
        font.pointSize: 11
        
        text: "Loading device information.\nPlease wait..."
    }
}