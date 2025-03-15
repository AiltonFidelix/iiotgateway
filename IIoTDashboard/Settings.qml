import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    signal logout

    Column {
        anchors.fill: parent
        spacing: 10

        Row {
            id: rowHeader
            width: parent.width
            height: 50

            Text {
                id: txtTitle
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 5
                text: qsTr("IIoTGateway Settings")
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
                spacing: 5
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
                    visible: cbxEdgeProtocol.currentText == "Modbus RTU"
                    width: parent.width - 20
                    height: parent.height - txtEdgeProtocol.height - cbxEdgeProtocol.height
                }
            }

            Column {
                id: colCloudProtocol
                spacing: 5
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
                    visible: cbxCloudProtocol.currentText == "MQTT"
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
    }
}
