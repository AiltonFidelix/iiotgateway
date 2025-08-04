import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: root

    width: parent.width * 0.95

    height: item.implicitHeight
    radius: 8

    color: "transparent"

    border {
        color: Material.color(Material.DeepPurple)
    }

    anchors {
        horizontalCenter: parent.horizontalCenter
    }

    property int address: 1
    property int startRegister: 0
    property int numberOfEntries: 1
    property string regysterType: "holdingregisters"

    property int itemsHeight: 35

    function getSettings() {
        const data = {
            address: spbAddress.value,
            type: cbxRegisterType.currentValue,
            startRegister: spbStartRegister.value,
            numberOfEntries: spbEntries.value
        }

        return data
    }

    function setSettings(data) {
        spbAddress.value = data.address
        cbxRegisterType.currentText = cbxRegisterType.find(data.type)
        spbStartRegister.value = data.startRegister
        spbEntries.value = data.numberOfEntries
    }

    Item {
        id: item
        width: parent.width
        implicitHeight: column.implicitHeight

        ColumnLayout {
            id: column
            spacing: 10

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            RowLayout {
                width: parent.width
                spacing: 15

                Layout.topMargin: 10
                Layout.rightMargin: 10
                Layout.leftMargin: 10

                Text {
                    text: qsTr("Device address:")
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.preferredWidth: 120
                }

                SpinBox {
                    id: spbAddress
                    Layout.preferredHeight: root.itemsHeight

                    editable: true
                    value: root.address
                    from: 1
                    to: 247
                    stepSize: 1
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: qsTr("Register type:")
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.preferredWidth: 145
                }

                ComboBox {
                    id: cbxRegisterType

                    Layout.preferredHeight: root.itemsHeight
                    Layout.minimumWidth: 200
                    anchors.verticalCenter: parent.verticalCenter

                    textRole: "text"
                    valueRole: "value"

                    Component.onCompleted: currentIndex = indexOfValue(root.regysterType)

                    model: [
                        { value: "coils", text: "Coils" },
                        { value: "discreteinputs", text: "Discrete Inputs" },
                        { value: "holdingregisters", text: "Holding Registers" },
                        { value: "inputregisters", text: "Input Registers" }
                    ]
                }
            }

            RowLayout {
                width: parent.width
                spacing: 15

                Layout.bottomMargin: 10
                Layout.rightMargin: 10
                Layout.leftMargin: 10

                Text {
                    text: qsTr("Start register:")
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.preferredWidth: 120
                }

                SpinBox {
                    id: spbStartRegister
                    Layout.preferredHeight: root.itemsHeight

                    editable: true
                    value: root.startRegister
                    from: 0
                    to: 65534
                    stepSize: 1
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: qsTr("Number of entries:")
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.preferredWidth: 145
                }

                SpinBox {
                    id: spbEntries
                    Layout.preferredHeight: root.itemsHeight

                    editable: true
                    value: root.numberOfEntries
                    from: 0
                    to: 65534
                    stepSize: 1
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
