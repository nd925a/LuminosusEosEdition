import QtQuick 2.5
import CustomElements 1.0
import "../CustomBasics"
import "../CustomControls"

BlockBase {
    id: root
    width: 70*dp
    height: 90*dp
    settingsComponent: settings

    StretchColumn {
        anchors.fill: parent

        NumericInput {
            centered: true
            minimumValue: 1
            maximumValue: 9999
            value: block.macroNumber
            onValueChanged: {
                if (value !== block.macroNumber) {
                    block.macroNumber = value
                }
            }
        }
        ButtonBottomLine {
            text: "Fire"
            onTouchDown: block.fire(1.0)
            onTouchUp: block.fire(0.0)
        }

        DragArea {
            text: "Macro"
            InputNode {
                objectName: "inputNode"
            }
        }
    }  // end main Column

    Component {
        id: settings
        StretchColumn {
            leftMargin: 15*dp
            rightMargin: 15*dp
            defaultSize: 30*dp

            BlockRow {
                Text {
                    text: "Macro Number:"
                    width: parent.width - 40*dp
                }
                NumericInput {
                    width: 40*dp
                    minimumValue: 1
                    maximumValue: 9999
                    value: block.macroNumber
                    onValueChanged: {
                        if (value !== block.macroNumber) {
                            block.macroNumber = value
                        }
                    }
                }
            }
        }
    }  // end Settings Component
}

