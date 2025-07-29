import QtQuick
import QtQuick.Controls.Material

Rectangle {
    id: root

    height: 150
    width: parent.width * 0.98

    radius: 8

    color: "transparent"

    border {
        color: Material.color(Material.DeepPurple)
    }

    anchors {
        horizontalCenter: parent.horizontalCenter
    }
}
