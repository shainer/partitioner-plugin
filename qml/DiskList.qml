import QtQuick 1.1

Rectangle {
    width: window.width / 3
    height: 400
    
    border {
        width: 2
        color: "black"
    }
    
    ListView {
        id: diskListView
        model: diskModel
        
        anchors.fill: parent
        anchors.margins: 6
        
        delegate: Text {
            text: modelData
            font.family: "Helvetica"
            font.pointSize: 11
            lineHeight: 1.3
            
            MouseArea {
                anchors.fill: parent
                onClicked: console.log(parent.text)
            }
        }
    }
}
