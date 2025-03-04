import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    GroupBox {
        width: root.width
        height: root.height

        CheckBox {
            id: chbPolling
            text: "Enable reading pollling"
        }

        TextField {
            id: txtSerialPort
            anchors.top: chbPolling.bottom
            placeholderText: "Serial Port"
        }
    }
}
