import QtQml 2.12
import QtQuick 2.11
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.12

Item {
    property alias value: progressBar.value

    FileDialog{
        id: fileDialog;
        title: "Please choose a source file";
        onAccepted: {
            path.text = fileDialog.fileUrl;
            fileDialog.close()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            TextField {
                id: path
                enabled: false
                Layout.fillWidth: true
                placeholderText: fileDialog.title
            }
            Button {
                text: "..."

                Layout.preferredWidth: parent.width * 0.05
                onClicked: fileDialog.open();
            }
            Button {
                text: ""
                Layout.preferredWidth: parent.width * 0.05
            }
            Button {
                text: ""
                Layout.preferredWidth: parent.width * 0.05
            }
        }

        ProgressBar {
            id: progressBar
            from: 0
            to: 100
            Layout.fillWidth: true
        }
    }
}

