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

        TextField {
            id: txtSerialPort
            width: 345
            height: root.itemsHeight
            anchors.top: rowPolling.bottom
            anchors.margins: 10
            placeholderText: "Serial Port"
            text: "default"
        }

        TextField {
            id: txtBaudrate
            width: 345
            height: root.itemsHeight
            anchors.top: txtSerialPort.bottom
            anchors.margins: 10
            placeholderText: "Baudrate"
            text: "115200"
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

        ComboBox {
            id: cbxParity
            model: ["None", "Even", "Odd"]
            width: 150
            height: root.itemsHeight
        }
    }
}
