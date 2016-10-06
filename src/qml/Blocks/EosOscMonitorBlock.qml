import QtQuick 2.5
import CustomElements 1.0
import "../CustomBasics"
import "../CustomControls"

BlockBase {
    id: root
    width: 450*dp
    height: 300*dp
    onWidthChanged: block.positionChanged()

    StretchColumn {
        anchors.fill: parent

        // ---------------------- Incoming / Outgoing Checkboxes -----------

        BlockRow {
            height: 30*dp
            implicitHeight: 0 // do not stretch
            ButtonBottomLine {
                id: logIncomingCheckbox
                onTouchDown: controller.eosConnection().enableLogging(logIncomingCheckbox.active, logOutgoingCheckbox.active)
                text: "Incoming"
                toggle: true
                //textColor: "#88d"
            }
            ButtonBottomLine {
                id: logOutgoingCheckbox
                onTouchDown: {
                    controller.eosConnection().enableLogging(logIncomingCheckbox.active, logOutgoingCheckbox.active)
                }
                text: "Outgoing"
                toggle: true
            }
            ButtonBottomLine {
                text: "Clear"
                onPress: controller.eosConnection().clearLog()
            }
            HeightResizeArea {
                width: 30*dp
                target: root
                maxSize: 1000*dp
            }

            Component.onCompleted: {
                // initialize checkbox values:
                logIncomingCheckbox.active = controller.eosConnection().getLogIncomingIsEnabled()
                logOutgoingCheckbox.active = controller.eosConnection().getLogOutgoingIsEnabled()
            }
        }

        // --------------- ListView with Log -------------

        Item {
            // this item either displays the listview or a label "No Messages"
            implicitHeight: -1  // stretch height

            ListView {
                id: logList
                model: controller.eosConnection().getLog()
                clip: true
                verticalLayoutDirection: ListView.BottomToTop
                anchors.fill: parent
                anchors.leftMargin: 5*dp
                visible: logList.count !== 0
                delegate: Text {
                    height: contentHeight + 3*dp
                    anchors.left: parent.left
                    anchors.right: parent.right
                    wrapMode: Text.Wrap
                    text: modelData
                    color: (modelData.indexOf("[In]") !== -1) ? "#5d82dd" : "#aaa"
                    font.pixelSize: 14*dp
                    font.family: "BPmono"
                }  // end delegate

                // the default cacheBuffer value is greater than zero
                // but a value greater than zeros causes the application to crahs because of
                // the asynchronous creation of delegates and a possible race condition with
                // the debugger
                cacheBuffer: 0

                Connections {
                    target: controller.eosConnection()
                    onLogChanged: {
                        logList.model = controller.eosConnection().getLog()
                    }
                }
            }  // end ListView

            Text {
                text: "No Messages"
                visible: logList.count === 0
                anchors.centerIn: parent
            }
        }

        DragArea {
            text: "Eos OSC Connection"

            WidthResizeArea {
                target: root
                minSize: 450*dp
                maxSize: 1100*dp
            }
        }
    }
}

