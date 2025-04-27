// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan@sineware.ca>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import org.kde.kirigami as Kirigami
import org.kde.kretro

Kirigami.Page {
    id: page

    actions: [
        Kirigami.Action {
            icon.name: "view-refresh"
            onTriggered: resetPrompt.open()
        },
        Kirigami.Action {
            icon.name: "document-save-all"
            onTriggered: {
                pageStack.layers.push('qrc:/SaveManager.qml')
            }
        }
    ]

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
            case Qt.Key_C:
                App.setButtonState("X", pressed)
                break;
            case Qt.Key_V:
                App.setButtonState("Y", pressed)
                break;
            case Qt.Key_Q:
                App.setButtonState("L1", pressed)
                break;
            case Qt.Key_W:
                App.setButtonState("R1", pressed)
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

    Kirigami.PromptDialog {
        id: resetPrompt
        title: i18n("Reset Game Core?")
        subtitle: i18n("This will restart the game. Any unsaved progress will be lost!")
        standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel

        onAccepted: App.resetRetroCore()
    }

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
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width
            Rectangle {
                id: rect
                anchors.fill: parent
                color: "black"
            }
            RetroFrame {
                id: frame
                anchors.fill: parent
            }
        }
        Item { Layout.preferredHeight: Kirigami.Units.gridUnit * 2
        Layout.fillWidth: true }
        MobileController {
            Layout.fillWidth: true
            Layout.leftMargin: Kirigami.Units.gridUnit
            Layout.rightMargin: Kirigami.Units.gridUnit
        }
        Item { Layout.preferredHeight: Kirigami.Units.gridUnit }
    }
}
