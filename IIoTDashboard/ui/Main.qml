import QtQuick
import QtQuick.Window
import QtQuick.Controls.Material
import "components"
import "pages"

ApplicationWindow {
    id: app

    width: 1024
    height: 768
    visible: true
    title: qsTr("IIoTGateway")

    Material.theme: Material.Light
    Material.accent: Material.DeepPurple
    Material.primary: Material.DeepPurple

    function pushPage(page) {
        let objectName = page + "Page";

        if (stackView.currentItem.objectName !== objectName) {
            if (stackView.depth >= 1) {
                stackView.pop();
            }

            stackView.push("pages/" + page + ".qml", { objectName });
        }

        drawer.close();
    }

    header: ToolBar {
        id: toolBar
        visible: stackView.depth > 1

        contentHeight: toolButton.implicitHeight

        Rectangle {
            color: Material.color(Material.DeepPurple)
            anchors.fill: parent

            ToolButton {
                id: toolButton
                text: qsTr("⋮")
                font.pixelSize: Qt.application.font.pixelSize * 1.8

                onClicked: drawer.open()
            }

            Label {
                text: stackView.currentItem.title
                anchors.centerIn: parent
                font.pointSize: 22
                font.bold: true
                color: "transparent"
            }
        }
    }

    footer: Footer {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 5
    }

    Drawer {
        id: drawer
        width: parent.width * 0.12
        height: parent.height

        Column {
            anchors.fill: parent

            Button {
                id: btnHide
                text: qsTr("‹")
                width: parent.width

                background: Rectangle {
                    color: btnHide.hovered ? Material.color(Material.DeepPurple) : "transparent"
                }

                contentItem: Text {
                    text: btnHide.text
                    color: btnHide.hovered ? "#FFF" : Material.color(Material.DeepPurple)
                }

                onClicked: drawer.close();
            }

            ItemDelegate {
                id: settings
                text: qsTr("Settings")
                width: parent.width

                background: Rectangle {
                    color: settings.hovered ? Material.color(Material.DeepPurple) : "transparent"
                }

                contentItem: Text {
                    text: settings.text
                    color: settings.hovered ? "#FFF" : Material.color(Material.DeepPurple)
                }

                onClicked: app.pushPage("Settings");
            }

            ItemDelegate {
                id: communication
                text: qsTr("Communication")
                width: parent.width

                background: Rectangle {
                    color: communication.hovered ? Material.color(Material.DeepPurple) : "transparent"
                }

                contentItem: Text {
                    text: communication.text
                    color: communication.hovered ? "#FFF" : Material.color(Material.DeepPurple)
                }

                onClicked: app.pushPage("Communication");
            }
        }

        Button {
            id: btnLogout
            text: qsTr("Logout")
            width: parent.width

            anchors.bottom: parent.bottom

            background: Rectangle {
                color: btnLogout.hovered ? Material.color(Material.DeepPurple) : "transparent"
            }

            contentItem: Text {
                text: btnLogout.text
                font.bold: true
                color: btnLogout.hovered ? "#FFF" : Material.color(Material.DeepPurple)
            }

            // onClicked: drawer.close();
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Login {
            id: login
        }
    }

    // Handle login
    Connections {
        target: login

        function onLoginSuccess() {
            toolBar.visible = true;
            app.pushPage("Settings");
        }
    }

    // Handle logout
    Connections {
        target: btnLogout

        function onClicked() {
            drawer.close();
            stackView.pop();
            toolBar.visible = false;
        }
    }
}
