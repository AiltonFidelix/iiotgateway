import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    id: root

    property string text: ""
    property string topic: ""
    property bool checked: false
    property int qos: 0
    property int itemsHeight: 35

    label: Switch {
        id: checkbox
        text: root.text
        checked: root.checked

        onCheckedChanged: {
            root.checked = checked;
        }
    }

    RowLayout {
        spacing: 10
        width: parent.width
        height: root.itemsHeight

        Text {
            text: qsTr("Topic")
            enabled: checkbox.checked
        }

        TextField {
            id: txtTopic

            Layout.fillWidth: true
            Layout.preferredHeight: parent.height

            height: parent.height
            text: root.topic
            enabled: checkbox.checked

            onTextChanged: {
                root.topic = text;
            }
        }

        Text {
            text: qsTr("QOS")
            enabled: checkbox.checked
        }

        SpinBox {
            id: spbQos

            Layout.preferredWidth: 150
            Layout.preferredHeight: parent.height

            editable: true
            value: root.qos
            from: 0
            to: 2
            stepSize: 1
            enabled: checkbox.checked

            onValueChanged: {
                root.qos = value;
            }
        }
    }
}
