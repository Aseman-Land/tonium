pragma Singleton

import QtQuick 2.12
import Toolkit.Core 1.0

QtObject {
    readonly property string touchIdPass: touchIdSecureKey

    readonly property real dialogsRoundness: 8
    readonly property real roundness: 16
    readonly property real controlsRoundness: 6

    readonly property bool mobileView: GlobalValues.mwin && GlobalValues.mwin.width < GlobalValues.mwin.height? true : false
    readonly property real panelWidth: 320

    readonly property real itemsHeight: 40

    readonly property int commentLimit: 128
}
