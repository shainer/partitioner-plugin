import QtQuick 1.1

Rectangle {
    id: deviceTree
    width: (window.width * 2) / 3
    height: 400
    
    border {
        width: 2
        color: "black"
    }
    
    ListView {
        id: deviceTreeView
        
        anchors.fill: parent
        anchors.margins: 6
        
        model: deviceTreeModel
        delegate: deviceTreeDelegate
        highlight: Rectangle { anchors.fill: parent; color: "#FFFF88"; radius: 5}
        focus: true
    }
    
    Component {
        id: deviceTreeDelegate
        
        Rectangle {
            border.color: "#CCCCCC"
            width: deviceTree.width
            height: 30
            
            smooth: true
            radius: 2
            
            MouseArea {
                anchors.fill: parent
                onClicked: deviceTreeView.currentIndex = index
            }
            
            Row {
                anchors.fill: parent
                anchors.margins: 6
                
                Text {
                    id: space
                    text: if (partitionType == 2)
                            "        "
                        else if (deviceType != 0)
                            "    "
                        else
                            ""
                }
                
                Text {
                    id: devname
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceName
                    
                    width: deviceTree.width / 2
                }
                
                Text {
                    id: devmp
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceMountPoint
                    
                    width: (deviceTree.width) / 5
                }
                
                Text {
                    id: devfs
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceFileSystem
                    
                    width: deviceTree.width / 10
                }
                
                Text {
                    id: devsize
                    font.family: "Helvetica"
                    font.pointSize: 11
                    text: deviceSize
                    
                    width: deviceTree.width / 5
                }
            }
        }
    }
}