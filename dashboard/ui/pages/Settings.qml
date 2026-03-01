import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components/network"

Page {
    id: root
    title: "Settings"

    property int itemsHeight: 35

    Component.onCompleted: {
        backend.requestNetworkSettings();
    }

    function getNetworkSettings() {
        let data = {
            "network": connectionType.currentValue,
            "method": methodType.currentValue,
        };

        if (methodType.currentIndex === 1) {
            data["ipv4"] = gpbIp.ipv4;
            data["netmask"] = gpbIp.netmask;
            data["router"] = gpbIp.router;
            data["dns"] = gpbIp.dns;
        }

        if (connectionType.currentIndex === 1) {
            data["wifi"] = {
                "ssid": gpbWiFi.ssid,
                "password": gpbWiFi.password
            };
        }

        return data;
    }

    function setNetworkSettings(data) {
        connectionType.currentIndex = findIndexByValue(connectionType.model, data.network);
        methodType.currentIndex = findIndexByValue(methodType.model, data.method);

        if (methodType.currentIndex === 1) {
            gpbIp.ipv4 = data.ipv4;
            gpbIp.netmask = data.netmask;
            gpbIp.router = data.router;
            gpbIp.dns = data.dns;
        }

        if (connectionType.currentIndex === 1) {
            gpbWiFi.ssid = data["wifi"].ssid;
            gpbWiFi.password = data["wifi"].password;
        }
    }

    function findIndexByValue(model, value) {
        for (let i = 0; i < model.length; i++) {
            if (model[i].value === value)
                return i;
        }
        return -1
    }

    ColumnLayout {
        width: parent.width
        spacing: 10

        Text {
            Layout.topMargin: 10
            Layout.alignment: Qt.AlignCenter

            text: qsTr("Network")
            font.pointSize: 15
            color: Material.color(Material.DeepPurple)
        }

        GroupBox {
            id: gpbNetwork

            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.fillWidth: true

            background: Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: Material.color(Material.DeepPurple)
                radius: 8
            }

            ColumnLayout {
                width: parent.width
                spacing: 10

                RowLayout {
                    spacing: 10

                    Text {
                        text: qsTr("Network type")
                        Layout.preferredWidth: 110
                    }

                    ComboBox {
                        id: connectionType

                        Layout.preferredHeight: root.itemsHeight
                        Layout.minimumWidth: 200
                        textRole: "text"
                        valueRole: "value"

                        model: [
                            { value: "eth0", text: "Ethernet" },
                            { value: "wlan0", text: "Wi-Fi" }
                        ]
                    }

                    Text {
                        text: qsTr("Method type")
                        Layout.preferredWidth: 110
                    }

                    ComboBox {
                        id: methodType

                        Layout.preferredHeight: root.itemsHeight
                        Layout.minimumWidth: 200

                        textRole: "text"
                        valueRole: "value"

                        model: [
                            { value: "auto", text: "Auto" },
                            { value: "manual", text: "Manual" }
                        ]
                    }
                }

                WiFiGroupBox {
                    id: gpbWiFi
                    Layout.fillWidth: true
                    visible: connectionType.currentIndex === 1
                }

                IpGroupBox {
                    id: gpbIp
                    Layout.fillWidth: true
                    visible: methodType.currentIndex === 1
                }
            }
        }
    }

    Button {
        id: btnSave
        text: qsTr("Save")
        anchors.bottom: parent.bottom
        anchors.margins: 8
        anchors.right: parent.right
        highlighted: true
        enabled: true
    }

    Button {
        id: btnReboot
        text: qsTr("Reboot")
        anchors.bottom: parent.bottom
        anchors.margins: 8
        anchors.right: btnSave.left
        highlighted: true
        enabled: true

        onClicked: backend.reboot();
    }

    // Save gateway network settings
    Connections {
        target: btnSave

        function onClicked() {
            let settings = root.getNetworkSettings();
            backend.setNetworkSettings(JSON.stringify(settings));
        }
    }

    // Handle gateway network settings comming from the backend
    Connections {
        target: backend

        function onNetworkSettings(settings) {
            let data = JSON.parse(JSON.parse(String(settings)).message);
            root.setNetworkSettings(data);
        }
    }
}
