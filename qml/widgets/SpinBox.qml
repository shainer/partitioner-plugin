/*
 * Taken from https://qt.gitorious.org/qt-components/qt-components (with some changes)
 * FIXME: fix the binding loop.
 */
import Qt 4.7
import "./styles/default"

Item
{
    id: spinbox

    property int minimumWidth: defaultStyle.minimumWidth
    property int minimumHeight: defaultStyle.minimumHeight

    property int leftMargin: defaultStyle.leftMargin
    property int topMargin: defaultStyle.topMargin
    property int rightMargin: defaultStyle.rightMargin
    property int bottomMargin: defaultStyle.bottomMargin

    width: Math.max(minimumWidth,
                    input.width + leftMargin + rightMargin)

    height: Math.max(minimumHeight,
                     input.height + topMargin + bottomMargin)

    property real value: 0.0
    property real actualValue: 0.0
    property real maximum: 99
    property real minimum: 0.0
    property real singlestep: 1
    
    property bool upEnabled: value != maximum;
    property bool downEnabled: value != minimum;

    property alias upPressed: mouseUp.pressed
    property alias downPressed: mouseDown.pressed
    property alias upHovered: mouseUp.containsMouse
    property alias downHovered: mouseDown.containsMouse
    property alias containsMouse: mouseArea.containsMouse

    property color backgroundColor: "#fff";
    property color textColor: "#222";

    property Component background: defaultStyle.background
    property Component up: defaultStyle.up
    property Component down: defaultStyle.down
    SpinBoxStyle { id: defaultStyle }

    /*
     * This signal is emitted when the user (and only the user!) sets a new value for the spinbox.
     * It's needed because in our use case changing the value of a spinbox affects the value of other spinboxes, but
     * if we send a signal for every change our application will loop forever. So don't try using valueChanged() instead.
     */
    signal setNewValue(real oldValue)
    
    /* increment() or decrement() are caused by the user clicking on the arrows, so always sent the signal */
    function increment()
    {
        var oldval = value
        value += singlestep
        actualValue += singlestep
        
        if (actualValue > maximum) {
            actualValue = maximum
            value = maximum
        }
            
        input.text = value
        spinbox.setNewValue(oldval)
    }

    function decrement()
    {
        var oldval = value
        value -= singlestep
        actualValue -= singlestep
        
        if (value < minimum) {
            value = minimum
        }
        
        input.text = value
        spinbox.setNewValue(oldval)
    }

    /*
     * This is called by the user when it changes the spinbox value, but also by the application because it performs all
     * the necessary checks on the new value. So send the signal only when safe.
     */
    function setValue(v, sendSignal)
    {
        var newval = parseFloat(v)
        var oldval = value
        
        if (newval > maximum)
            newval = maximum
        else if (newval < minimum)
            newval = minimum
        
        value = newval
        actualValue += (newval - oldval)
        input.text = value
        
        if (sendSignal) {
            spinbox.setNewValue(oldval)
        }
    }

    // background
    Loader
    {
        id: backgroundComponent
        anchors.fill: parent
        sourceComponent: background
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
    }

    TextInput
    {
        id: input
        font.pixelSize: 14
        anchors.margins: 5
        anchors.leftMargin: leftMargin
        anchors.topMargin: topMargin
        anchors.rightMargin: rightMargin
        anchors.bottomMargin: bottomMargin
        anchors.fill: parent
        selectByMouse: true
        text: spinbox.value
        validator: DoubleValidator { bottom: 11; top: 31 }
        onTextChanged: { spinbox.setValue(text, true); } /* the user set a different value from the GUI */
        color: textColor
        opacity: parent.enabled ? 1 : 0.5
    }

    Loader
    {
        id: upButton
        sourceComponent: up
        MouseArea
        {
            id: mouseUp
            anchors.fill: upButton.item
            onClicked: increment()
            Timer { running: parent.pressed; interval: 100 ; repeat: true ; onTriggered: increment() }
        }
        
        onLoaded: {
            item.parent = spinbox
            mouseUp.parent = item
            item.x = spinbox.width-item.width
            item.y = 0
        }
    }

    Loader
    {
        id: downButton
        sourceComponent: down
        MouseArea
        {
            id: mouseDown
            anchors.fill: downButton.item
            onClicked: decrement()
            Timer { running: parent.pressed; interval: 100 ; repeat: true ; onTriggered: decrement() }
        }
        
        onLoaded: {
            item.parent = spinbox
            mouseDown.parent = item
            item.x = spinbox.width-item.width
            item.y = spinbox.height - item.height
        }
    }
}
