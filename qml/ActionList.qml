import QtQuick 1.0

Rectangle {
    width: window.width
    height: (window.height / 7) * 2
    
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