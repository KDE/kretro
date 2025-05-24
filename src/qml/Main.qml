// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan@sineware.ca>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.config as Config
import org.kde.kretro

Kirigami.ApplicationWindow {
    id: root
    pageStack.globalToolBar.style: Kirigami.ApplicationHeaderStyle.ToolBar;
    pageStack.globalToolBar.showNavigationButtons: Kirigami.ApplicationHeaderStyle.ShowBackButton;

    title: i18n("kretro")

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    function startGame(rom) {
        App.setRomFilePath(rom.path);
        App.setRomConsole(rom.console);
        pageStack.layers.push(Qt.resolvedUrl('./MobilePlayer.qml'));
    }

    Config.WindowStateSaver {
        configGroupName: "Main"
    }

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("kretro")
        titleIcon: "applications-graphics"
        isMenu: !root.isMobile
        actions: [
            Kirigami.Action {
                text: i18n("Open ROMs Directory")
                icon.name: "folder-add"
                onTriggered: {
                    var dir = App.gamesDir();
                    var fileUrl = "file://" + dir;
                    Qt.openUrlExternally(fileUrl);
                }
            },
            Kirigami.Action {
                text: i18n("Settings")
                icon.name: "settings-configure"
                onTriggered: {
                    pageStack.layers.push(Qt.resolvedUrl('./Settings.qml'));
                }
            },
            Kirigami.Action {
                text: i18n("About kretro")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push(Qt.createComponent("org.kde.kirigamiaddons.formcard", "AboutPage"))
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

    onActiveFocusItemChanged: {
        // hack: force focus to gridview upon return to main page
        if (activeFocusItem.toString().includes("QQuickRowLayout")) {
            console.log("Forcing focus to gridview");
            gridViewFocusScope.forceActiveFocus();
        }
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

        Kirigami.Heading {
            id: heading
            anchors.top: errorMessage.bottom
            text: i18n("Game Library")
        }
        FocusScope {
            id: gridViewFocusScope
            anchors.top: heading.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: Kirigami.Units.gridUnit

            focus: true
            activeFocusOnTab: true
            focusPolicy: Qt.StrongFocus
            
            GridView {
                id: gamesGridView
                anchors.fill: parent
                
                cellWidth: Kirigami.Units.gridUnit * 9
                cellHeight: Kirigami.Units.gridUnit * 9
                focus: true
                
                highlight: Rectangle {
                    color: Kirigami.Theme.highlightColor
                    opacity: 0.3
                    radius: Kirigami.Units.cornerRadius
                    z: 2
                }
                highlightMoveDuration: Kirigami.Units.longDuration
                
                keyNavigationEnabled: true
                Keys.onPressed: function(event) {
                    if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                        var retroGame = retroGameModel.get(gamesGridView.currentIndex);
                        root.startGame(retroGame);
                    }
                }
                
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
                            gamesGridView.currentIndex = index;
                            root.startGame(model.rom);
                        }
                    }
                }
                
                model: retroGameModel
                delegate: fileDelegate
            }
        }
    }


    Component.onCompleted: {
        gridViewFocusScope.forceActiveFocus();
    }
}