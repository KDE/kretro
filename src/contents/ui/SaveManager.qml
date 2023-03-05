// SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>
// SPDX-FileCopyrightText: 2023 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kretro 1.0

Kirigami.ScrollablePage {
    id: page

    title: i18n("Core Save States")

    ListView {
        header: RowLayout {
            id: text

            QQC2.Label {
                text: i18n("Select a save state to load or delete.\nSlot 0 is the default auto save slot\n(restored on game start, saved on exit)")
            }

            Item {
                Layout.fillWidth: true
            }

            QQC2.Button {
                text: i18n("Save Game\n(New Slot)")
                onClicked: {
                    const path = App.saveNewSaveSlot()
                    if (path.length > 0) {
                        console.log(path);
                        gameSaveModel.append(path);
                    }

                    pageStack.layers.pop()
                }
            }
        }

        model: RetroGameSaveModel {
            id: gameSaveModel
        }

        delegate: QQC2.ItemDelegate {
            id: saveDelegate

            required property int index
            required property string path
            required property string slot

            contentItem: RowLayout {
                spacing: Kirigami.Units.smallSpacing

                QQC2.Label {
                    text: i18n("Slot %1", saveDelegate.slot)
                }

                Item {
                    Layout.fillWidth: true
                }

                QQC2.Button {
                    text: i18nc("@action:button", "Load")
                    onClicked: {
                        App.loadSaveSlot(saveDelegate.path)
                        pageStack.layers.pop()
                    }
                }

                QQC2.Button {
                    text: i18nc("@action:button", "Save")
                    onClicked: {
                        App.saveSaveSlot(saveDelegate.path)
                        pageStack.layers.pop()
                    }
                }

                QQC2.Button {
                    text: i18nc("@action:button", "Delete")
                    onClicked: {
                        gameSaveModel.removeSaveSlot(saveDelegate.index)
                        pageStack.layers.pop()
                    }
                }
            }
        }
    }
}