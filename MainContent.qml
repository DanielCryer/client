import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.15

Page {
    id: root

    signal userRequestsButtonClicked

    property var controller



    header: Rectangle {
        height: 56
        color: Material.listHighlightColor

        Label {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "Главная"
            font.pixelSize: 20
        }

        RowLayout {
            anchors {
                fill: parent
                leftMargin: 28
                rightMargin: 28
            }

            Item {
                Layout.fillWidth: true
            }

            ProfileButton {
                Layout.fillHeight: true
                controller: root.controller
                rootItem: root

                onUserRequestsButtonClicked: root.userRequestsButtonClicked()
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
                    model: root.controller.generalRequests

                    onFlickEnded: {
                        if (contentHeight - contentY < height*2)
                            root.controller.loadGeneralRequests(true)
                    }

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
                                checked: root.controller.generalFilters.isBuy
                                Layout.preferredHeight: 42
                                Material.foreground: Material.secondaryTextColor
                                text: "Покупку"

                                onCheckedChanged: {
                                    root.controller.generalFilters.isBuy = checked
                                    root.controller.loadGeneralRequests()
                                }
                            }

                            RadioButton {
                                Layout.preferredHeight: 42
                                checked: !root.controller.generalFilters.isBuy
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
                                    checked: root.controller.generalFilters.isOrderByDate
                                    Layout.preferredHeight: 42
                                    Material.foreground: Material.secondaryTextColor
                                    text: "По дате"

                                    onCheckedChanged: {
                                        root.controller.generalFilters.isOrderByDate = checked
                                        root.controller.loadGeneralRequests()
                                    }
                                }

                                RadioButton {
                                    Layout.preferredHeight: 42
                                    checked: !root.controller.generalFilters.isOrderByDate
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
                                    checked: !root.controller.generalFilters.isReverseSort
                                    Material.foreground: Material.secondaryTextColor
                                    text: "По убыванию"
                                }

                                RadioButton {
                                    id: reverseButton
                                    Layout.preferredHeight: 42
                                    checked: root.controller.generalFilters.isReverseSort
                                    Material.foreground: Material.secondaryTextColor
                                    text: "По возрастанию"

                                    onCheckedChanged: {
                                        root.controller.generalFilters.isReverseSort = checked
                                        root.controller.loadGeneralRequests()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
