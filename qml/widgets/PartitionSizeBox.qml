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
import ApplicationWidgets 1.0

Grid
{
    columns: 2
    rows: 3
    spacing: 5
    
    property real pSize: partitionSize.actualValue
    property real bSize: spaceBefore.actualValue
    
    /* Sets initial values for spinboxes */
    function init(bvalue, value, avalue, minSize)
    {
        var spaceSum = bvalue + value + avalue;
        
        spaceBefore.maximum = spaceSum - 1;
        spaceAfter.maximum = spaceSum - 1;
        partitionSize.maximum = spaceSum;
        partitionSize.minimum = minSize;
        
        spaceBefore.setValueWithoutNotify(bvalue.toFixed(3));
        spaceBefore.actualValue = bvalue;
        
        partitionSize.setValueWithoutNotify(value.toFixed(3));
        partitionSize.actualValue = value;
        
        spaceAfter.setValueWithoutNotify(avalue.toFixed(3));
        spaceAfter.actualValue = avalue;
    }
    
    SpacedText { text: "Free space before (MB): "; spacerHeight: 10 }

    SpinBox
    {
        id: spaceBefore
        value: 0.0
        minimum: 0.0
        
        width: 100
        height: 20
        
        /*
         * When the free space before is increase/decreased, decrease/increase the partition size respectively,
         * but only if we aren't going above the allowed values.
         */
        onValueChanged: {
            var newPartitionVal = partitionSize.value - (spaceBefore.value - oldValue);
            
            if (newPartitionVal > partitionSize.maximum || newPartitionVal < partitionSize.minimum) {
                spaceBefore.setValueWithoutNotify(oldValue);
            }
            else {
                partitionSize.setValueWithoutNotify(newPartitionVal);
            }
            
            parent.bSize = spaceBefore.actualValue;
        }
        
        onSimpleValueChanged: {
            parent.bSize = spaceBefore.actualValue;
        }
    }

    SpacedText { text: "Partition size (MB): "; spacerHeight: 10 }

    SpinBox
    {
        id: partitionSize
        minimum: 1.0
        
        width: 100
        height: 20
        
        onValueChanged: {
            var newAfterVal = spaceAfter.value - (partitionSize.value - oldValue);
            
            if (newAfterVal > spaceAfter.maximum || newAfterVal < spaceAfter.minimum) {
                partitionSize.setValueWithoutNotify(oldValue);
            }
            else {
                spaceAfter.setValueWithoutNotify(newAfterVal);
            }
            
            parent.pSize = partitionSize.actualValue;
        }
        
        onSimpleValueChanged: {
            parent.pSize = partitionSize.actualValue;
        }
    }

    SpacedText { text: "Free space after (MB): "; spacerHeight: 10 }

    SpinBox
    {
        id: spaceAfter
        value: 0.0
        minimum: 0.0
        
        width: 100
        height: 20
        
        onValueChanged: {
            var newPartitionVal = partitionSize.value - (spaceAfter.value - oldValue);
            
            if (newPartitionVal > partitionSize.maximum || newPartitionVal < partitionSize.minimum) {
                spaceAfter.setValueWithoutNotify(oldValue);
            }
            else {
                partitionSize.setValueWithoutNotify(newPartitionVal);
            }
        }
    }
}