/*
 * Button Box: contains a row of buttons for actions

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.1

Rectangle
{    
    width: window.width
    height: 70
    
    border {
        width: 1
        color: "black"
    }
    
    Row
    {
        id: buttonrow
        objectName: "buttonrow"
        
        Repeater
        {
            id: repeater
            model: buttonBoxModel
            
            delegate:
                ActionButton {
                    text: buttonName
                    iconPath: iconName
                }
        }
    }
}
