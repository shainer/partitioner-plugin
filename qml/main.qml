import QtQuick 1.1

Rectangle
{
    id: window
    width: 1250
    height: 700
    color: "#DDDDDD"
    
    signal selectedDiskChanged (string newDisk)
    signal selectedDeviceChanged (string newDevice)
    signal actionButtonClicked (string actionName)
    
    Column {
        spacing: 10
        
        ButtonBox {
            id: buttonbox
        }
        
        Row {
            spacing: 7
            
            DiskList {
                id: diskList
            }
            
            DeviceTree {
                id: deviceTree
            }
        }
        
        ActionList {
            id: actionList
        }
    }
}