/*
 * Simple item for the rectangle underlying selections in ListView.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.1

Rectangle
{
    id: selectionRectangle
    objectName: "selectionRectangle"
    
    color: "lightsteelblue"
    smooth: true
    radius: 5
}