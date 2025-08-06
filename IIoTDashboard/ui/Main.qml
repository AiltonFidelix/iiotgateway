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

                onClicked: drawer.open();
            }

            Label {
                text: stackView.currentItem.title
                anchors.centerIn: parent
                font.pointSize: 20
                color: "#FFF"
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

            MenuButton {
                id: btnHide
                text: qsTr("‹")

                onClicked: drawer.close();
            }

            MenuItemDelegate {
                id: settings
                text: qsTr("Settings")

                onClicked: app.pushPage("Settings");
            }

            MenuItemDelegate {
                id: communication
                text: qsTr("Communication")

                onClicked: app.pushPage("Communication");
            }
        }

        MenuButton {
            id: btnLogout
            text: qsTr("Logout")
            font.bold: true

            anchors.bottom: parent.bottom
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
