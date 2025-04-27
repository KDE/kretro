// SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>
// SPDX-FileCopyrightText: 2023 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kretro 1.0

Kirigami.ScrollablePage {
    id: page
    
    title: i18n("Save States")
    
    actions: Kirigami.Action {
        icon.name: "list-add"
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
            
            contentItem: Item {
                implicitWidth: cardLayout.implicitWidth
                implicitHeight: cardLayout.implicitHeight
                
                RowLayout {
                    id: cardLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }
                    spacing: Kirigami.Units.largeSpacing
                    
                    Kirigami.Heading {
                        level: 3
                        text: i18n("Slot %1", saveDelegate.slot)
                    }
                    
                    Item {
                        Layout.fillWidth: true
                    }
                    
                    RowLayout {
                        spacing: Kirigami.Units.smallSpacing
                        
                        Controls.Button {
                            text: i18nc("@action:button", "Load")
                            onClicked: {
                                App.loadSaveSlot(saveDelegate.path)
                                pageStack.layers.pop()
                            }
                        }
                        
                        Controls.Button {
                            text: i18nc("@action:button", "Save")
                            onClicked: {
                                App.saveSaveSlot(saveDelegate.path)
                                pageStack.layers.pop()
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