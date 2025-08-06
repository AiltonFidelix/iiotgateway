import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components/network"

Page {
    id: root
    title: "Settings"

    property int itemsHeight: 35

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

                        // Component.onCompleted: currentIndex = indexOfValue(root.regysterType)

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

                        // Component.onCompleted: currentIndex = indexOfValue(root.regysterType)

                        model: [
                            { value: "dhcp", text: "DHCP" },
                            { value: "manual", text: "Manual" }
                        ]
                    }

                    // TODO: Add SSID and password if Wi-Fi
                    // TODO: Add IPv4 address if manually
                    // TODO: Add Net mask if manually
                    // TODO: Add Router/Gateway if manually
                    // TODO: Add DNS if manually
                }

                WiFiGroupBox {
                    id: gpbWiFi

                    Layout.fillWidth: true

                    visible: connectionType.currentIndex === 1
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
    }
}
