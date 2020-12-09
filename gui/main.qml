import Provider 1.0
import QtQml 2.3
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 900
    height: 620
    title: qsTr("Histogram of occurrence of words")

    property Provider provider: null
    property alias progressValue: controlBlock.progressValue
    property alias labelText: controlBlock.choiceBarText
    property alias barText: histogramView.barText

    onBarTextChanged: {
        labelText = barText
    }

    onProviderChanged: {
        controlBlock.provider = provider
    }

//    Connections {
//        id: wordsUpdate
//        target: provider
//        onValueChanged: function (value) {
//            histogramView.changeWordCount(value[0], value[1])
//            histogramView.rescale()
//            histogramView.colorize()
//        }
//    }

    Connections {
        id: wordsUpdate
        target: provider
        onMapChanged: function (map) {
            histogramView.changeWordCountbyMap(map)
//            histogramView.rescale()
            histogramView.colorize()
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
            Layout.preferredHeight: parent.width * 0.06
            Layout.preferredWidth: parent.width * 0.9
            Layout.topMargin: 10
        }

        HistogramView {
            id: histogramView
            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
