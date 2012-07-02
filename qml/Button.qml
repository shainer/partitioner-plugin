import QtQuick 1.0

Rectangle {
    id: actionButton
    objectName: "actionButton"
    
    property string text: "Button"
    property string iconPath: "none"
    
    signal clicked
    
    height: buttonbox.height
    width: window.width / 6
    
    border {
        width: 2
        color: "black"
    }
    
    color: "#EEDDDD"
    smooth: true
    radius: 4
    
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: actionButton.clicked();
    }
    
    Column {
        id: layout
        anchors.fill: parent
        spacing: 3
        
        Image {
            id: buttonIcon
            source: actionButton.iconPath
            anchors.horizontalCenter: parent.horizontalCenter
        }
        
        Text {
            id: buttonLabel
            anchors.horizontalCenter: parent.horizontalCenter
            color: "black"
            text: actionButton.text
        }
    }
}
