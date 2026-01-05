// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import org.kde.kirigami as Kirigami
import org.kde.kretro

Kirigami.Page {
    id: page

    property bool isMobileControllerVisible: Config.overrideMobileControllerVisibility ? !Kirigami.Settings.isMobile : Kirigami.Settings.isMobile
    property bool isFullscreen: false

    onIsFullscreenChanged: {
        var appWindow = applicationWindow();
        if (isFullscreen) {
            appWindow.visibility = Window.FullScreen;
            page.titleDelegate = null;
            showPassiveNotification(i18n("Press Escape to exit fullscreen mode"));
        } else {
            appWindow.visibility = Window.Windowed;
            page.titleDelegate = item;
        }
    }

    Component { 
        id: item 
        Item { }
    }

    actions: [
        Kirigami.Action {
            text: i18n("Fullscreen")
            visible: !page.isMobileControllerVisible
            icon.name: "view-fullscreen"
            onTriggered: {
                page.isFullscreen = !page.isFullscreen;
            }
        },
        Kirigami.Action {
            text: i18n("Reset Core")
            icon.name: "view-refresh"
            onTriggered: resetPrompt.open()
        },
        Kirigami.Action {
            text: i18n("Core Settings")
            icon.name: "settings-configure"
            onTriggered: {
                pageStack.push(Qt.resolvedUrl('./SettingsRetroCore.qml'))
            }
        },
        Kirigami.Action {
            text: i18n("Save Slots")
            icon.name: "document-save-all"
            onTriggered: {
                pageStack.push(Qt.resolvedUrl('./SaveManager.qml'))
            }
        }
    ]

    topPadding: 0
    bottomPadding: 0
    leftPadding: 0
    rightPadding: 0

    title: i18n("KRetro Player")

    focusPolicy: Qt.StrongFocus

    Component.onCompleted:  {
        if (Config.defaultFullscreen) {
            page.isFullscreen = true;
        }
        App.startRetroCore()
    }
    Component.onDestruction: {
        App.stopRetroCore()
        App.error = ""

        var appWindow = applicationWindow();
        appWindow.visibility = Window.Windowed;
        page.titleDelegate = item;        
    }

    function handleKeyPress(event, pressed) {
        if (event.isAutoRepeat) {
            return;
        }
        if (event.key === Qt.Key_Escape && pressed) {
            if(page.isFullscreen) {
                page.isFullscreen = false;
            } else {
                pageStack.pop();
            }
            return;
        }
        App.getRetroPad().updateInputStates(event.key, pressed);
    }
    Keys.onPressed: handleKeyPress(event, true)
    Keys.onReleased: handleKeyPress(event, false)

    IdleInhibit {
        id: idleInhibit
    }

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
                smooth: true

                // when clicking on the game, force focus to the page
                TapHandler {
                    onTapped: {
                        page.forceActiveFocus();
                    }
                }
            }
        }

        Item {
            Layout.preferredHeight: Kirigami.Units.gridUnit * 2
            Layout.fillWidth: true 
            visible: page.isMobileControllerVisible
        }
        MobileController {
            Layout.fillWidth: true
            Layout.leftMargin: Kirigami.Units.gridUnit
            Layout.rightMargin: Kirigami.Units.gridUnit
            visible: page.isMobileControllerVisible
        }
        Item { 
            Layout.preferredHeight: Kirigami.Units.gridUnit 
            visible: page.isMobileControllerVisible
        }
    }
}
