// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2025 Seshan Ravikumar <seshan@sineware.ca>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.folderlistmodel
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.kretro

Kirigami.ScrollablePage {
    id: page
    actions: []
    
    title: i18n("Settings")
    
    FolderListModel {
        id: libretroModel
        folder: "file://" + Config.libretroCoresDirectory
        nameFilters: ["*.so", "*.dll", "*.dylib"]
        showDirs: false
        sortField: FolderListModel.Name
    }
    
    FolderDialog {
        id: romFolderDialog
        title: i18n("Select ROMs Directory")
        currentFolder: "file://" + Config.romsDirectory
        onAccepted: {
            Config.romsDirectory = selectedFolder.toString().replace("file://", "")
        }
    }
    
    FolderDialog {
        id: libretroCoreFolderDialog
        title: i18n("Select Libretro Cores Directory")
        currentFolder: "file://" + Config.libretroCoresDirectory
        onAccepted: {
            Config.libretroCoresDirectory = selectedFolder.toString().replace("file://", "")
        }
    }
    
    ColumnLayout {
        anchors.fill: parent
        spacing: Kirigami.Units.largeSpacing
        
        FormCard.FormHeader {
            title: i18n("General")
        }
        
        FormCard.FormCard {
            FormCard.FormButtonDelegate {
                text: i18n("ROMs Directory")
                description: Config.romsDirectory
                onClicked: romFolderDialog.open()
                icon.name: "folder-open"
            }
            
            FormCard.FormButtonDelegate {
                visible: !App.isFlatpak()
                text: i18n("Libretro Cores Directory")
                description: Config.libretroCoresDirectory
                onClicked: libretroCoreFolderDialog.open()
                icon.name: "folder-open"
            }
            FormCard.FormSwitchDelegate {
                text: i18n("Override Mobile Controller Visibility")
                description: i18n("Overrides the default visibility of the touch controller.")
                checked: Config.overrideMobileControllerVisibility
                onCheckedChanged: Config.overrideMobileControllerVisibility = checked
            }
            FormCard.FormSwitchDelegate {
                text: i18n("Fullscreen Mode")
                description: i18n("Open games in fullscreen mode by default.")
                checked: Config.defaultFullscreen
                onCheckedChanged: Config.defaultFullscreen = checked
            }
        }
        
        FormCard.FormHeader {
            title: i18n("Libretro Cores")
        }
        
        FormCard.FormCard {
            visible: !App.isFlatpak()

            FormCard.FormComboBoxDelegate {
                id: gbaComboBox
                text: i18n("GBA Core:")
                model: libretroModel
                textRole: "fileName"
                valueRole: "fileName"
                
                onCurrentValueChanged: {
                    Config.gbaCore = currentValue
                }
                
                Connections {
                    target: libretroModel
                    function onCountChanged() {
                        gbaComboBox.currentIndex = gbaComboBox.indexOfValue(Config.gbaCore)
                    }
                }
            }
            
            FormCard.FormComboBoxDelegate {
                id: nesComboBox
                text: i18n("NES Core:")
                model: libretroModel
                textRole: "fileName"
                valueRole: "fileName"
                
                onCurrentValueChanged: {
                    Config.nesCore = currentValue
                }
                
                Connections {
                    target: libretroModel
                    function onCountChanged() {
                        nesComboBox.currentIndex = nesComboBox.indexOfValue(Config.nesCore)
                    }
                }
            }
            
            FormCard.FormComboBoxDelegate {
                id: snesComboBox
                text: i18n("SNES Core:")
                model: libretroModel
                textRole: "fileName"
                valueRole: "fileName"
                
                onCurrentValueChanged: {
                    Config.snesCore = currentValue
                }
                
                Connections {
                    target: libretroModel
                    function onCountChanged() {
                        snesComboBox.currentIndex = snesComboBox.indexOfValue(Config.snesCore)
                    }
                }
            }

            FormCard.FormComboBoxDelegate {
                id: smsComboBox
                text: i18n("Master System Core:")
                model: libretroModel
                textRole: "fileName"
                valueRole: "fileName"
                
                onCurrentValueChanged: {
                    Config.smsCore = currentValue
                }
                
                Connections {
                    target: libretroModel
                    function onCountChanged() {
                        smsComboBox.currentIndex = smsComboBox.indexOfValue(Config.smsCore)
                    }
                }
            }

            FormCard.FormComboBoxDelegate {
                id: genesisComboBox
                text: i18n("Genesis / Mega Drive Core:")
                model: libretroModel
                textRole: "fileName"
                valueRole: "fileName"
                
                onCurrentValueChanged: {
                    Config.genesisCore = currentValue
                }
                
                Connections {
                    target: libretroModel
                    function onCountChanged() {
                        genesisComboBox.currentIndex = genesisComboBox.indexOfValue(Config.genesisCore)
                    }
                }
            }
        }
        
        Kirigami.InlineMessage {
            visible: App.isFlatpak()
            implicitWidth: Kirigami.Units.gridUnit * 30
            Layout.alignment: Qt.AlignHCenter
            text: i18n("Flatpak releases of KRetro manage retro cores automatically.")
        }

        Item {
            Layout.fillHeight: true
        }
    }
    
    Component.onDestruction: {
        Config.save()
        showPassiveNotification(i18n("Settings saved!"));
    }
}