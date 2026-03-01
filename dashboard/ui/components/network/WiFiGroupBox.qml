import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    id: root

    property string ssid: ""
    property string password: ""

    property int itemsHeight: 35

    label: Text {
        text: qsTr("Wi-Fi Settings")
    }

    RowLayout {
        spacing: 10
        height: root.itemsHeight
        layoutDirection: Qt.LeftToRight

        Text {
            Layout.minimumWidth: 60
            text: qsTr("SSID")
        }

        TextField {
            id: txtSSID

            Layout.minimumWidth: 250
            Layout.preferredHeight: parent.height

            height: parent.height
            text: root.ssid

            onTextChanged: {
                root.ssid = text;
            }
        }

        Text {
            Layout.minimumWidth: 60
            text: qsTr("Password")
        }

        TextField {
            id: txtPassword

            Layout.minimumWidth: 250
            Layout.preferredHeight: parent.height

            height: parent.height
            text: root.password

            echoMode: TextInput.Password

            onTextChanged: {
                root.password = text;
            }
        }
    }
}
