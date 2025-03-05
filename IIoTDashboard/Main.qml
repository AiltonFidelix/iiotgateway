import QtQuick
import QtQuick.Window
import QtQuick.Controls.Material

Window {
    width: 1024
    height: 768
    visible: true
    title: qsTr("IIoTGateway")

    Material.theme: Material.Light
    Material.accent: Material.DeepPurple
    Material.primary: Material.DeepPurple

    Login {
        id: loginScreen
        anchors.fill: parent
        visible: true
    }

    Settings {
        id: settingsScreen
        anchors.fill: parent
        visible: false
    }

    Connections {
        target: loginScreen

        function onLogin() {
            settingsScreen.visible = true
            loginScreen.visible = false
        }
    }

    Connections {
        target: settingsScreen

        function onLogout() {
            settingsScreen.visible = false
            loginScreen.visible = true
            loginScreen.clear()
        }
    }
}
