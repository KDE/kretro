// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kretro 1.0

Kirigami.ApplicationWindow {
    id: root
    pageStack.globalToolBar.style: Kirigami.ApplicationHeaderStyle.ToolBar;
    pageStack.globalToolBar.showNavigationButtons: Kirigami.ApplicationHeaderStyle.ShowBackButton;

    title: i18n("kretro")

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    onClosing: App.saveWindowGeometry(root)

    onWidthChanged: saveWindowGeometryTimer.restart()
    onHeightChanged: saveWindowGeometryTimer.restart()
    onXChanged: saveWindowGeometryTimer.restart()
    onYChanged: saveWindowGeometryTimer.restart()

    Component.onCompleted: App.restoreWindowGeometry(root)

    // This timer allows to batch update the window size change to reduce
    // the io load and also work around the fact that x/y/width/height are
    // changed when loading the page and overwrite the saved geometry from
    // the previous session.
    Timer {
        id: saveWindowGeometryTimer
        interval: 1000
        onTriggered: App.saveWindowGeometry(root)
    }
    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("kretro")
        titleIcon: "applications-graphics"
        isMenu: !root.isMobile
        actions: [
            Kirigami.Action {
                text: i18n("About kretro")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:/About.qml')
            },
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "application-exit"
                onTriggered: Qt.quit()
            }
        ]
    }


    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    pageStack.initialPage: page

    Kirigami.Page {
        id: page
        Layout.fillWidth: true

        title: i18n("KRetro")

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

        // Heading
        Kirigami.Heading {
            id: heading
            anchors.top: errorMessage.bottom
            text: i18n("Game Library")
        }

        GridView {
            anchors.top: heading.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: Kirigami.Units.gridUnit

            cellWidth: Kirigami.Units.gridUnit * 9
            cellHeight: Kirigami.Units.gridUnit * 9

            RetroGameModel {
                id: retroGameModel
            }
            Component {
                id: fileDelegate
                Kirigami.Card {
                    banner.source: model.rom.icon
                    banner.title: model.rom.name
                    implicitHeight: Kirigami.Units.gridUnit * 8
                    implicitWidth: Kirigami.Units.gridUnit * 8
                    onPressed: {
                        App.setRomFilePath(model.rom.path)
                        App.setRomConsole(model.rom.console)
                        pageStack.layers.push('qrc:/MobilePlayer.qml')
                    }
                }
            }

            model: retroGameModel
            delegate: fileDelegate
        }
    }
}
