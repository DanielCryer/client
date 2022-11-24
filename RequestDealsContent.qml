import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.15

Page {
    id: root

    signal backButtonClicked

    property var controller

    header: Rectangle {
        height: 56
        color: Material.listHighlightColor

        Label {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "Сделки"
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
                Layout.fillHeight: true
                Layout.fillWidth: true

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Сделки не найдены"
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
                    flickableDirection: Flickable.VerticalFlick
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    spacing: 8
                    model: root.controller.deals

                    delegate: Rectangle {
                        height: 100
                        width: control.width - 6
                        color: Material.listHighlightColor
                        radius: 4

                        RowLayout {
                            anchors{
                                fill: parent
                                margins: 14
                                leftMargin: 42
                                rightMargin: 42
                            }

                            ColumnLayout {
                                spacing: 14

                                Label {
                                    text: "Заключена с " + name
                                    Material.foreground: Material.secondaryTextColor
                                }

                                RowLayout {
                                    Layout.alignment: Qt.AlignLeft

                                    Label {
                                        text: cost
                                        font.pixelSize: 35
                                    }

                                    Image {
                                        Layout.preferredHeight: 24
                                        Layout.preferredWidth: 24
                                        source: "icons/icon-rub-usd.svg"

                                        ColorOverlay {
                                            anchors.fill: parent
                                            source: parent
                                            opacity: 0.75
                                            color: Material.backgroundDimColor
                                        }
                                    }
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
                                        text: "На сумму:"
                                        font.pixelSize: 16
                                        Material.foreground: Material.secondaryTextColor
                                    }

                                    Label {
                                        text: value
                                        font.pixelSize: 24
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
                                    text: date
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
                            color: parent.pressed ? Material.scrollBarPressedColor :
                                                    parent.hovered ? Material.scrollBarHoveredColor :
                                                                     Material.scrollBarColor
                        }
                    }
                }
            }


            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                Layout.maximumWidth: 300
                spacing: 14
                Material.foreground: Material.secondaryTextColor

                Label {
                    text: "Заявка"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: "Открыта: " + root.controller.dealsRequest.date
                    font {
                        pixelSize: 14
                        italic: true
                        weight: Font.Light
                    }
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    Label {
                        text: root.controller.dealsRequest.direction === "buy" ? "На покупку" : "На продажу"
                        font.pixelSize: 16
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Label {
                        text: "Статус: " + (root.controller.dealsRequest.status === "open" ? "открыта" :
                                                root.controller.dealsRequest.status === "closed" ? "закрыта" :
                                                                                                   "отменена")
                        font.pixelSize: 16
                    }
                }

                RowLayout {

                    Label {
                        text: root.controller.dealsRequest.cost
                        font.pixelSize: 24
                    }

                    Image {
                        Layout.preferredHeight: 16
                        Layout.preferredWidth: 16
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

                    Label {
                        Layout.alignment: Qt.AlignBottom
                        text: "Осталось: "
                        font.pixelSize: 16
                        Material.foreground: Material.secondaryTextColor
                    }

                    Label {
                        Layout.alignment: Qt.AlignBottom
                        text: root.controller.dealsRequest.current_value
                        font.pixelSize: 24
                    }

                    Label {
                        Layout.alignment: Qt.AlignBottom
                        text: "/" + root.controller.dealsRequest.value
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
            }
        }
    }
}
