/*
 * Simple progress bar

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

import QtQuick 1.1

Rectangle
{
    id: progressBar

    property real maximum: 100
    property real minimum: 0
    property real value: 0
    property int multiplier: 6 /* costant value to use for dimensions */
    
    onValueChanged: {
        if (value > maximum) {
            value = maximum;
        }
        
        progressText.text = value + "%";
    }
    
    color: "#00000000"
    smooth: true
    radius: 5
    
    border
    {
        color: "black"
        width: 1
    }
    
    width: maximum * multiplier
    height: 20
    
    Rectangle
    {

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 2
        
        color: "blue"
        height: parent.height - 2
        width: progressBar.value * progressBar.multiplier
        smooth: true
        radius: 5
        
        Text
        {
            id: progressText
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 4
            anchors.leftMargin: progressBar.width / 2
            
            color: "black"
            text: "0%"
        }
    }
}
