import QtQuick 2.15
import "../globals"

TControlElement {
    id: element
    height: Constants.itemsHeight

    property alias digitsCount: input.maximumLength
    property alias text: input.text

    property color color: Colors.foreground

    onDigitsCountChanged: input.clear()

    function vibrate() {
        vibrateTimer.restart();
    }

    onFocusChanged: {
        if (focus) {
            input.focus = true;
            input.forceActiveFocus();
        }
    }

    TextInput {
        id: input
        anchors.fill: parent
        opacity: 0
        clip: true
        maximumLength: 4
        inputMethodHints: Qt.ImhDigitsOnly
        validator: RegularExpressionValidator {
            regularExpression: /\d+/
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            input.focus = true;
            input.forceActiveFocus();
        }
    }

    Item {
        id: passFieldArea
        width: parent.width
        height: parent.height

        Timer {
            id: vibrateTimer
            interval: 50
            running: false
            repeat: true
            triggeredOnStart: true
            onTriggered: {
                passFieldArea.x = (counter % 2 == 0? 1 : -1) * 10;

                counter++;
                if (counter == 6) {
                    passFieldArea.x = 0;
                    counter = 0;
                    stop();
                }
            }

            property int counter: 0
        }

        Behavior on x {
            NumberAnimation { easing.type: Easing.OutCubic; duration: vibrateTimer.interval }
        }

        Repeater {
            model: element.digitsCount

            Rectangle {
                width: 14
                x: model.index * (element.width / element.digitsCount) + (element.width / element.digitsCount)/2 - width/2
                anchors.verticalCenter: parent.verticalCenter
                height: width
                radius: width / 2
                opacity: model.index < input.length? 1 : 0.4
                color: model.index < input.length? element.color : "transparent"
                border.width: 1
                border.color: element.color
            }
        }
    }
}
