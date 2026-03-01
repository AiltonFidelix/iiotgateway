import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Popup {
    id: root

    property string statusText: ""
    property color statusColor: Material.color(Material.Red)

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    anchors.centerIn: parent
    width: 280
    height: 180
    modal: true
    focus: true

    Rectangle {
        anchors.fill: parent

        Label {
            id: lblStatus
            anchors.horizontalCenter: parent.horizontalCenter
            y: (parent.y / 2) + btnOk.height
            text: root.statusText
            font.bold: true
            color: root.statusColor
        }

        Button {
            id: btnOk
            text: qsTr("Ok")
            anchors.bottom: parent.bottom
            anchors.margins: 2
            anchors.horizontalCenter: parent.horizontalCenter
            highlighted: true
            enabled: true

            onClicked: root.close();
        }
    }
}
