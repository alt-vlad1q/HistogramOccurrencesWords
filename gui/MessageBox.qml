import QtQuick 2.12
import QtQuick.Controls 2.12

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
