import QtQuick 1.0
import ComboBox 1.0

Rectangle
{
    id: formatDialog
    objectName: "formatDialog"
    
    width: 260
    height: 150
    color: "#DDDDDD"
    opacity: 0
    
    smooth: true
    radius: 2
    
    border {
        color: "black"
        width: 2
    }
    
    states: [
        State {
            name: "visible"
            PropertyChanges { target: formatDialog; opacity: 1 }
        },
        
        State {
            name: "invisible"
            PropertyChanges { target: formatDialog; opacity: 0 }
        }
    ]
    state: "invisible"
    
    transitions: [
        Transition {
            from: "invisible"
            to: "visible"
            
            PropertyAnimation { target: formatDialog; property: "opacity"; to: 1; duration: 150 }
        },
        
        Transition {
            from: "visible"
            to: "invisible"
            
            PropertyAnimation { target: formatDialog; property: "opacity"; to: 0; duration: 150 }
        }
    ]
    
    property string partition : "none"
    signal closed(bool accepted, string filesystem, string partition)
    
    function show(partition) {
        formatDialog.partition = partition;
        formatDialog.state = "visible";
    }
    
    function hide() {
        formatDialog.state = "invisible";
    }
    
    Text {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 6
        
        text: "Select filesystem: "
        font {
            family: "Helvetica"
            pointSize: 12
        }
    }
    
    ComboBox {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        
        id: combobox
    }
    
    FlagsList {
        id: flagsList
        objectName: "flagsList"
        
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
    }
        
    Row {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        spacing: 10
        
        GenericButton {
            text: "Ok"
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    formatDialog.hide();
                    formatDialog.closed(true, combobox.currentText, formatDialog.partition);
                }
            }
        }
        
        GenericButton {
            text: "Cancel"
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    formatDialog.hide();
                    formatDialog.closed(false, combobox.currentText, formatDialog.partition);
                }
            }
        }
    }
}