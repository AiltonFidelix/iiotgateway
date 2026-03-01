import QtQuick
import QtQuick.Controls.Material

Item {
    id: root

    Text {
        text: qsTr("<b>IIoTGateway</b> - by Ailton Fidelix")
        anchors.bottom: root.bottom
        anchors.horizontalCenter: root.horizontalCenter
        color: Material.color(Material.DeepPurple)
    }
}
