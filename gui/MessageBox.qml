import QtQuick 2.0
import QtQuick.Controls 2.0

Popup {
    id: root
    property alias message : messageText.text

    Text {
        id: messageText
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
}
