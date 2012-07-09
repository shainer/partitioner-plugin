/*
 * Taken from https://qt.gitorious.org/qt-components/qt-components
 */
import Qt 4.7

QtObject {

    property Component background:

    Component {
        Item {
            Rectangle{
                x: 1; y: 1
                width: parent.width-2
                height: parent.height-2
                color: button.backgroundColor
                radius: 5
            }

            BorderImage {
                anchors.fill: parent
                smooth: true
                source: button.pressed ? "images/button_pressed.png" : "images/button_normal.png"
                border.left: 6; border.top: 6
                border.right: 6; border.bottom: 6
            }

            Rectangle {
                anchors.fill: parent
                color: "black"
                opacity: button.checked ? 0.4 : 0
            }
        }
    }

    property Component content:

    Component {
        id: defaultContent
        Item {
            property int minimumWidth: 40
            property int minimumHeight: 25
            width: Math.max(minimumWidth, row.width + 15 + 15) // add left and right margins
            height: Math.max(minimumHeight, row.height + 5 + 5) // add top and bottom margins

            Row {
                id: row
                spacing: 4
                anchors.centerIn: parent
                Image {
                    source: button.iconSource
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    color: !button.enabled ? "gray" : button.textColor
                    anchors.verticalCenter: parent.verticalCenter
                    text: button.text
                }
            }
        }
    }
}
