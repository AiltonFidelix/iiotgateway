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

    // Text {
    //     id: welcome
    //     text: qsTr("Welcome to the <b>IIoTGateway</b> system...")
    //     anchors.centerIn: parent
    //     color: Material.color(Material.DeepPurple)
    //     visible: !loginScreen.visible
    // }

    Settings {
        id: settingsScreen
        anchors.fill: parent
        visible: !loginScreen.visible
    }

    Login {
        id: loginScreen
        anchors.fill: parent
        visible: true
    }
}
