/*
 * Sets the properties and layout of the main partitioning window.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.1
import "./dialogs" as Dialogs

Rectangle
{
    id: window
    color: "#DDDDDD"
    
    /*
     * NOTE: signals are reported here so we can connect easily to them in the main application without
     * having to search for objects deeply into the object tree.
     */
    signal selectedDiskChanged (string newDisk)
    signal selectedDeviceChanged (string newDevice)
    signal actionButtonClicked (string actionName)
    
    Column
    {
        spacing: 10
        objectName: "mainColumn"
        
        ButtonBox { id: buttonbox }
        
        Row
        {
            spacing: 7
            objectName: "mainRow"
            
            DiskList { id: diskList }
            DeviceTree { id: deviceTree }
        }
        
        ActionList { id: actionList }
    }
    
    /* Stores all the hidden dialogs, to be called when necessary */
    Dialogs.DialogSet {}
}