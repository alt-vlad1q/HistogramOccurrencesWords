import Provider 1.0
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    visible: true
    width: 900
    height: 620
    title: qsTr("Histogram of occurrence of words")

    property Provider provider
    property alias progressValue: controlBlock.progressValue
    property alias labelText: controlBlock.choiceBarText
    property alias barText: histogramView.barText

    onBarTextChanged: {
        labelText = barText
    }

    onProviderChanged: {
        controlBlock.provider = provider
    }

    Connections {
        id: wordsUpdate
        target: provider
        onMapChanged: function (map) {
            histogramView.changeWordCountbyMap(map)
        }
    }

    Connections {
        id: percentUpdate
        target: provider
        onCompletedPercentChanged: function (value) {
            progressValue = value
        }
    }

    onClosing: {
        provider.stop()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        ControlBlock {
            id: controlBlock
            chartView: histogramView
            refWordUpdate: wordsUpdate
            refPercentUpdate: percentUpdate
            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: parent.height * 0.15
            Layout.preferredWidth: parent.width * 0.95
            Layout.topMargin: 10
        }

        HistogramView {
            id: histogramView
            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.85
        }
    }
}
