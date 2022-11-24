import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12

ApplicationWindow {
    id: root
    width: 1000
    height: 600
    visible: true
    minimumWidth: 800
    minimumHeight: 400
    title: qsTr("Биржа")

    StackView {
        id: stack
        anchors.fill: parent
        
        initialItem: allRequests
    }
    
    Component {
        id: allRequests

        MainContent {
            controller: contr

            onUserRequestsButtonClicked: stack.push(userRequests)
        }
    }

    Component {
        id: userRequests

        UserRequestsContent {
            controller: contr

            onBackButtonClicked: stack.pop()
            onRequestClicked: stack.push(deals)
        }
    }

    Component {
        id: deals

        RequestDealsContent {
            controller: contr

            onBackButtonClicked: stack.pop()
        }
    }


    Dialog {
        id: dialog
        modal: true
        x: (root.width - width)/2
        y: (root.height - height)/2
        width: 300
        closePolicy: Popup.NoAutoClose

        property var controller: contr

        Connections {
            target: contr
            function onServerEnabledChanged() {
                if (dialog.controller.isServerEnabled) {
                    dialog.close()
                } else {
                    dialog.open()
                }
            }
            function onServerDisabled() {
                if (dialog.controller.isServerEnabled) {
                    dialog.close()
                } else {
                    dialog.open()
                }
            }
        }

        onApplied: dialog.controller.changeServerHost(host.text, parseInt(port.text))

        Overlay.modal: Rectangle {
            color: "#aa303030"
        }

        header: Rectangle {
            height: 42
            color: Material.listHighlightColor

            RowLayout {
                anchors {
                    fill: parent
                    verticalCenter: parent.verticalCenter
                }

                Label {
                    text: "Подключение к серверу"
                    font.pixelSize: 16
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }

        contentItem: Rectangle {
            color: Material.dialogColor
            implicitHeight: 80 + message.contentHeight

            ColumnLayout {
                anchors {
                    fill: parent
                }
                width: parent.width
                spacing: 8

                Material.foreground: Material.secondaryTextColor

                RowLayout {
                    TextField {
                        id: host
                        placeholderText: "Хост"
                        text: "127.0.0.1"
                        Layout.fillWidth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        selectByMouse: true
                    }

                    TextField {
                        id: port
                        placeholderText: "Порт"
                        text: "7070"
                        Layout.fillWidth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        selectByMouse: true
                        validator: IntValidator {
                            bottom: 1
                        }
                    }
                }

                Label {
                    id: message
                    Layout.preferredWidth: 250
                    text: dialog.controller.connectionErrorMessage
                    Material.foreground: Material.Red
                    wrapMode: Label.WrapAnywhere
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }

        footer: DialogButtonBox {

            Button {
                text: "Подключиться"
                flat: true
                DialogButtonBox.buttonRole: DialogButtonBox.ApplyRole
            }
        }
    }
}
