// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kretro 1.0

Kirigami.Page {
    id: page

    Layout.fillWidth: true

    title: i18n("Mobile Player Layout")

    Component.onCompleted:  {
        App.startRetroCore()
    }
    Component.onDestruction: {
        App.stopRetroCore()
    }

    function handleKeyPress(event, pressed) {
        switch(event.key) {
            case Qt.Key_Z:
                App.setButtonState("A", pressed)
                break;
            case Qt.Key_X:
                App.setButtonState("B", pressed)
                break;
            case Qt.Key_Up:
                App.setButtonState("UP", pressed)
                break;
            case Qt.Key_Down:
                App.setButtonState("DOWN", pressed)
                break;
            case Qt.Key_Left:
                App.setButtonState("LEFT", pressed)
                break;
            case Qt.Key_Right:
                App.setButtonState("RIGHT", pressed)
                break;
            case Qt.Key_A:
                App.setButtonState("START", pressed)
                break;
            case Qt.Key_S:
                App.setButtonState("SELECT", pressed)
                break;

        }
    }
    Keys.onPressed: handleKeyPress(event, true)
    Keys.onReleased: handleKeyPress(event, false)

    ColumnLayout {
        width: page.width
        height: page.height
        anchors.centerIn: parent
        Item {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: parent.height / 2
            Layout.preferredWidth: parent.width
            Rectangle {
                color: "black"
                anchors.fill: parent
            }
            RetroFrame {
                anchors.fill: parent
            }
        }
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Item {
                width: 190
                height: 110
                Controls.Button {
                    text: "↑"
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    onPressedChanged: {
                        App.setButtonState("UP", pressed)
                    }
                }
                Controls.Button {
                    text: "↓"
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    onPressedChanged: {
                        App.setButtonState("DOWN", pressed)
                    }
                }
                Controls.Button {
                    text: "←"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    onPressedChanged: {
                        App.setButtonState("LEFT", pressed)
                    }
                }
                Controls.Button {
                    text: "→"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    onPressedChanged: {
                        App.setButtonState("RIGHT", pressed)
                    }
                }
            }
            GridLayout {
                rows: 2
                columns: 1
                RowLayout {
                    Controls.Button {
                        text: "START"
                        onPressedChanged: {
                            App.setButtonState("START", pressed)
                        }
                    }
                    Controls.Button {
                        text: "SELECT"
                        onPressedChanged: {
                            App.setButtonState("SELECT", pressed)
                        }
                    }
                }
                RowLayout {
                    Controls.Button {
                        text: "A"
                        onPressedChanged: {
                            App.setButtonState("A", pressed)
                        }
                    }
                    Controls.Button {
                        text: "B"
                        onPressedChanged: {
                            App.setButtonState("B", pressed)
                        }
                    }
                }
            }


        }

    }
}
