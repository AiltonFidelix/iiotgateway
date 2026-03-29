import QtQuick
import QtQuick.Controls
import "../components"

Page {
    id: root

    property string username: ""
    property string password: ""
    property string error: "Failed!"

    signal loginSuccess

    function clear() {
        usernameField.clear()
        passwordField.clear()
    }

    InfoPopUp {
        id: errorPopup
        statusText: root.error
        statusColor: Material.color(Material.Red)
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

    Connections {
        id: connSuccess
        target: backend
        enabled: root.visible

        function onSuccess(message) {
            root.clear();
            root.loginSuccess();
        }
    }

    Connections {
        id: connError
        target: backend
        enabled: root.visible

        function onError(error) {
            root.error = qsTr(error);
            errorPopup.open();
            root.clear();
        }
    }

    Connections {
        target: btnLogin

        function onClicked() {
            backend.login(usernameField.text, passwordField.text);
        }
    }
}
