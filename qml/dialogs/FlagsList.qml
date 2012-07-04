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