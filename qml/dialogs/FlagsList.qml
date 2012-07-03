import QtQuick 1.1

Item {
    id: flagsList
    
    Column {
        
        Repeater {
            id: repeater
            objectName: "repeater"
            
            model: flagsModel
            delegate: flagsDelegate
        }
    }
    
    /* DUMMY MODEL */
    ListModel {
        id: flagsModel
        
        ListElement { name: "boot"; index: 0 }
        ListElement { name: "required"; index: 1 }
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

                width: 20
                height: 20
            }
            
            Text {
                id: flagName
                objectName: "flagName"
                
                text: name
                font.pointSize: 10
            }
        }
    }
}