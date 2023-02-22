// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.0
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kretro 1.0

Kirigami.Page {
    id: page

    Layout.fillWidth: true
    Layout.fillHeight: true
    topPadding: 0
    bottomPadding: 0
    leftPadding: 0
    rightPadding: 0

    title: i18n("KRetro Mobile Player")

    Component.onCompleted:  {
        App.startRetroCore()
    }
    Component.onDestruction: {
        App.stopRetroCore()
        App.error = ""
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

    Kirigami.InlineMessage {
            id: errorMessage
            type: Kirigami.MessageType.Error
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            z: 1

            visible: App.error !== ""
            text: App.error
    }

    ColumnLayout {
        anchors.fill: parent
        Item {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width
            Rectangle {
                id: rect
                anchors.fill: parent
                color: "black"
                border.color: "white"
                border.width: 1
            }
            RetroFrame {
                id: frame
                anchors.fill: parent
            }
        }
        Item {
            Layout.preferredHeight: Kirigami.Units.gridUnit * 2
            Layout.fillWidth: true
            Text {
                id: text
                text: i18n("||||||| KRetro Player |||||||")
                anchors.centerIn: parent
                font.pixelSize: Kirigami.Units.gridUnit
                visible: false
            }
            LinearGradient {
                id: mask
                anchors.fill: text
                start: Qt.point(0, 0)
                end: Qt.point(text.width, 0)
                gradient: Gradient {
                    GradientStop { position: 0.01; color: "transparent" }
                    GradientStop { position: 0.5; color: "white" }
                    GradientStop { position: 0.99; color: "transparent" }
                }
                visible: false
            }
            OpacityMask {
                anchors.fill: text
                source: text
                maskSource: mask
            }
        }
        MobileController {
            Layout.fillWidth: true
            Layout.bottomMargin: Kirigami.Units.gridUnit * 2
            Layout.leftMargin: Kirigami.Units.gridUnit
            Layout.rightMargin: Kirigami.Units.gridUnit
        }
    }
}
