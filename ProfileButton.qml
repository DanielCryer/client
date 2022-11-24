import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.15

TabButton {
    id: root

    property var rootItem
    property var controller

    signal userRequestsButtonClicked

    onClicked: {
        if (!controller.isAuthorized)
            dialog.open()
        else if (menu.opened)
            menu.close()
        else {
            controller.updateUserData()
            menu.open()
        }
    }

    contentItem: Label{
        anchors {
            fill: parent
            margins: 14
        }
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        text: controller.isAuthorized ? controller.user.name : "Вход"
        font.pixelSize: 16
        Material.foreground: root.hovered || menu.opened ? Material.iconColor : Material.secondaryTextColor
    }

    Menu {
        id: menu
        width: 140
        height: con.height + 14
        y: parent.height
        x: parent.width - width
        closePolicy: Popup.CloseOnPressOutsideParent | Popup.CloseOnEscape
        background: Rectangle {
            color: Material.backgroundColor
            border {
                width: 2
                color: Material.dividerColor
            }

            radius: 4
        }

        ColumnLayout {
            id: con
            spacing: 8

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 14

                Image {
                    Layout.preferredHeight: 18
                    Layout.preferredWidth: 18
                    Layout.leftMargin: -6
                    Layout.rightMargin: -4
                    source: "icons/icon-usd.svg"

                    ColorOverlay {
                        anchors.fill: parent
                        source: parent
                        opacity: 0.75
                        color: Material.backgroundDimColor
                    }
                }

                Label {
                    text: controller.user.usdBalance
                    font.pixelSize: 16
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                Image {
                    Layout.preferredHeight: 16
                    Layout.preferredWidth: 16
                    Layout.leftMargin: -6
                    Layout.rightMargin: -4
                    source: "icons/icon-rub.svg"

                    ColorOverlay {
                        anchors.fill: parent
                        source: parent
                        opacity: 0.75
                        color: Material.backgroundDimColor
                    }
                }

                Label {
                    text: controller.user.rubBalance
                    font.pixelSize: 16
                }
            }

            Item {
                Layout.fillHeight: true
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                text: "Мои заявки"

                onClicked: {
                    menu.close()
                    userRequestsButtonClicked()
                }
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                text: "Выход"
                flat: true
                font.pixelSize: 12
                Material.foreground: Material.secondaryTextColor
                onClicked: {
                    menu.close();
                    controller.signOut();
                }
            }
        }
    }





    Dialog {
        id: dialog
        modal: true
        x: (root.rootItem.width - width)/2
        y: (root.rootItem.height - height)/2
        parent: root.rootItem
        padding: 0

        onClosed: {
            controller.registrationStatus = "";
            controller.authorizationStatus = "";
            if (stack.depth == 2)
                stack.pop();
        }

        Overlay.modal: Rectangle {
            color: "#aa303030"
        }

        contentItem: Rectangle {
            anchors.fill: parent
            implicitHeight: 300
            implicitWidth: 280
            color: "#aa303030"

            StackView {
                id: stack
                anchors.fill: parent
                initialItem: signIn
            }

            Component {
                id: signIn

                Page {
                    header: Rectangle {
                        height: 42
                        color: Material.listHighlightColor

                        Label {
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: "Вход"
                            font{
                                pixelSize: 16
                            }
                        }
                    }

                    contentItem: Rectangle {
                        color: Material.backgroundColor

                        ColumnLayout {
                            anchors {
                                fill: parent
                                margins: 28
                            }
                            spacing: 8

                            TextField {
                                id: emailFolder
                                Layout.fillWidth: true
                                placeholderText: "Email"
                                selectByMouse: true
                                validator: RegExpValidator {
                                    regExp: /^[\w-\.]+@([\w-]+\.)+[\w-]{2,4}$/
                                }
                            }

                            TextField {
                                id: passFolder
                                Layout.fillWidth: true
                                placeholderText: "Password"
                                echoMode: TextInput.Password
                                selectByMouse: true
                            }

                            Label {
                                text: controller.authorizationStatus
                                Material.foreground: Material.Red
                                wrapMode: Text.WordWrap
                                onTextChanged: {
                                    if (text == "Successful")
                                        dialog.close();
                                }
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }

                    footer: Rectangle {
                        height: 50
                        color: Material.listHighlightColor

                        RowLayout {
                            anchors{
                                fill: parent
                                leftMargin: 14
                                rightMargin: 14
                            }
                            Button {
                                text: "Создать аккаунт"
                                flat: true
                                font.pixelSize: 12

                                onPressed: stack.push(signUp)
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Button {
                                text: "Войти"
                                onClicked: {
                                    if (emailFolder.acceptableInput)
                                        controller.signIn(emailFolder.text, passFolder.text)
                                }
                            }
                        }
                    }
                }
            }

            Component {
                id: signUp

                Page {
                    header: Rectangle {
                        height: 42
                        color: Material.listHighlightColor

                        Button {
                            icon.source: "icons/icon-back.svg"
                            icon.height: 20
                            icon.width: 20
                            icon.color: pressed ? Material.iconColor : hovered ? Material.secondaryTextColor : Material.iconDisabledColor

                            onPressed: stack.pop()

                            background: Item {}
                        }

                        Label {
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: "Создать аккаунт"
                            font{
                                pixelSize: 16
                            }
                        }
                    }

                    contentItem: Rectangle {
                        color: Material.backgroundColor

                        ColumnLayout {
                            anchors {
                                fill: parent
                                margins: 28
                            }
                            spacing: 8

                            TextField {
                                id: emailFolder
                                Layout.fillWidth: true
                                placeholderText: "Email"
                                selectByMouse: true
                                validator: RegExpValidator {
                                    regExp: /^[\w-\.]+@([\w-]+\.)+[\w-]{2,4}$/
                                }
                            }

                            TextField {
                                id: nameFolder
                                Layout.fillWidth: true
                                placeholderText: "Name"
                                selectByMouse: true
                            }

                            TextField {
                                id: passFolder
                                Layout.fillWidth: true
                                placeholderText: "Password"
                                echoMode: TextInput.Password
                                selectByMouse: true
                            }

                            Label {
                                text: controller.registrationStatus
                                Material.foreground: Material.Red
                                wrapMode: Text.WordWrap
                                onTextChanged: {
                                    if (text == "Successful")
                                        dialog.close();
                                }
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }

                    footer: Rectangle {
                        height: 50
                        color: Material.listHighlightColor

                        RowLayout {
                            anchors{
                                fill: parent
                                leftMargin: 14
                                rightMargin: 14
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Button {
                                text: "Создать"
                                onClicked: {
                                    if (emailFolder.acceptableInput)
                                        controller.signUp(emailFolder.text, passFolder.text, nameFolder.text)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
