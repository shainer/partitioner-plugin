/*
 * Taken from https://qt.gitorious.org/qt-components/qt-components
 */
import Qt 4.7
import "./behaviors"
import "./styles/default" as DefaultStyles

CheckBox {
    id: radiobutton
    
    width: 15
    height: 15

    checkmark: defaultStyle.checkmark
    background: defaultStyle.background

    DefaultStyles.RadioButtonStyle { id: defaultStyle }
}
