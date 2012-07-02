import QtQuick 1.1

Rectangle {
    id: diskList
    width: window.width / 3
    height: (window.height * 4) / 7
    
    border {
        width: 2
        color: "black"
    }
    
    ListView {
        id: diskListView
        model: diskModel
        
        highlight: SelectionRectangle {}
        
        anchors.fill: parent
        anchors.margins: 6
        
        delegate: Text {
            text: modelData
            font.family: "Helvetica"
            font.pointSize: 11
            lineHeight: 1.3
            
            MouseArea {
                anchors.fill: parent
                onClicked: { diskListView.currentIndex = index; window.selectedDiskChanged(modelData) }
            }
        }
    }
}
