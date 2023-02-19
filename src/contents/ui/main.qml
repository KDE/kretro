// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kretro 1.0

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

    property int counter: 0

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("kretro")
        titleIcon: "applications-graphics"
        isMenu: !root.isMobile
        actions: [
            Kirigami.Action {
                text: i18n("Plus One")
                icon.name: "list-add"
                onTriggered: {
                    counter += 1
                }
            },
            Kirigami.Action {
                text: i18n("About kretro")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:About.qml')
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

        title: i18n("Main Page")

        actions.main: Kirigami.Action {
            text: i18n("Plus One")
            icon.name: "list-add"
            tooltip: i18n("Add one to the counter")
            onTriggered: {
                counter += 1
            }
        }

        ColumnLayout {
            width: page.width

            anchors.centerIn: parent
            RetroFrame {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 464
                Layout.preferredWidth: 376
            }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
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
                Controls.Button {
                    text: "UP"
                    onPressedChanged: {
                        App.setButtonState("UP", pressed)
                    }
                }
                Controls.Button {
                    text: "DOWN"
                    onPressedChanged: {
                        App.setButtonState("DOWN", pressed)
                    }
                }
                Controls.Button {
                    text: "LEFT"
                    onPressedChanged: {
                        App.setButtonState("LEFT", pressed)
                    }
                }
                Controls.Button {
                    text: "RIGHT"
                    onPressedChanged: {
                        App.setButtonState("RIGHT", pressed)
                    }
                }
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

        }
    }
}
