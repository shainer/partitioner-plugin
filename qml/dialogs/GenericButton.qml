import QtQuick 1.0

Rectangle
{
    property variant text
    signal clicked

    height: 30
    width: 80
    
    border.width: 1
    radius: 4
    smooth: true

    gradient: 
        Gradient
        {
            GradientStop
            {
                position: 0.0
                color: !mouseArea.pressed ? activePalette.light : activePalette.button
            }
            GradientStop
            {
                position: 1.0
                color: !mouseArea.pressed ? activePalette.button : activePalette.dark
            }
        }

    SystemPalette { id: activePalette }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        onClicked: parent.clicked()
    }

    Text
    {
        id: text
        anchors.centerIn: parent
        font.pointSize: 9
        text: parent.text
        color: activePalette.buttonText
    }
}