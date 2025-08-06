import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property int itemsHeight: 35

    function addRequestItem(data) {
        if (typeof (data) === "undefined") {
            listModel.append({
                                 "addressValue": 1,
                                 "startRegisterValue": 0,
                                 "numberOfEntriesValue": 1,
                                 "registerTypeValue": "holdingregisters"
                             });
        } else {
            listModel.append({
                                 "addressValue": data.address,
                                 "startRegisterValue": data.startRegister,
                                 "numberOfEntriesValue": data.numberOfEntries,
                                 "registerTypeValue": data.registerType
                             });
        }
    }

    function removeRequestItem() {
        listModel.remove(listModel.count - 1);
    }

    // TODO: Change all JSON objects for QObject DTOs
    function getSettings() {
        let requests = [];

        for (var i = 0; i < listRepeater.count; ++i) {
            requests.push(listRepeater.itemAt(i).getSettings());
        }

        const data = {
            "pollingEnabled": chbPolling.checked,
            "pollingTimeout": spbPolling.value,
            "port": txtSerialPort.text,
            "baudrate": txtBaudrate.text,
            "parity": cbxParity.currentText,
            "dataBits": spbDatabits.value,
            "stopBits": spbStopbits.value,
            "retries": spbRetries.value,
            "timeout": spbTimeout.value,
            "requests": requests
        }

        return data;
    }

    function setSettings(data) {
        chbPolling.checked = data.pollingEnabled;
        spbPolling.value = data.pollingTimeout;
        txtSerialPort.text = data.port;
        txtBaudrate.text = data.baudrate;
        cbxParity.currentIndex = cbxParity.find(data.parity);
        spbDatabits.value = data.dataBits;
        spbStopbits.value = data.stopBits;
        spbRetries.value = data.retries;
        spbTimeout.value = data.timeout;

        listModel.clear();

        data.requests.forEach(function (request) {
            root.addRequestItem(request);
        })
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

                    Row {
                        spacing: 10
                        width: parent.width
                        rightPadding: 9
                        layoutDirection: Qt.RightToLeft

                        Button {
                            id: addRequest

                            text: "<b>+</b>"
                            enabled: chbPolling.checked
                            visible: chbPolling.checked
                            highlighted: true

                            onClicked: root.addRequestItem();
                        }

                        Button {
                            id: removeRequest

                            text: "<b>-</b>"
                            enabled: chbPolling.checked && (listModel.count > 1)
                            visible: chbPolling.checked && (listModel.count > 1)
                            highlighted: true

                            onClicked: root.removeRequestItem();
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: chbPolling

        function onClicked() {
            if (chbPolling.checked) {
                root.addRequestItem();
            } else {
                listModel.clear();
            }
        }
    }
}
