// SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>
// SPDX-FileCopyrightText: 2023 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kretro

Kirigami.ScrollablePage {
    id: page
    title: i18n("Save States")
    actions: Kirigami.Action {
        icon.name: "list-add"
        text: i18nc("@action:button", "New Save State")
        onTriggered: {
            const path = App.saveNewSaveSlot()
            if (path.length > 0) {
                console.log(path);
                gameSaveModel.append(path);
                showPassiveNotification(i18n("Game saved!"));
            }
        }
    }
    Kirigami.CardsListView {
        id: saveStatesListView
        header: ColumnLayout {
            width: parent.width
            spacing: Kirigami.Units.largeSpacing
            Item {
                Layout.fillWidth: true
                height: Kirigami.Units.largeSpacing
            }
            Kirigami.InlineMessage {
                Layout.fillWidth: true
                visible: true
                text: i18n("Slot 0 is the default auto save slot. It is restored on game start and saved on exit.")
            }
            Item {
                Layout.fillWidth: true
                height: Kirigami.Units.largeSpacing
            }
        }
        model: RetroGameSaveModel {
            id: gameSaveModel
        }
        delegate: Kirigami.AbstractCard {
            id: saveDelegate
            required property int index
            required property string path
            required property string slot
            required property var lastModified
            
            contentItem: Item {
                implicitWidth: cardLayout.implicitWidth
                implicitHeight: cardLayout.implicitHeight
                ColumnLayout {
                    id: cardLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }
                    spacing: Kirigami.Units.smallSpacing
                    
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: Kirigami.Units.largeSpacing
                        
                        Kirigami.Heading {
                            level: 3
                            text: i18n("Slot %1", saveDelegate.slot)
                        }
                        
                        Item {
                            Layout.fillWidth: true
                        }
                    }
                    
                    Controls.Label {
                        Layout.fillWidth: true
                        text: i18n("Last modified: %1", saveDelegate.lastModified.toLocaleString())
                        font.italic: true
                        opacity: 0.7
                        wrapMode: Text.WordWrap
                        maximumLineCount: 2
                        elide: Text.ElideRight
                    }
                    
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: Kirigami.Units.smallSpacing
                        spacing: Kirigami.Units.smallSpacing
                        
                        Controls.Button {
                            text: i18nc("@action:button", "Load")
                            onClicked: {
                                App.loadSaveSlot(saveDelegate.path)
                                pageStack.pop()
                            }
                        }
                        Controls.Button {
                            text: i18nc("@action:button", "Save")
                            onClicked: {
                                App.saveSaveSlot(saveDelegate.path)
                                pageStack.pop()
                            }
                        }
                        Controls.Button {
                            text: i18nc("@action:button", "Delete")
                            onClicked: {
                                gameSaveModel.removeSaveSlot(saveDelegate.index)
                            }
                        }
                    }
                }
            }
        }
    }
}