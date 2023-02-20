// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kretro 1.0
import Qt.labs.folderlistmodel 2.15

Kirigami.ApplicationWindow {
    id: root

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
                onTriggered: pageStack.layers.push('qrc:About.qml')
            },
            Kirigami.Action {
                text: i18n("[Debug] Mobile Player UI")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:MobilePlayer.qml')
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

        ListView {
            width: 200; height: 400

            FolderListModel {
                id: folderModel
                folder: "file://" + App.getEnv("HOME") + "/Documents/Games"
                nameFilters: ["*.gba"]
            }

            Component {
                id: fileDelegate
                Kirigami.Card {
                    // Banner
                    banner.source: "https://upload.wikimedia.org/wikipedia/en/thumb/9/9f/Super_Mario_Advance_4_cover.jpg/220px-Super_Mario_Advance_4_cover.jpg"
                    banner.title: model.fileName
                    onPressed: {
                        App.setRomFilePath(model.filePath)
                        pageStack.layers.push('qrc:MobilePlayer.qml')
                    }
                }

            }

            model: folderModel
            delegate: fileDelegate
        }
    }
}
