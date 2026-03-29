import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property int itemsHeight: 35

    function getSettings() {
        const data = {
            "protocol": cbxHost.currentText,
            "host": txtHost.text,
            "port": spbPort.value,
            "clientId": txtClientId.text,
            "username": txtUsername.text,
            "password": txtPassword.text,
            "version": cbxVersion.currentText,
            "connectionTimeout": spbTimeout.value,
            "keepAlive": spbKeepAlive.value,
            "autoReconnect": chbAutoReconnect.checked,
            "cleanStart": chbCleanStart.checked,
            "publish": gpbPublish.checked,
            "publishTopic": gpbPublish.topic,
            "publishQos": gpbPublish.qos,
            "subscribe": gpbSubscribe.checked,
            "subscribeTopic": gpbSubscribe.topic,
            "subscribeQos": gpbSubscribe.qos
        }

        return data
    }

    function setSettings(data) {
        cbxHost.currentIndex = cbxHost.find(data.protocol)
        txtHost.text = data.host
        spbPort.value = data.port
        txtClientId.text = data.clientId
        txtUsername.text = data.username
        txtPassword.text = data.password
        cbxVersion.currentIndex = cbxVersion.find(data.version)
        spbTimeout.value = data.connectionTimeout
        spbKeepAlive.value = data.keepAlive
        chbAutoReconnect.checked = data.autoReconnect
        chbCleanStart.checked = data.cleanStart
        gpbPublish.checked = data.publish
        gpbPublish.topic = data.publishTopic
        gpbPublish.qos = data.publishQos
        gpbSubscribe.checked = data.subscribe
        gpbSubscribe.topic = data.subscribeTopic
        gpbSubscribe.qos = data.subscribeQos
    }

    GroupBox {
        id: gpbMqtt
        width: root.width
        height: root.height

        background: Rectangle {
            y: gpbMqtt.topPadding - gpbMqtt.bottomPadding
            width: parent.width
            height: parent.height - gpbMqtt.topPadding + gpbMqtt.bottomPadding
            color: "transparent"
            border.color: Material.color(Material.DeepPurple)
            radius: 8
        }

        ScrollView {
            id: scroller
            anchors.fill: parent

            Item {
                width: parent.width
                implicitHeight: column.implicitHeight

                Column {
                    id: column
                    width: parent.width
                    spacing: 10

                    Row {
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Text {
                            id: lblHost
                            width: 155
                            text: qsTr("Host")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        ComboBox {
                            id: cbxHost
                            model: ["mqtt://", "mqtts://", "ws://", "wss://"]
                            width: 150
                            height: root.itemsHeight
                        }

                        TextField {
                            id: txtHost
                            width: parent.width - lblHost.width - cbxHost.width - 20
                            height: parent.height
                            text: ""
                        }
                    }

                    Row {
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Text {
                            id: lblPort
                            width: lblHost.width
                            text: qsTr("Port")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        SpinBox {
                            id: spbPort
                            width: 150
                            height: parent.height
                            editable: true
                            value: 1883
                            from: 1
                            to: 65535
                            stepSize: 1
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Row {
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Text {
                            id: lblClientId
                            width: lblHost.width
                            text: qsTr("Client ID")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField {
                            id: txtClientId
                            width: (parent.width - 12) - lblClientId.width
                            height: parent.height
                            text: ""
                        }
                    }

                    Row {
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Text {
                            id: lblUsername
                            width: lblHost.width
                            text: qsTr("Username")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField {
                            id: txtUsername
                            width: (parent.width - 12) - lblUsername.width
                            height: parent.height
                            text: ""
                        }
                    }

                    Row {
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Text {
                            id: lblPassword
                            width: lblHost.width
                            text: qsTr("Password")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField {
                            id: txtPassword
                            width: (parent.width - 12) - lblPassword.width
                            height: parent.height
                            text: ""
                            echoMode: TextInput.Password
                        }
                    }

                    Row {
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Text {
                            id: lblVersion
                            width: lblHost.width
                            text: qsTr("MQTT Version")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        ComboBox {
                            id: cbxVersion
                            model: ["3.1", "3.1.1", "5.0"]
                            width: 150
                            height: root.itemsHeight
                        }
                    }

                    Row {
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Text {
                            id: lblTimeout
                            width: lblHost.width
                            text: qsTr("Connection timeout")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        SpinBox {
                            id: spbTimeout
                            width: 150
                            height: parent.height
                            editable: true
                            value: 10
                            from: 1
                            to: 3600
                            stepSize: 1
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: "(s)"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Row {
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Text {
                            id: lblKeepAlive
                            width: lblHost.width
                            text: qsTr("Keep alive")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        SpinBox {
                            id: spbKeepAlive
                            width: 150
                            height: parent.height
                            editable: true
                            value: 60
                            from: 1
                            to: 3600
                            stepSize: 1
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: "(s)"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Row {
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight

                        Switch {
                            id: chbAutoReconnect
                            text: qsTr("Auto reconnet")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Switch {
                            id: chbCleanStart
                            text: qsTr("Clean start")
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Row {
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight * 2.7

                        PubSubGroup {
                            id: gpbPublish
                            width: parent.width

                            text: qsTr("Publish")
                            checked: false
                        }
                    }

                    Row {
                        spacing: 10
                        width: parent.width
                        height: root.itemsHeight * 2.7

                        PubSubGroup {
                            id: gpbSubscribe
                            width: parent.width

                            text: qsTr("Subscribe")
                            checked: false
                        }
                    }
                }
            }
        }
    }
}
