import QtQuick
import QtQuick.Controls

Button {
    id: root
    width: parent.width

    background: Rectangle {
        color: root.hovered ? Material.color(Material.DeepPurple) : "transparent"
    }

    contentItem: Text {
        text: root.text
        font.bold: root.font.bold
        color: root.hovered ? "#FFF" : Material.color(Material.DeepPurple)
    }
}
