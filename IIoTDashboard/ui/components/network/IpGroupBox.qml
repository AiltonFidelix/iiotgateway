import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    id: root

    property string ipv4: "0.0.0.0"
    property string netmask: "0.0.0.0"
    property string router: "0.0.0.0"
    property string dns: "0.0.0.0"

    property int itemsHeight: 35

    label: Text {
        text: qsTr("IP Settings")
    }

    RowLayout {
        spacing: 10
        height: root.itemsHeight
        layoutDirection: Qt.LeftToRight

        Text {
            Layout.minimumWidth: 60
            text: qsTr("IPv4")
        }

        TextField {
            id: txIpv4

            Layout.minimumWidth: 250
            Layout.preferredHeight: parent.height

            text: root.ipv4
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: RegularExpressionValidator {
                regularExpression: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
            }

            onTextChanged: {
                root.ipv4 = text
            }
        }

        Text {
            Layout.minimumWidth: 60
            text: qsTr("Netmask")
        }

        TextField {
            id: txtNetMask

            Layout.minimumWidth: 250
            Layout.preferredHeight: parent.height

            text: root.netmask
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: RegularExpressionValidator {
                regularExpression: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
            }

            onTextChanged: {
                root.netmask = text
            }
        }

        Text {
            Layout.minimumWidth: 60
            text: qsTr("Router")
        }

        TextField {
            id: txtRouter

            Layout.minimumWidth: 250
            Layout.preferredHeight: parent.height

            text: root.router
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: RegularExpressionValidator {
                regularExpression: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
            }

            onTextChanged: {
                root.router = text
            }
        }

        Text {
            Layout.minimumWidth: 60
            text: qsTr("DNS")
        }

        TextField {
            id: txtDns

            Layout.minimumWidth: 250
            Layout.preferredHeight: parent.height

            text: root.dns
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: RegularExpressionValidator {
                regularExpression: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
            }

            onTextChanged: {
                root.dns = text
            }
        }
    }
}
