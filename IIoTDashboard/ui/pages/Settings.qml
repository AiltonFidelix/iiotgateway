import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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

            RowLayout {
                spacing: 10

                Text {
                    text: qsTr("Network type:")
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.preferredWidth: 110
                }

                ComboBox {
                    id: connectionType

                    Layout.preferredHeight: root.itemsHeight
                    Layout.minimumWidth: 200
                    anchors.verticalCenter: parent.verticalCenter

                    textRole: "text"
                    valueRole: "value"

                    // Component.onCompleted: currentIndex = indexOfValue(root.regysterType)

                    model: [
                        { value: "wlan0", text: "Wi-Fi" },
                        { value: "eth0", text: "Ethernet" }
                    ]
                }

                Text {
                    text: qsTr("Method type:")
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.preferredWidth: 110
                }

                ComboBox {
                    id: methodType

                    Layout.preferredHeight: root.itemsHeight
                    Layout.minimumWidth: 200
                    anchors.verticalCenter: parent.verticalCenter

                    textRole: "text"
                    valueRole: "value"

                    // Component.onCompleted: currentIndex = indexOfValue(root.regysterType)

                    model: [
                        { value: "dhcp", text: "DHCP" },
                        { value: "manual", text: "Manually" }
                    ]
                }

                // TODO: Add SSID and password if Wi-Fi
                // TODO: Add IPv4 address if manually
                // TODO: Add Net mask if manually
                // TODO: Add Router/Gateway if manually
                // TODO: Add DNS if manually
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
}
