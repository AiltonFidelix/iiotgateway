import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import IIoTDashboard 1.0

Item {
    id: root

    property string statusText: "Failed!"
    property color statusColor: Material.color(Material.Red)
    property bool gtwRunning: false

    signal logout

    GatewayController {
        id: deviceController
    }

    Popup {
        id: statusPopup
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        anchors.centerIn: parent
        width: 280
        height: 180
        modal: true
        focus: true

        Label {
            id: statusLabel
            anchors.horizontalCenter: parent.horizontalCenter
            y: (root.y / 2) + btnStatus.height
            text: root.statusText
            font.bold: true
            color: root.statusColor
        }

        Button {
            id: btnStatus
            text: qsTr("Ok")
            anchors.bottom: parent.bottom
            anchors.margins: 2
            anchors.horizontalCenter: parent.horizontalCenter
            highlighted: true
            enabled: true
        }
    }

    Column {
        anchors.fill: parent
        spacing: 20

        Row {
            id: rowHeader
            width: parent.width
            height: 50

            Rectangle {
                id: gtwStatusIndicator
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 5
                width: 20
                height: 20
                radius: gtwStatusIndicator.width
                color: Material.color(Material.Red)

                Timer {
                    id: gtwStatusTimer
                    interval: 500
                    running: root.gtwRunning
                    repeat: true
                }
            }

            Text {
                id: txtGtwStatus
                anchors.left: gtwStatusIndicator.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 5
                text: root.gtwRunning ? qsTr("Gateway running") : qsTr("Gateway stopped")
                font.pointSize: 12
                color: Material.color(Material.DeepPurple)
            }

            Text {
                id: txtTitle
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 5
                text: qsTr("IIoTGateway Settings")
                font.bold: true
                font.pointSize: 20
                color: Material.color(Material.DeepPurple)
            }

            Button {
                id: btnLogout
                text: qsTr("Logout")
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 5
                highlighted: true
                enabled: true
                height: parent.height - 8
                onClicked: root.logout()
            }
        }

        Row {
            width: parent.width
            height: parent.height - rowHeader.height
            spacing: 10

            Column {
                id: colEdgeProtocol
                spacing: 8
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.horizontalCenter
                width: parent.width / 2
                height: parent.height - rowHeader.height

                Text {
                    id: txtEdgeProtocol
                    text: qsTr("Edge Protocol")
                    font.pointSize: 15
                    color: Material.color(Material.DeepPurple)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                ComboBox {
                    id: cbxEdgeProtocol
                    model: ["Modbus RTU"]
                    width: 200
                    height: 35
                    anchors.horizontalCenter: txtEdgeProtocol.horizontalCenter
                    anchors.top: txtEdgeProtocol.bottom
                    anchors.margins: 5
                }

                ModbusRTU {
                    id: modbusRTUSettings
                    anchors.top: cbxEdgeProtocol.bottom
                    anchors.horizontalCenter: cbxEdgeProtocol.horizontalCenter
                    anchors.margins: 5
                    visible: cbxEdgeProtocol.currentText === "Modbus RTU"
                    width: parent.width - 20
                    height: parent.height - txtEdgeProtocol.height - cbxEdgeProtocol.height
                }
            }

            Column {
                id: colCloudProtocol
                spacing: 8
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.horizontalCenter
                width: parent.width / 2
                height: parent.height - rowHeader.height

                Text {
                    id: txtCloudProtocol
                    text: qsTr("Cloud Protocol")
                    font.pointSize: 15
                    color: Material.color(Material.DeepPurple)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                ComboBox {
                    id: cbxCloudProtocol
                    model: ["MQTT"]
                    width: 200
                    height: 35
                    anchors.horizontalCenter: txtCloudProtocol.horizontalCenter
                    anchors.top: txtCloudProtocol.bottom
                    anchors.margins: 5
                }

                MQTT {
                    id: mqttSettings
                    anchors.top: cbxCloudProtocol.bottom
                    anchors.horizontalCenter: cbxCloudProtocol.horizontalCenter
                    anchors.margins: 5
                    visible: cbxCloudProtocol.currentText === "MQTT"
                    width: parent.width - 20
                    height: parent.height - txtCloudProtocol.height - cbxCloudProtocol.height
                }
            }
        }

        Button {
            id: btnSave
            text: qsTr("Save")
            anchors.bottom: parent.bottom
            anchors.margins: 5
            anchors.right: parent.right
            height: btnLogout.height
            highlighted: true
            enabled: true
        }


        Button {
            id: btnRestart
            text: qsTr("Restart")
            anchors.bottom: parent.bottom
            anchors.margins: 5
            anchors.right: btnSave.left
            height: btnLogout.height
            highlighted: root.gtwRunning
            enabled: root.gtwRunning

            onClicked: deviceController.restart()
        }

        Button {
            id: btnStart
            text: qsTr("Start")
            anchors.bottom: parent.bottom
            anchors.margins: 5
            anchors.right: btnRestart.left
            height: btnLogout.height
            visible: !root.gtwRunning
            highlighted: !root.gtwRunning
            enabled: !root.gtwRunning

            onClicked: deviceController.start()
        }

        Button {
            id: btnStop
            text: qsTr("Stop")
            anchors.bottom: parent.bottom
            anchors.margins: 5
            anchors.right: btnRestart.left
            height: btnLogout.height
            visible: root.gtwRunning
            highlighted: root.gtwRunning
            enabled: root.gtwRunning

            onClicked: deviceController.stop()
        }

        Footer {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 5
        }
    }

    // Slots Connections

    Connections {
        target: btnStatus

        function onClicked() {
            statusPopup.close()
        }
    }

    Connections {
        target: btnSave

        function onClicked() {
            let deviceSettings = {
                MQTT: mqttSettings.getSettings(),
                ModbusRTU: modbusRTUSettings.getSettings()
            }

            deviceController.setSettings(JSON.stringify(deviceSettings))
        }
    }

    Connections {
        target: deviceController

        function onDeviceSettings(settings) {
            let json = JSON.parse(settings)

            mqttSettings.setSettings(json.MQTT)
            modbusRTUSettings.setSettings(json.ModbusRTU)
        }
    }

    Connections {
        target: deviceController

        function onSuccess(message) {
            root.statusText = message
            root.statusColor = Material.color(Material.Green)
            statusPopup.open()
        }
    }

    Connections {
        target: deviceController

        function onError(error) {
            root.statusText = error
            root.statusColor = Material.color(Material.Red)
            statusPopup.open()
        }
    }

    Connections {
        target: gtwStatusIndicator

        function onColorChanged() {
            root.gtwRunning = !(gtwStatusIndicator.color === Material.color(Material.Red))
        }
    }

    Connections {
        target: gtwStatusTimer

        function onTriggered() {
            if (gtwStatusIndicator.color === Material.color(Material.Green)) {
                gtwStatusIndicator.color = "white";
            } else {
                gtwStatusIndicator.color = Material.color(Material.Green);
            }
        }
    }
}
