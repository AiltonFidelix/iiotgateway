import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property int itemsHeight: 35

    GroupBox {
        id: gpbMqtt
        width: root.width
        height: root.height

        background: Rectangle {
            y: gpbMqtt.topPadding - gpbMqtt.bottomPadding
            width: parent.width
            height: parent.height - gpbMqtt.topPadding + gpbMqtt.bottomPadding
            color: "transparent"
            border.color: Material.color(Material.DeepPurple)
            radius: 8
        }

        Row {
            id: rowHost
            spacing: 10
            width: parent.width
            height: root.itemsHeight

            Text {
                id: lblHost
                width: 155
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
            id: rowClientId
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowPort.bottom
            anchors.margins: 10

            Text {
                id: lblClientId
                width: lblHost.width
                text: qsTr("Client ID")
                anchors.verticalCenter: parent.verticalCenter
            }

            TextField {
                id: txtClientId
                width: (parent.width - 12) - lblClientId.width
                height: parent.height
                text: ""
            }
        }

        Row {
            id: rowUsername
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowClientId.bottom
            anchors.margins: 10

            Text {
                id: lblUsername
                width: lblHost.width
                text: qsTr("Username")
                anchors.verticalCenter: parent.verticalCenter
            }

            TextField {
                id: txtUsername
                width: (parent.width - 12) - lblUsername.width
                height: parent.height
                text: ""
            }
        }

        Row {
            id: rowPassword
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowUsername.bottom
            anchors.margins: 10

            Text {
                id: lblPassword
                width: lblHost.width
                text: qsTr("Password")
                anchors.verticalCenter: parent.verticalCenter
            }

            TextField {
                id: txtPassword
                width: (parent.width - 12) - lblPassword.width
                height: parent.height
                text: ""
                echoMode: TextInput.Password
            }
        }

        Row {
            id: rowVersion
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowPassword.bottom
            anchors.margins: 10

            Text {
                id: lblVersion
                width: lblHost.width
                text: qsTr("MQTT Version")
                anchors.verticalCenter: parent.verticalCenter
            }

            ComboBox {
                id: cbxVersion
                model: ["3.1", "3.1.1", "5.0"]
                width: 150
                height: root.itemsHeight
            }
        }

        Row {
            id: rowTimeout
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowVersion.bottom
            anchors.margins: 10

            Text {
                id: lblTimeout
                width: lblHost.width
                text: qsTr("Connection timeout")
                anchors.verticalCenter: parent.verticalCenter
            }

            SpinBox {
                id: spbTimeout
                width: 150
                height: parent.height
                value: 10
                from: 1
                to: 3600
                stepSize: 1
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: "(s)"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row {
            id: rowKeepAlive
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowTimeout.bottom
            anchors.margins: 10

            Text {
                id: lblKeepAlive
                width: lblHost.width
                text: qsTr("Keep alive")
                anchors.verticalCenter: parent.verticalCenter
            }

            SpinBox {
                id: spbKeepAlive
                width: 150
                height: parent.height
                value: 60
                from: 1
                to: 3600
                stepSize: 1
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: "(s)"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row {
            id: rowSwitch
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowKeepAlive.bottom
            anchors.margins: 10

            Switch {
                id: chbAutoReconnect
                text: "Auto reconnet"
                anchors.verticalCenter: parent.verticalCenter
            }

            Switch {
                id: chbCleanStart
                text: "Clean start"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
