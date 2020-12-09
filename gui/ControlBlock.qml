import Provider 1.0
import "functions.js" as Js
import QtQml 2.3
import QtQuick 2.11
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.3

Item {
    property Provider provider: null
    property HistogramView chartView
    property Connections refWordUpdate
    property Connections refPercentUpdate
    property double timeMs : 0
    property alias progressValue: progressBar.value
    property alias choiceBarText: labelText.text
    property color foreGround: Qt.rgba(0.26, 0.67, 0.55, 1)
    property color buttonImageColor: Qt.rgba(0, 0, 0, 0.5)

    function connectEnabled (enabled) {
        if (refWordUpdate.enabled !== enabled) refWordUpdate.enabled = enabled
        if (refPercentUpdate.enabled !== enabled) refPercentUpdate.enabled = enabled
    }

    onProgressValueChanged: {
        if (progressValue === progressBar.to) {
            if (provider.stop()) {
                connectEnabled(false)
                playButton.enabled = true
                stopButton.enabled = false
                choiceButton.enabled = true
                var interval = Js.convertMs(new Date().getTime() - timeMs)
                elapsedTime.text = "<b>Elapsed time</b> : " + Js.makeStringByInterval(interval)
            }
        }
    }

    FileDialog{
        id: fileDialog;
        title: "Please choose a source file";
        onAccepted: {
            var path = fileDialog.fileUrl.toString();
            path= path.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"");
            pathField.text = decodeURIComponent(path);
            fileDialog.close()
            playButton.enabled = true
        }
    }

    MessageBox {
        id: messageBox
        width: parent.width * 0.5
        height: parent.height
        x: width / 2
        onClosed: {
            playButton.enabled = false
            stopButton.enabled = false
            choiceButton.enabled = true
        }
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            width: parent.width

            TextField {
                id: pathField
                enabled: false
                Layout.fillWidth: true
                placeholderText: fileDialog.title
            }

            ImageButton {
                id : choiceButton
                Layout.preferredWidth: parent.width * 0.05
                imageSource: "img/choice.svg"
                imageColor: buttonImageColor
                onClicked: fileDialog.open();
            }

            ImageButton {
                id : playButton
                Layout.preferredWidth: parent.width * 0.05
                enabled: false
                imageSource: "img/play.svg"
                imageColor: buttonImageColor
                onClicked: {
                    progressBar.value = progressBar.from
                    chartView.clearAllWords()

                    connectEnabled(true)
                    enabled = false
                    stopButton.enabled = true
                    choiceButton.enabled = false
                    timeMs = new Date().getTime()
                    var check = provider.start(fileDialog.fileUrl)
                    if (check) {
                        var error = qsTr(check + " Try to select another file.")
                        messageBox.message = error
                        messageBox.open()
                    }
                }
            }

            ImageButton {
                id: stopButton
                Layout.preferredWidth: parent.width * 0.05
                enabled: false
                imageSource: "img/stop.svg"
                imageColor: buttonImageColor
                onClicked: {
                    if (provider.stop()) {
                        connectEnabled(false)
                        chartView.clearAllWords()
                        progressBar.value = progressBar.from
                        elapsedTime.text = ""
                        enabled = false
                        playButton.enabled = true
                    }
                }
            }
        }

        ProgressBar {
            id: progressBar
            Layout.fillWidth: true
            from: 0
            to: 100

            contentItem: Item {
                width: parent.width
                height: parent.height
                Rectangle {
                    width: progressBar.visualPosition * parent.width
                    height: parent.height
                    color: foreGround
                }
            }
        }

        RowLayout {
            width: parent.width

            Text {
                id: labelText
                Layout.fillWidth: true
                verticalAlignment: Text.AlignTop
            }

            Text {
                id: elapsedTime
                verticalAlignment: Text.AlignTop
            }
        }
    }
}

