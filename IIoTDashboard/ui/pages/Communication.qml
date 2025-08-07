import QtQuick
import QtQuick.Controls
import "../components"
import "../modbus"
import "../mqtt"

Page {
    id: root
    title: "Communication"

    property string statusText: "Failed!"
    property color statusColor: Material.color(Material.Red)
    property bool gtwRunning: false

    signal logout

    Component.onCompleted: {
        backend.requestCommnunicationSettings(["MQTT", "MODBUS_RTU"]);
    }

    InfoPopUp {
        id: statusPopup
        statusText: root.statusText
        statusColor: root.statusColor
    }

    Column {
        anchors.fill: parent
        spacing: 20

        Row {
            id: rowHeader
            width: parent.width
            height: 50
            spacing: 5

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
        }

        Row {
            width: parent.width
            height: parent.height - rowHeader.height
            spacing: 10

            Column {
                id: colCloudCommunication
                spacing: 8
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

            Column {
                id: colEdgeCommunication
                spacing: 8
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
        }

        Button {
            id: btnSave
            text: qsTr("Save")
            anchors.bottom: parent.bottom
            anchors.margins: 5
            anchors.right: parent.right
            highlighted: true
            enabled: true
        }

        Button {
            id: btnRestart
            text: qsTr("Restart")
            anchors.bottom: parent.bottom
            anchors.margins: 5
            anchors.right: btnSave.left
            highlighted: root.gtwRunning
            enabled: root.gtwRunning

            onClicked: backend.restart();
        }

        Button {
            id: btnStart
            text: qsTr("Start")
            anchors.bottom: parent.bottom
            anchors.margins: 5
            anchors.right: btnRestart.left
            visible: !root.gtwRunning
            highlighted: !root.gtwRunning
            enabled: !root.gtwRunning

            onClicked: backend.start();
        }

        Button {
            id: btnStop
            text: qsTr("Stop")
            anchors.bottom: parent.bottom
            anchors.margins: 5
            anchors.right: btnRestart.left
            visible: root.gtwRunning
            highlighted: root.gtwRunning
            enabled: root.gtwRunning

            onClicked: backend.stop();
        }
    }

    // Send new settings to the backend and save it
    Connections {
        target: btnSave

        function onClicked() {
            // TODO: improve to be dynamic

            let settings = {
                cloud: {
                    protocol: "MQTT",
                    settings: mqttSettings.getSettings()
                },
                edge: {
                    protocol: "MODBUS_RTU",
                    settings: modbusRTUSettings.getSettings()
                }
            };

            backend.setCommunicationSettings(JSON.stringify(settings));
        }
    }

    // Handle gateway status comming from the backend
    Connections {
        target: backend

        function onStatus(status) {
            let data = JSON.parse(String(status));
            root.gtwRunning = (data.status  === "running");
        }
    }

    // Handle gateway communication settings comming from the backend
    Connections {
        target: backend

        function onCommnunicationSettings(settings) {
            let data = JSON.parse(String(settings));

            mqttSettings.setSettings(data.MQTT);
            modbusRTUSettings.setSettings(data.MODBUS_RTU);
        }
    }

    // Handle success messages from the backend
    Connections {
        target: backend
        enabled: root.visible

        function onSuccess(message) {
            if (message.length > 0) {
                root.statusText = qsTr(message);
                root.statusColor = Material.color(Material.Green);
                statusPopup.open();
            }
        }
    }

    // Handle error messages from the backend
    Connections {
        target: backend
        enabled: root.visible

        function onError(error) {
            if (error.length > 0) {
                root.statusText = qsTr(error);
                root.statusColor = Material.color(Material.Red);
                statusPopup.open();
            }
        }
    }

    Connections {
        target: gtwStatusIndicator

        function onColorChanged() {
            root.gtwRunning = !(gtwStatusIndicator.color === Material.color(Material.Red));
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
