/*
 * This widget displays three spinboxes to set a partition's size, and implicitly the partition's offset too, knowing how
 * many free space there is before and after it.
 * NOTE: all sizes are in megabytes.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
import QtQuick 1.0
import "../widgets"

Grid
{
    columns: 2
    rows: 3
    spacing: 5
    
    property real pSize: partitionSize.value
    property real bSize: spaceBefore.value
    
    /* Sets initial values for spinboxes */
    function init(bvalue, value, avalue)
    {
        var spaceSum = bvalue + value + avalue;
        
        spaceBefore.maximum = spaceSum - 1;
        spaceAfter.maximum = spaceSum - 1;
        partitionSize.maximum = spaceSum;
        
        spaceBefore.setValue(bvalue, false);
        partitionSize.setValue(value, false);
        spaceAfter.setValue(avalue, false);
        
    }
    
    SpacedText { text: "Free space before (MB): "; spacerHeight: 10 }

    SpinBox
    {
        id: spaceBefore
        value: 0.0
        minimum: 0.0
        
        minimumWidth: 150
        minimumHeight: 20
        
        /*
         * When the free space before is increase/decreased, decrease/increase the partition size respectively,
         * but only if we aren't going above the allowed values.
         */
        onSetNewValue: {
            var newPartitionVal = partitionSize.value - (spaceBefore.value - oldValue);
            
            if (newPartitionVal > partitionSize.maximum || newPartitionVal < partitionSize.minimum) {
                spaceBefore.setValue(oldValue, false);
            }
            else {
                partitionSize.setValue(newPartitionVal, false);
            }
        }
    }

    SpacedText { text: "Partition size (MB): "; spacerHeight: 10 }

    SpinBox
    {
        id: partitionSize
        minimum: 1.0
        
        minimumWidth: 150
        minimumHeight: 20
        
        onSetNewValue: {
            var newAfterVal = spaceAfter.value - (partitionSize.value - oldValue);
            
            if (newAfterVal > spaceAfter.maximum || newAfterVal < spaceAfter.minimum) {
                partitionSize.setValue(oldValue, false);
            }
            else {
                spaceAfter.setValue(newAfterVal, false);
            }
        }
    }

    SpacedText { text: "Free space after (MB): "; spacerHeight: 10 }

    SpinBox
    {
        id: spaceAfter
        value: 0.0
        minimum: 0.0
        
        minimumWidth: 150
        minimumHeight: 20
        
        onSetNewValue: {
            var newPartitionVal = partitionSize.value - (spaceAfter.value - oldValue);
            
            if (newPartitionVal > partitionSize.maximum || newPartitionVal < partitionSize.minimum) {
                spaceAfter.setValue(oldValue, false);
            }
            else {
                partitionSize.setValue(newPartitionVal, false);
            }
        }
    }
}