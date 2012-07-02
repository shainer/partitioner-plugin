import QtQuick 1.0

Rectangle {
    width: window.width
    height: 200
    
    border {
        width: 2
        color: "black"
    }
    
    ListView {
        id: actionListView
        model: actionModel
        
        anchors.fill: parent
        anchors.margins: 6
        
        delegate: Text {
            text: modelData
        }
    }
}