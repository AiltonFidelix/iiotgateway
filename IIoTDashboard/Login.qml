import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property string username: "a"
    property string password: "a"
    property string error: "Failed!"

    signal login

    function clear() {
        usernameField.clear()
        passwordField.clear()
    }

    Popup {
        id: errorPopup
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        anchors.centerIn: parent
        width: 380
        height: 220
        modal: true
        focus: true

        Label {
            id: errorLabel
            anchors.horizontalCenter: parent.horizontalCenter
            y: (root.y / 2) + btnPopup.height
            text: root.error
            font.bold: true
            color: Material.color(Material.Red)
        }

        Button {
            id: btnPopup
            text: qsTr("Ok")
            anchors.bottom: parent.bottom
            anchors.margins: 2
            anchors.horizontalCenter: parent.horizontalCenter
            highlighted: true
            enabled: true
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

    Footer {
        anchors.bottom: root.bottom
        anchors.horizontalCenter: root.horizontalCenter
        anchors.margins: 5
    }

    // Slots Connections

    Connections {
        target: btnPopup

        function onClicked() {
            errorPopup.close()
        }
    }

    Connections {
        id: connSuccess
        target: deviceController
        enabled: root.visible

        function onSuccess(message) {
            root.login()
        }
    }

    Connections {
        id: connError
        target: deviceController
        enabled: root.visible

        function onError(error) {
            root.error = qsTr(error)
            errorPopup.open()
            root.clear()
        }
    }

    Connections {
        target: btnLogin

        function onClicked() {
            deviceController.login(usernameField.text, passwordField.text)
        }
    }
}
