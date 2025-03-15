import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property int itemsHeight: 35

    GroupBox {
        width: root.width
        height: root.height

        Row {
            id: rowHost
            spacing: 10
            width: parent.width
            height: root.itemsHeight

            Text {
                id: lblHost
                text: qsTr("Host")
                anchors.verticalCenter: parent.verticalCenter
            }

            ComboBox {
                id: cbxHost
                model: ["mqtt://", "mqtts://", "ws://", "wss://"]
                width: 150
                height: root.itemsHeight
            }

            TextField {
                id: txtHost
                width: parent.width - lblHost.width - cbxHost.width - 20
                height: parent.height
                text: ""
            }
        }

        Row {
            id: rowPort
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowHost.bottom
            anchors.margins: 10

            Text {
                id: lblPort
                width: lblHost.width
                text: qsTr("Port")
                anchors.verticalCenter: parent.verticalCenter
            }

            SpinBox {
                id: spbPort
                width: 150
                height: parent.height
                value: 1883
                from: 1
                to: 65535
                stepSize: 1
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row {
            id: rowClient
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowPort.bottom
            anchors.margins: 10

            Text {
                id: lblClientId
                text: qsTr("Client ID")
                anchors.verticalCenter: parent.verticalCenter
            }

            TextField {
                id: txtClientId
                width: (parent.width / 3 - 20) - lblClientId.width
                height: parent.height
                text: ""
            }

            Text {
                id: lblUsername
                text: qsTr("Username")
                anchors.verticalCenter: parent.verticalCenter
            }

            TextField {
                id: txtUsername
                width: (parent.width / 3 - 20) - lblUsername.width
                height: parent.height
                text: ""
            }

            Text {
                id: lblPassword
                text: qsTr("Password")
                anchors.verticalCenter: parent.verticalCenter
            }

            TextField {
                id: txtPassword
                width: (parent.width / 3 - 20) - lblPassword.width
                height: parent.height
                text: ""
                echoMode: TextInput.Password
            }
        }
    }
}
