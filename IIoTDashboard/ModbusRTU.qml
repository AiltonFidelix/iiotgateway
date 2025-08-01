import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property int itemsHeight: 35

    function getSettings() {
        const data = {
            "pollingEnabled": chbPolling.checked,
            "pollingTimeout": spbPolling.value,
            "port": txtSerialPort.text,
            "baudrate": txtBaudrate.text,
            "parity": cbxParity.currentText,
            "dataBits": spbDatabits.value,
            "stopBits": spbStopbits.value,
            "retries": spbRetries.value,
            "timeout": spbTimeout.value
        }

        // for (let i = 0; i < listRepeater.count; ++i) {
        //     console.log(i, JSON.stringify(listRepeater.itemAt(i).getSettings()))
        // }

        return data
    }

    function setSettings(data) {
        chbPolling.checked = data.pollingEnabled
        spbPolling.value = data.pollingTimeout
        txtSerialPort.text = data.port
        txtBaudrate.text = data.baudrate
        cbxParity.currentIndex = cbxParity.find(data.parity)
        spbDatabits.value = data.dataBits
        spbStopbits.value = data.retries
        spbTimeout.value = data.timeout

        // TODO: Add list of requests
        // listModel.append([{
        //                       "addressValue": 105,
        //                       "startRegisterValue": 10,
        //                       "numberOfEntriesValue": 12
        //                   }, {
        //                       "addressValue": 240,
        //                       "startRegisterValue": 0,
        //                       "numberOfEntriesValue": 40
        //                   }, {
        //                       "addressValue": 18,
        //                       "startRegisterValue": 8,
        //                       "numberOfEntriesValue": 1
        //                   }])
    }

    GroupBox {
        id: gpbModbusRTU
        anchors.fill: parent

        background: Rectangle {
            y: gpbModbusRTU.topPadding - gpbModbusRTU.bottomPadding
            width: parent.width
            height: parent.height - gpbModbusRTU.topPadding + gpbModbusRTU.bottomPadding
            color: "transparent"
            border.color: Material.color(Material.DeepPurple)
            radius: 8
        }

        ScrollView {
            id: scroller
            anchors.fill: parent

            Item {
                width: parent.width
                implicitHeight: column.implicitHeight

                Column {
                    id: column
                    width: parent.width
                    spacing: 10

                    Row {
                        id: rowPort
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Text {
                            id: lblSerialPort
                            text: qsTr("Serial port")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField {
                            id: txtSerialPort
                            width: (parent.width - 15) - lblSerialPort.width
                            height: parent.height
                            text: "default"
                        }
                    }

                    Row {
                        id: rowBaud
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Text {
                            id: lblBaudrate
                            width: lblSerialPort.width
                            text: qsTr("Baudrate")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField {
                            id: txtBaudrate
                            width: (parent.width - 15) - lblBaudrate.width
                            height: parent.height
                            text: "115200"
                        }
                    }

                    Row {
                        id: rowParams
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Text {
                            width: lblSerialPort.width
                            text: qsTr("Parity")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        ComboBox {
                            id: cbxParity
                            model: ["None", "Even", "Odd"]
                            width: parent.width / 6
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
                            width: parent.width / 6
                            height: parent.height
                            editable: true
                            value: 8
                            from: 5
                            to: 8
                            stepSize: 1
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            id: txtStopbits
                            text: qsTr("Stopbits")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        SpinBox {
                            id: spbStopbits
                            width: parent.width / 6
                            height: parent.height
                            editable: true
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

                        Text {
                            width: lblSerialPort.width
                            text: qsTr("Retries")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        SpinBox {
                            id: spbRetries
                            width: parent.width / 6
                            height: parent.height
                            editable: true
                            value: 5
                            from: 1
                            to: 100
                            stepSize: 1
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            id: txtTimeout
                            width: lblDatabits.width
                            text: qsTr("Timeout")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        SpinBox {
                            id: spbTimeout
                            width: parent.width / 6
                            height: parent.height
                            editable: true
                            value: 1000
                            from: 10
                            to: 1000 * 1000
                            stepSize: 50
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: "(ms)"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Row {
                        id: rowPolling
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Switch {
                            id: chbPolling
                            text: "Enable reading polling"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        SpinBox {
                            id: spbPolling
                            width: 150
                            height: parent.height
                            enabled: chbPolling.checked
                            editable: true
                            value: 1000
                            from: 1000
                            to: 1000 * 1000
                            stepSize: 50
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: "(ms)"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    ListModel {
                        id: listModel
                    }

                    Repeater {
                        id: listRepeater
                        model: listModel
                        delegate: ModbusRequest {
                            address: addressValue
                            startRegister: startRegisterValue
                            numberOfEntries: numberOfEntriesValue
                            regysterType: registerTypeValue
                        }
                    }
                }
            }
        }
    }
}
