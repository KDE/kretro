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
            page.globalToolBarStyle = Kirigami.ApplicationHeaderStyle.None;
        } else {
            appWindow.visibility = Window.Windowed;
            page.globalToolBarStyle = Kirigami.ApplicationHeaderStyle.ToolBar;
        }
    }

    actions: [
        Kirigami.Action {
            icon.name: "view-fullscreen"
            onTriggered: {
                page.isFullscreen = !page.isFullscreen;
                showPassiveNotification(i18n("Press Escape to exit fullscreen mode!"));
            }
        },
        Kirigami.Action {
            icon.name: "view-refresh"
            onTriggered: resetPrompt.open()
        },
        Kirigami.Action {
            icon.name: "settings-configure"
            onTriggered: {
                pageStack.layers.push(Qt.resolvedUrl('./SettingsRetroCore.qml'))
            }
        },
        Kirigami.Action {
            icon.name: "document-save-all"
            onTriggered: {
                pageStack.layers.push(Qt.resolvedUrl('./SaveManager.qml'))
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

        var appWindow = applicationWindow();
        appWindow.visibility = Window.Windowed;
        appWindow.pageStack.globalToolBar.style = Kirigami.ApplicationHeaderStyle.Auto;
    }

    function handleKeyPress(event, pressed) {
        if (event.isAutoRepeat) {
            return;
        }
        if (event.key === Qt.Key_Escape && pressed) {
            if(page.isFullscreen) {
                page.isFullscreen = false;
            } else {
                pageStack.layers.pop();
            }
            return;
        }
        App.getRetroPad().updateInputStates(event.key, pressed);
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
                smooth: true
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
