import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property int itemsHeight: 35

    function getSettings() {
        const data = {
            protocol: cbxHost.currentText,
            host: txtHost.text,
            port: spbPort.value,
            clientId: txtClientId.text,
            username: txtUsername.text,
            password: txtPassword.text,
            version: cbxVersion.currentText,
            connectionTimeout: spbTimeout.value,
            keepAlive: spbKeepAlive.value,
            autoReconnect: chbAutoReconnect.checked,
            cleanStart: chbCleanStart.checked,
            publish: chbPublish.checked,
            publishTopic: txtPublishTopic.text,
            publishQos: spbQosPublish.value,
            subscribe: chbSubscribe.checked,
            subscribeTopic: txtSubscribeTopic.text,
            subscribeQos: spbQosSubscribe.value
        }

        return data
    }

    function setSettings(data) {
        cbxHost.currentIndex =  cbxHost.find(data.protocol)
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
        chbPublish.checked = data.publish
        txtPublishTopic.text = data.publishTopic
        spbQosPublish.value = data.publishQos
        chbSubscribe.checked = data.subscribe
        txtSubscribeTopic.text = data.subscribeTopic
        spbQosSubscribe.value = data.subscribeQos
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
            width: parent.width
            height: parent.height

            Column {
                id: column
                width: parent.width

                Row {
                    id: rowHost
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
                    id: rowPort
                    spacing: 10
                    width: parent.width
                    height: root.itemsHeight
                    anchors.top: rowHost.bottom
                    anchors.margins: 10

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
                    id: rowClientId
                    spacing: 10
                    width: parent.width
                    height: root.itemsHeight
                    anchors.top: rowPort.bottom
                    anchors.margins: 10

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
                    id: rowUsername
                    spacing: 10
                    width: parent.width
                    height: root.itemsHeight
                    anchors.top: rowClientId.bottom
                    anchors.margins: 10

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
                    id: rowPassword
                    spacing: 10
                    width: parent.width
                    height: root.itemsHeight
                    anchors.top: rowUsername.bottom
                    anchors.margins: 10

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
                    id: rowVersion
                    spacing: 10
                    width: parent.width
                    height: root.itemsHeight
                    anchors.top: rowPassword.bottom
                    anchors.margins: 10

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
                    id: rowTimeout
                    spacing: 10
                    width: parent.width
                    height: root.itemsHeight
                    anchors.top: rowVersion.bottom
                    anchors.margins: 10

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
                    id: rowKeepAlive
                    spacing: 10
                    width: parent.width
                    height: root.itemsHeight
                    anchors.top: rowTimeout.bottom
                    anchors.margins: 10

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
                    id: rowSwitch
                    spacing: 10
                    width: parent.width
                    height: root.itemsHeight
                    anchors.top: rowKeepAlive.bottom
                    anchors.margins: 10

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
                    id: rowPublish
                    spacing: 10
                    width: parent.width
                    height: root.itemsHeight * 2.7
                    anchors.top: rowSwitch.bottom
                    anchors.margins: 10

                    GroupBox {
                        id: gpbPublish
                        anchors.fill: parent

                        label: Switch {
                            id: chbPublish
                            text: qsTr("Publish")
                            checked: false
                        }

                        Row {
                            spacing: 10
                            width: parent.width
                            height: root.itemsHeight

                            Text {
                                text: qsTr("Topic")
                                anchors.verticalCenter: parent.verticalCenter
                                enabled: chbPublish.checked
                            }

                            TextField {
                                id: txtPublishTopic
                                anchors.verticalCenter: parent.verticalCenter
                                width: 300
                                height: parent.height
                                text: ""
                                enabled: chbPublish.checked
                            }

                            Text {
                                text: qsTr("QOS")
                                anchors.verticalCenter: parent.verticalCenter
                                enabled: chbPublish.checked
                            }

                            SpinBox {
                                id: spbQosPublish
                                width: 150
                                height: parent.height
                                editable: true
                                value: 0
                                from: 0
                                to: 2
                                stepSize: 1
                                anchors.verticalCenter: parent.verticalCenter
                                enabled: chbPublish.checked
                            }
                        }
                    }
                }

                Row {
                    id: rowSubscribe
                    spacing: 10
                    width: parent.width
                    height: root.itemsHeight * 2.7
                    anchors.top: rowPublish.bottom
                    anchors.margins: 10

                    GroupBox {
                        id: gpbSubscribe
                        anchors.fill: parent

                        label: Switch {
                            id: chbSubscribe
                            text: qsTr("Subscribe")
                            checked: false
                        }

                        Row {
                            spacing: 10
                            width: parent.width
                            height: root.itemsHeight

                            Text {
                                text: qsTr("Topic")
                                anchors.verticalCenter: parent.verticalCenter
                                enabled: chbSubscribe.checked
                            }

                            TextField {
                                id: txtSubscribeTopic
                                width: 300
                                height: parent.height
                                text: ""
                                enabled: chbSubscribe.checked
                            }

                            Text {
                                text: qsTr("QOS")
                                anchors.verticalCenter: parent.verticalCenter
                                enabled: chbSubscribe.checked
                            }

                            SpinBox {
                                id: spbQosSubscribe
                                width: 150
                                height: parent.height
                                editable: true
                                value: 0
                                from: 0
                                to: 2
                                stepSize: 1
                                anchors.verticalCenter: parent.verticalCenter
                                enabled: chbSubscribe.checked
                            }
                        }
                    }
                }
            }
        }
    }
}
