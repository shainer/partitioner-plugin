import QtQuick 1.0

Rectangle
{    
    width: window.width
    height: 75
    
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