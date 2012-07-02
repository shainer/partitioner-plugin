import QtQuick 1.0

Rectangle {
    id: actionButton
    objectName: "actionButton"
    
    property string text: "Button"
    property string iconPath: "none"
    
    signal clicked
    
    height: buttonbox.height
    width: window.width / 8
    
    border {
        width: 2
        color: "gray"
    }
    
    color: "#99CCCC"
    smooth: true
    radius: 5
    
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: actionButton.clicked();
    }
    
    Column {
        id: layout
        anchors.fill: parent
        spacing: 10
        
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
