import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    Column {
        anchors.fill: parent
        spacing: 5

        Row {
            id: rowHeader
            width: parent.width
            height: 50

            Text {
                id: txtTitle
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 15
                text: qsTr("IIoTGateway Settings")
                font.pointSize: 20
                color: Material.color(Material.DeepPurple)
            }

            Button {
                id: logoutButton
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
            // anchors.fill: parent
            width: parent.width
            height: parent.height - rowHeader.height
            spacing: 20

            Column {
                id: colInputProtocol
                spacing: 5
                leftPadding: 20
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.horizontalCenter
                width: parent.width / 2
                height: parent.height - rowHeader.height

                Text {
                    id: txtInputProtocol
                    text: qsTr("Input Protocol")
                    font.pointSize: 15
                    color: Material.color(Material.DeepPurple)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                ComboBox {
                    id: cbxInputProtocol
                    model: ["Modbus RTU"]
                    width: 150
                    height: 35
                    anchors.horizontalCenter: txtInputProtocol.horizontalCenter
                    anchors.top: txtInputProtocol.bottom
                    anchors.margins: 5
                }

                ModbusRTU {
                    id: modbusRTU
                    anchors.top: cbxInputProtocol.bottom
                    anchors.margins: 5
                    visible: true
                    width: parent.width - 20
                    height: parent.height - cbxInputProtocol.height - txtInputProtocol.height
                }
            }

            Column {
                id: colOutputProtocol
                spacing: 5
                leftPadding: 20
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.horizontalCenter
                width: parent.width / 2
                height: parent.height - (txtTitle.height * 2)

                Text {
                    id: txtOutputProtocol
                    text: qsTr("Output Protocol")
                    font.pointSize: 15
                    color: Material.color(Material.DeepPurple)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                ComboBox {
                    id: cbxOutputProtocol
                    model: ["MQTT"]
                    width: 150
                    height: 35
                    anchors.horizontalCenter: txtOutputProtocol.horizontalCenter
                }

                GroupBox {
                    width: 380
                    height: 380
                    anchors.horizontalCenter: cbxOutputProtocol.horizontalCenter
                }
            }
        }
    }
}
