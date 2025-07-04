/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>
*/

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kretro

RowLayout {
    HapticsEffectLoader {
        id: haptics
    }
    Item {
        height: 150
        width: 150
        Controls.Button {
            text: "↑"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 50
            height: 50
            onPressedChanged: {
                App.setButtonState("UP", pressed)
                haptics.buttonVibrate()
            }
        }
        Controls.Button {
            text: "↓"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 50
            height: 50
            onPressedChanged: {
                App.setButtonState("DOWN", pressed)
                haptics.buttonVibrate()
            }
        }
        Controls.Button {
            text: "←"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: 50
            height: 50
            onPressedChanged: {
                App.setButtonState("LEFT", pressed)
                haptics.buttonVibrate()
            }
        }
        Controls.Button {
            text: "→"
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            width: 50
            height: 50
            onPressedChanged: {
                App.setButtonState("RIGHT", pressed)
                haptics.buttonVibrate()
            }
        }
    }
    Item {
        width: Kirigami.Units.gridUnit * 2
    }
    GridLayout {
        rows: 3
        columns: 1
        ColumnLayout {
            RowLayout {
                Controls.Button {
                    text: "START"
                    Layout.fillWidth: true
                    onPressedChanged: {
                        App.setButtonState("START", pressed)
                        haptics.buttonVibrate()
                    }
                }
                Controls.Button {
                    text: "SELECT"
                    Layout.fillWidth: true
                    onPressedChanged: {
                        App.setButtonState("SELECT", pressed)
                        haptics.buttonVibrate()
                    }
                }
            }
            RowLayout {
                Controls.Button {
                    text: "L1"
                    Layout.fillWidth: true
                    onPressedChanged: {
                        App.setButtonState("L1", pressed)
                        haptics.buttonVibrate()
                    }
                }
                Controls.Button {
                    text: "R1"
                    Layout.fillWidth: true
                    onPressedChanged: {
                        App.setButtonState("R1", pressed)
                        haptics.buttonVibrate()
                    }
            }
            }
        }
        RowLayout {
            Controls.Button {
                text: "A"
                Layout.fillWidth: true
                implicitHeight: 50
                onPressedChanged: {
                    App.setButtonState("A", pressed)
                    haptics.buttonVibrate()
                }
            }
            Controls.Button {
                text: "B"
                Layout.fillWidth: true
                implicitHeight: 50
                onPressedChanged: {
                    App.setButtonState("B", pressed)
                    haptics.buttonVibrate()
                }
            }
        }
        RowLayout {
            Controls.Button {
                text: "X"
                Layout.fillWidth: true
                implicitHeight: 35
                onPressedChanged: {
                    App.setButtonState("X", pressed)
                    haptics.buttonVibrate()
                }
            }
            Controls.Button {
                text: "Y"
                Layout.fillWidth: true
                implicitHeight: 35
                onPressedChanged: {
                    App.setButtonState("Y", pressed)
                    haptics.buttonVibrate()
                }
            }
        }
    }
}