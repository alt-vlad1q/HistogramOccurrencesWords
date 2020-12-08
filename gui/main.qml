import Provider 1.0
import QtQml 2.12
import QtQuick 2.11
import QtCharts 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.12

ApplicationWindow {
    visible: true
    width: 900
    height: 620
    title: qsTr("Histogram of occurrence of words")

    property Provider provider: null
    property alias progressValue: controlBlock.value

    Connections {
        target: provider
        onValueChanged: function (value) {
            histogramView.changeWordCount(value[0], value[1])
        }
    }
    Connections {
        target: provider
        onCompletedPercentChanged: function (value) {
            progressValue = value
        }
    }

    onClosing: {
        provider.close()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        ControlBlock {
            id: controlBlock
            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: parent.width * 0.05
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
