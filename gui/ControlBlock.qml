import Provider 1.0
import "functions.js" as Js
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12

Item {
    property Provider provider
    property HistogramView chartView
    property Connections refWordUpdate
    property Connections refPercentUpdate
    property double timeMs : 0
    property alias progressValue: progressBar.value
    property alias choiceBarText: labelText.text
    property int buttonWidth: parent.width * 0.05 > 40
                              ? 40 : parent.width * 0.05
    property int buttonHeight: rowLayout.height > 40
                               ? 40 : rowLayout.height
    property color foreGround: Qt.rgba(0.26, 0.67, 0.55, 1)
    property color buttonImageColor: Qt.rgba(0, 0, 0, 0.5)

    function connectEnabled (enabled) {
        if (refWordUpdate.enabled !== enabled) refWordUpdate.enabled = enabled
        if (refPercentUpdate.enabled !== enabled) refPercentUpdate.enabled = enabled
    }

    onProgressValueChanged: {
        if (progressValue === progressBar.to) {
            provider.stop()
            connectEnabled(false)
            playButton.enabled = true
            stopButton.enabled = false
            choiceButton.enabled = true
            var interval = Js.convertMs(new Date().getTime() - timeMs)
            elapsedTime.text = "<b>Elapsed time</b> : " + Js.makeStringByInterval(interval)
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
            id: rowLayout
            width: parent.width
            Layout.preferredHeight: parent.height * 0.7

            TextField {
                id: pathField
                enabled: false
                clip: true
                Layout.preferredHeight: buttonHeight
                Layout.fillWidth: true
                placeholderText: fileDialog.title
            }

            ImageButton {
                id : choiceButton
                Layout.preferredHeight: buttonHeight
                Layout.preferredWidth: buttonWidth
                imageSource: "img/choice.svg"
                imageColor: buttonImageColor
                onClicked: fileDialog.open();
            }

            ImageButton {
                id : playButton
                Layout.preferredHeight: buttonHeight
                Layout.preferredWidth: buttonWidth
                enabled: false
                imageSource: "img/play.svg"
                imageColor: buttonImageColor
                onClicked: {
                    progressBar.value = progressBar.from
                    chartView.clearAllWords()
                    connectEnabled(true)
                    enabled = false
                    elapsedTime.text = ""
                    stopButton.enabled = true
                    choiceButton.enabled = false
                    timeMs = new Date().getTime()
                    var error = provider.start(fileDialog.fileUrl)
                    if (error) {
                        var message = qsTr(error + " Try to select another file.")
                        messageBox.message = message
                        messageBox.open()
                    }
                }
            }

            ImageButton {
                id: stopButton
                Layout.preferredWidth: buttonWidth
                Layout.preferredHeight: buttonHeight
                enabled: false
                imageSource: "img/stop.svg"
                imageColor: buttonImageColor
                onClicked: {
                    provider.stop()
                    connectEnabled(false)
                    chartView.clearAllWords()
                    progressBar.value = progressBar.from
                    elapsedTime.text = ""
                    choiceButton.enabled = true
                    enabled = false
                    playButton.enabled = true
                }
            }
        }

        ProgressBar {
            id: progressBar
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.15
            from: 0
            to: 100

            background: Rectangle {
                width: parent.width
                height: parent.height
                color: Qt.rgba(0, 0, 0, 0.1)
            }

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
            Layout.preferredHeight: parent.height * 0.5

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

