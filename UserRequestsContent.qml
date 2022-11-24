import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.15

Page {
    id: root

    signal backButtonClicked
    signal requestClicked

    property var controller

    header: Rectangle {
        height: 56
        color: Material.listHighlightColor

        Label {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "Мои заявки"
            font.pixelSize: 20
        }

        RowLayout {
            anchors {
                fill: parent
                leftMargin: 28
                rightMargin: 28
            }

            Button {
                icon {
                    source: "icons/icon-back.svg"
                    width: 24
                    height: 24
                }
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: 24
                padding: 0
                background: Item {}
                Material.foreground: pressed ? Material.iconColor : hovered ? Material.secondaryTextColor : Material.iconDisabledColor

                onClicked: root.backButtonClicked()
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }

    contentItem: Rectangle {
        color: Material.background

        RowLayout {
            anchors.fill: parent
            anchors.margins: 28
            spacing: 28

            ColumnLayout {
                Layout.fillWidth: true

                Button {
                    icon {
                        source: "icons/icon-add.svg"
                        height: 72
                        width:72
                    }
                    padding: 0
                    Layout.alignment: Qt.AlignHCenter
                    background: Item {}
                    Material.foreground: pressed ? Material.iconColor : hovered ? Material.secondaryTextColor : Material.iconDisabledColor

                    onClicked: dialog.open()
                }

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Заявки не найдены"
                    font {
                        italic: true
                        pixelSize: 16
                    }
                    Material.foreground: Material.secondaryTextColor
                    visible: control.count == 0
                }

                ListView {
                    id: control
                    clip: true
                    flickableDirection: Flickable.AutoFlickIfNeeded
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    spacing: 8
                    model: root.controller.userRequests

                    onMovementEnded: {
                        if (contentHeight - contentY < height*2)
                            root.controller.loadUserRequests(true)
                    }

                    delegate: Rectangle {
                        height: 100
                        width: control.width - 6
                        color: area.pressed ? Material.highlightedButtonColor :
                                              area.containsMouse ? Material.backgroundDimColor :
                                                                   Material.listHighlightColor
                        border {
                            width: 2
                            color: status === "open" ? color :
                                                       status === "closed" ? Material.backgroundDimColor :
                                                                             Material.color(Material.Orange)
                        }

                        radius: 4

                        MouseArea {
                            id: area
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                root.controller.getRequestDeals(id);
                                root.requestClicked();
                            }
                        }

                        Button {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.margins: 0
                            text: "Отменить"
                            font.pixelSize: 12
                            background: Item {}
                            Material.foreground: pressed ? Material.Red : hovered ? Material.Orange : Material.secondaryTextColor
                            visible: status === "open"

                            onClicked: root.controller.canselRequest(id)
                        }

                        RowLayout {
                            anchors{
                                fill: parent
                                margins: 14
                                leftMargin: 42
                                rightMargin: 42
                            }

                            Label {
                                text: cost
                                font.pixelSize: 40
                            }

                            Image {
                                Layout.preferredHeight: 30
                                Layout.preferredWidth: 30
                                source: "icons/icon-rub-usd.svg"

                                ColorOverlay {
                                    anchors.fill: parent
                                    source: parent
                                    opacity: 0.75
                                    color: Material.backgroundDimColor
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            ColumnLayout {
                                spacing: 14

                                RowLayout {
                                    Layout.alignment: Qt.AlignRight

                                    Label {
                                        Layout.alignment: Qt.AlignBottom
                                        text: "Осталось: "
                                        font.pixelSize: 16
                                        Material.foreground: Material.secondaryTextColor
                                    }

                                    Label {
                                        Layout.alignment: Qt.AlignBottom
                                        text: current_value
                                        font.pixelSize: 30
                                    }

                                    Label {
                                        Layout.alignment: Qt.AlignBottom
                                        text: "/" + value
                                        font.pixelSize: 16
                                        Material.foreground: Material.secondaryTextColor
                                    }

                                    Image {
                                        Layout.preferredHeight: 14
                                        Layout.preferredWidth: 14
                                        Layout.bottomMargin: 2
                                        Layout.leftMargin: -6
                                        Layout.rightMargin: -4
                                        Layout.alignment: Qt.AlignBottom
                                        source: "icons/icon-usd.svg"

                                        ColorOverlay {
                                            anchors.fill: parent
                                            source: parent
                                            opacity: 0.75
                                            color: Material.secondaryTextColor
                                        }
                                    }
                                }

                                Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: "Открыта: " + date
                                    font {
                                        pixelSize: 14
                                        italic: true
                                        weight: Font.Light
                                    }
                                    Material.foreground: Material.secondaryTextColor
                                }
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar {
                        active: true
                        width: 6

                        contentItem: Rectangle {
                            radius: width / 2
                            color: parent.pressed ? Material.scrollBarPressedColor : parent.hovered ? Material.scrollBarHoveredColor : Material.scrollBarColor
                        }
                    }
                }
            }


            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                spacing: 14

                Rectangle {
                    Layout.preferredWidth: 300
                    Layout.preferredHeight: 100
                    color: Material.backgroundColor
                    radius: 4

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 8

                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredHeight: 25
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 16
                            color: Material.secondaryTextColor
                            text: "Заявки на"
                        }

                        Rectangle {
                            Layout.preferredWidth: parent.width
                            Layout.preferredHeight: 2
                            Layout.alignment: Qt.AlignHCenter
                            color: Material.dividerColor
                        }

                        RowLayout {
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignLeft
                            spacing: 28

                            RadioButton {
                                id: buyButton
                                checked: root.controller.userFilters.isBuy
                                Layout.preferredHeight: 42
                                Material.foreground: Material.secondaryTextColor
                                text: "Покупку"

                                onCheckedChanged: {
                                    root.controller.userFilters.isBuy = checked
                                    root.controller.loadUserRequests()
                                }
                            }

                            RadioButton {
                                Layout.preferredHeight: 42
                                checked: !root.controller.userFilters.isBuy
                                Material.foreground: Material.secondaryTextColor
                                text: "Продажу"
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.preferredWidth: 300
                    Layout.preferredHeight: 145
                    color: Material.backgroundColor
                    radius: 4

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 8

                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredHeight: 25
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 16
                            color: Material.secondaryTextColor
                            text: "Сортировка"
                        }

                        Rectangle {
                            Layout.preferredWidth: parent.width
                            Layout.preferredHeight: 2
                            Layout.alignment: Qt.AlignHCenter
                            color: Material.dividerColor
                        }

                        RowLayout {
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignLeft
                            spacing: 14

                            ColumnLayout {
                                Layout.fillWidth: true

                                RadioButton {
                                    id: dateButton
                                    checked: root.controller.userFilters.isOrderByDate
                                    Layout.preferredHeight: 42
                                    Material.foreground: Material.secondaryTextColor
                                    text: "По дате"

                                    onCheckedChanged: {
                                        root.controller.userFilters.isOrderByDate = checked
                                        root.controller.loadUserRequests()
                                    }
                                }

                                RadioButton {
                                    Layout.preferredHeight: 42
                                    checked: !root.controller.userFilters.isOrderByDate
                                    Material.foreground: Material.secondaryTextColor
                                    text: "По цене"
                                }
                            }

                            Rectangle {
                                Layout.preferredHeight: parent.height
                                Layout.preferredWidth: 2
                                color: Material.dividerColor
                            }

                            ColumnLayout {
                                Layout.fillWidth: true

                                RadioButton {
                                    Layout.preferredHeight: 42
                                    checked: !root.controller.userFilters.isReverseSort
                                    Material.foreground: Material.secondaryTextColor
                                    text: "По убыванию"
                                }

                                RadioButton {
                                    id: reverseButton
                                    Layout.preferredHeight: 42
                                    checked: root.controller.userFilters.isReverseSort
                                    Material.foreground: Material.secondaryTextColor
                                    text: "По возрастанию"

                                    onCheckedChanged: {
                                        root.controller.userFilters.isReverseSort = checked
                                        root.controller.loadUserRequests()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: dialog
        modal: true
        x: (root.width - width)/2
        y: (root.height - height)/2
        width: 300

        onClosed: root.controller.creationRequestStatus = ""

        onApplied: root.controller.createRequest(isBuy.checked ? "buy" : "sale", parseInt(value.text), parseInt(cost.text))

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
                    text: "Новая заявка"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }

        contentItem: Rectangle {
            color: Material.dialogColor
            implicitHeight: 150

            ColumnLayout {
                anchors {
                    fill: parent
                }
                spacing: 8

                Material.foreground: Material.secondaryTextColor

                RowLayout {
                    RadioButton {
                        id: isBuy
                        text: "Купить"
                        checked: true
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter
                    }
                    RadioButton {
                        text: "Продать"
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter
                    }
                }

                RowLayout {
                    TextField {
                        id: cost
                        placeholderText: "Цена"
                        Layout.fillWidth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        selectByMouse: true
                        validator: IntValidator {
                            bottom: 0
                        }
                    }

                    TextField {
                        id: value
                        placeholderText: "Количество"
                        Layout.fillWidth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        selectByMouse: true
                        validator: IntValidator {
                            bottom: 1
                        }
                    }
                }

                Label {
                    text: root.controller.creationRequestStatus
                    Material.foreground: Material.Red
                    onTextChanged: {
                        if (text == "Успешно")
                            dialog.close();
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }

        footer: DialogButtonBox {

            Button {
                text: "Отмена"
                flat: true
                DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
            }

            Button {
                text: "Создать"
                flat: true
                DialogButtonBox.buttonRole: DialogButtonBox.ApplyRole
            }
        }
    }
}
