/*
 * Displays a list of flags with checkboxes to set/unset them

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

import QtQuick 1.1

Item {
    id: flagsList
    objectName: "flagsList"
    
    Column {
        
        Repeater {
            id: repeater
            objectName: "repeater"
            
            model: flagsModel
            delegate: flagsDelegate
        }
    }
    
    Component {
        id: flagsDelegate
        
        Row {
            id: row
            objectName: "row"
            
            spacing: 10
            
            CheckBox {
                id: checkbox
                objectName: "flagCheckBox"
                
                initialChecked: flagChecked
                width: 20
                height: 20
            }
            
            Text {
                objectName: "flagName"
                
                text: flagName
                font.pointSize: 10
            }
        }
    }
}