import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property string username: "a"
    property string password: "a"
    property string status: "Failed!"

    signal login

    function clear() {
        usernameField.clear()
        passwordField.clear()
    }

    Popup {
        id: statusPopup
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        anchors.centerIn: parent
        width: 280
        height: 180
        modal: true
        focus: true

        Label {
            id: statusLabel
            anchors.horizontalCenter: parent.horizontalCenter
            y: (root.y / 2) + btnStatus.height
            text: root.status
            font.bold: true
            color: Material.color(Material.DeepPurple)
        }

        Button {
            id: btnStatus
            text: qsTr("Close")
            anchors.bottom: parent.bottom
            anchors.margins: 2
            anchors.horizontalCenter: parent.horizontalCenter
            highlighted: true
            enabled: true
        }

        Connections {
            target: btnStatus

            function onClicked() {
                statusPopup.close()
            }
        }
    }

    Rectangle {
        anchors.centerIn: parent

        TextField {
            id: usernameField
            text: qsTr("")
            placeholderText: qsTr("Username")
            anchors.horizontalCenter: parent.horizontalCenter
            y: (root.y / 2) - 90
            height: 45
            width: 250
        }

        TextField {
            id: passwordField
            text: qsTr("")
            placeholderText: qsTr("Password")
            echoMode: TextInput.Password
            anchors.top: usernameField.bottom
            anchors.margins: 10
            anchors.horizontalCenter: parent.horizontalCenter
            height: usernameField.height
            width: usernameField.width
            Material.foreground: Material.DeepPurple
        }

        Button {
            id: btnLogin
            text: qsTr("Login")
            anchors.top: passwordField.bottom
            anchors.margins: 5
            anchors.horizontalCenter: parent.horizontalCenter
            width: passwordField.width
            highlighted: (usernameField.text.length > 0) && (passwordField.text.length > 0)
            enabled: (usernameField.text.length > 0) && (passwordField.text.length > 0)
        }
    }

    Text {
        text: qsTr("<b>IIoTGateway</b> - by Ailton Fidelix")
        anchors.bottom: root.bottom
        anchors.horizontalCenter: root.horizontalCenter
        anchors.margins: 5
        color: Material.color(Material.DeepPurple)
    }

    Connections {
        target: btnLogin

        function onClicked() {
            // TODO: get credentials from C++ side
            if ((usernameField.text == root.username) && (passwordField.text == root.password)) {
                root.login()
            }
            else {
                root.status = "Login failed, wrong credentials!"
                statusPopup.open()
                root.clear()
            }
        }
    }
}
