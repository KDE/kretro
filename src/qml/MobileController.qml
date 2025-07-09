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
                App.getRetroPad().updateInputStates(Qt.Key_Up, pressed)
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
                App.getRetroPad().updateInputStates(Qt.Key_Down, pressed)
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
                App.getRetroPad().updateInputStates(Qt.Key_Left, pressed)
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
                App.getRetroPad().updateInputStates(Qt.Key_Right, pressed)
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
                        App.getRetroPad().updateInputStates(Qt.Key_A, pressed)
                        haptics.buttonVibrate()
                    }
                }
                Controls.Button {
                    text: "SELECT"
                    Layout.fillWidth: true
                    onPressedChanged: {
                        App.getRetroPad().updateInputStates(Qt.Key_S, pressed)
                        haptics.buttonVibrate()
                    }
                }
            }
            RowLayout {
                Controls.Button {
                    text: "L1"
                    Layout.fillWidth: true
                    onPressedChanged: {
                        App.getRetroPad().updateInputStates(Qt.Key_Q, pressed)
                        haptics.buttonVibrate()
                    }
                }
                Controls.Button {
                    text: "R1"
                    Layout.fillWidth: true
                    onPressedChanged: {
                        App.getRetroPad().updateInputStates(Qt.Key_W, pressed)
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
                    App.getRetroPad().updateInputStates(Qt.Key_Z, pressed)
                    haptics.buttonVibrate()
                }
            }
            Controls.Button {
                text: "B"
                Layout.fillWidth: true
                implicitHeight: 50
                onPressedChanged: {
                    App.getRetroPad().updateInputStates(Qt.Key_X, pressed)
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
                    App.getRetroPad().updateInputStates(Qt.Key_C, pressed)
                    haptics.buttonVibrate()
                }
            }
            Controls.Button {
                text: "Y"
                Layout.fillWidth: true
                implicitHeight: 35
                onPressedChanged: {
                    App.getRetroPad().updateInputStates(Qt.Key_V, pressed)
                    haptics.buttonVibrate()
                }
            }
        }
    }
}