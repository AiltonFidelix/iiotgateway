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
            id: rowPolling
            spacing: 10
            width: parent.width
            height: root.itemsHeight

            CheckBox {
                id: chbPolling
                text: "Enable reading polling"
                anchors.verticalCenter: parent.verticalCenter
            }

            SpinBox {
                id: spbPolling
                width: 150
                height: parent.height
                enabled: chbPolling.checked
                value: 1000
                from: 1000
                to: 1000 * 1000
                stepSize: 100
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: "ms"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row {
            id: rowPortAndBaud
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowPolling.bottom
            anchors.margins: 10

            Text {
                id: lblSerialPort
                text: qsTr("Serial Port")
                anchors.verticalCenter: parent.verticalCenter
            }

            TextField {
                id: txtSerialPort
                width: ((parent.width / 2) - 15) - lblSerialPort.width
                height: parent.height
                text: "default"
            }

            Text {
                id: lblBaudrate
                text: qsTr("Baudrate")
                anchors.verticalCenter: parent.verticalCenter
            }

            TextField {
                id: txtBaudrate
                width: ((parent.width / 2) - 15) - lblBaudrate.width
                height: parent.height
                text: "115200"
            }
        }

        Row {
            id: rowParams
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowPortAndBaud.bottom
            anchors.margins: 10

            Text {
                width: lblSerialPort.width
                text: qsTr("Parity")
                anchors.verticalCenter: parent.verticalCenter
            }

            ComboBox {
                id: cbxParity
                model: ["None", "Even", "Odd"]
                width: 150
                height: root.itemsHeight
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: lblDatabits
                text: qsTr("Databits")
                anchors.verticalCenter: parent.verticalCenter
            }

            SpinBox {
                id: spbDatabits
                width: 150
                height: parent.height
                value: 8
                from: 5
                to: 8
                stepSize: 1
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtStopbits
                // width: 60
                text: qsTr("Stopbits")
                anchors.verticalCenter: parent.verticalCenter
            }

            SpinBox {
                id: spbStopbits
                width: 150
                height: parent.height
                value: 1
                from: 1
                to: 2
                stepSize: 1
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row {
            id: rowTimeout
            spacing: 10
            width: parent.width
            height: root.itemsHeight
            anchors.top: rowParams.bottom
            anchors.margins: 10

            Text {
                id: txtTimeout
                width: lblSerialPort.width
                text: qsTr("Timeout")
                anchors.verticalCenter: parent.verticalCenter
            }

            SpinBox {
                id: spbTimeout
                width: 150
                height: parent.height
                value: 1000
                from: 10
                to: 1000 * 1000
                stepSize: 1
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                width: lblDatabits.width
                text: qsTr("Retries")
                anchors.verticalCenter: parent.verticalCenter
            }

            SpinBox {
                id: spbRetries
                width: 150
                height: parent.height
                value: 5
                from: 1
                to: 100
                stepSize: 1
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
