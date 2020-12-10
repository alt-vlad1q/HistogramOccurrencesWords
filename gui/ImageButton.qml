import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12

Button {
    property var imageSource
    property color imageColor
    property var image: contentImage

    height: 50
    width: parent.width
    padding: 10

    ColorImage {
        id: contentImage
        color: imageColor
        anchors.centerIn: parent
        source: imageSource
        sourceSize: Qt.size(parent.width - parent.padding, parent.height - parent.padding)
    }
}
