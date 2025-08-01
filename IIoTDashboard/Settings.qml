import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property string statusText: "Failed!"
    property color statusColor: Material.color(Material.Red)
    property bool gtwRunning: false

    signal logout

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
            }
        }

        Row {
            width: parent.width
            height: parent.height - rowHeader.height
            spacing: 10

            Column {
                id: colEdgeCommunication
                spacing: 8
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.horizontalCenter
                width: parent.width / 2
                height: parent.height - rowHeader.height

                Text {
                    id: txtEdgeCommunication
                    text: qsTr("Edge Communication")
                    font.pointSize: 15
                    color: Material.color(Material.DeepPurple)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                // TODO: improve to be dynamic
                ComboBox {
                    id: cbxEdgeCommunication
                    model: ["Modbus RTU"]
                    width: 200
                    height: 35
                    anchors.horizontalCenter: txtEdgeCommunication.horizontalCenter
                    anchors.top: txtEdgeCommunication.bottom
                    anchors.margins: 5
                }

                ModbusRTU {
                    id: modbusRTUSettings
                    anchors.top: cbxEdgeCommunication.bottom
                    anchors.horizontalCenter: cbxEdgeCommunication.horizontalCenter
                    anchors.margins: 5
                    visible: cbxEdgeCommunication.currentText === "Modbus RTU"
                    width: parent.width - 20
                    height: parent.height - txtEdgeCommunication.height - cbxEdgeCommunication.height
                }
            }

            Column {
                id: colCloudCommunication
                spacing: 8
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.horizontalCenter
                width: parent.width / 2
                height: parent.height - rowHeader.height

                Text {
                    id: txtCloudCommunication
                    text: qsTr("Cloud Communication")
                    font.pointSize: 15
                    color: Material.color(Material.DeepPurple)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                // TODO: improve to be dynamic
                ComboBox {
                    id: cbxCloudCommunication
                    model: ["MQTT"]
                    width: 200
                    height: 35
                    anchors.horizontalCenter: txtCloudCommunication.horizontalCenter
                    anchors.top: txtCloudCommunication.bottom
                    anchors.margins: 5
                }

                MQTT {
                    id: mqttSettings
                    anchors.top: cbxCloudCommunication.bottom
                    anchors.horizontalCenter: cbxCloudCommunication.horizontalCenter
                    anchors.margins: 5
                    visible: cbxCloudCommunication.currentText === "MQTT"
                    width: parent.width - 20
                    height: parent.height - txtCloudCommunication.height - cbxCloudCommunication.height
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
        target: root

        function onVisibleChanged() {
            if (root.visible) {
                deviceController.requestSettings(["MQTT", "MODBUS_RTU"])
            }
        }
    }

    Connections {
        target: btnStatus

        function onClicked() {
            statusPopup.close()
        }
    }

    Connections {
        target: btnSave

        function onClicked() {
            // TODO: improve to be dynamic

            let deviceSettings = {
                cloud: {
                    protocol: "MQTT",
                    settings: mqttSettings.getSettings()
                },
                edge: {
                    protocol: "MODBUS_RTU",
                    settings: modbusRTUSettings.getSettings()
                }
            }

            deviceController.setSettings(JSON.stringify(deviceSettings))
        }
    }

    Connections {
        target: btnLogout

        function onClicked() {
            root.logout()
        }
    }

    Connections {
        target: deviceController

        function onStatus(status) {
            let data = JSON.parse(String(status))
            root.gtwRunning = (data.status  === "running")
        }
    }

    Connections {
        target: deviceController

        function onSettings(settings) {
            let data = JSON.parse(String(settings))

            mqttSettings.setSettings(data.MQTT)
            modbusRTUSettings.setSettings(data.MODBUS_RTU)
        }
    }

    Connections {
        id: connSuccess
        target: deviceController
        enabled: root.visible

        function onSuccess(message) {
            if (message.length > 0) {
                root.statusText = qsTr(message)
                root.statusColor = Material.color(Material.Green)
                statusPopup.open()
            }
        }
    }

    Connections {
        id: connError
        target: deviceController
        enabled: root.visible

        function onError(error) {
            if (error.length > 0) {
                root.statusText = qsTr(error)
                root.statusColor = Material.color(Material.Red)
                statusPopup.open()
            }
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
