import Provider 1.0
import QtQuick 2.11
import QtCharts 2.2
import QtQuick.Controls 2.3

ApplicationWindow {
    visible: true
    width: 900
    height: 480
    title: qsTr("Histogram of occurrence of words")

    property Provider provider: null

    Connections {
        target: provider
        onValueChanged: function (value) {
            histogramView.changeWordCount(value[0], value[1])
        }
    }

    onClosing: {
        provider.close()
    }

    HistogramView {
        id: histogramView
    }
}
