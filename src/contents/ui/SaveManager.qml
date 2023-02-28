import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.0
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kretro 1.0

Kirigami.Page {
    id: page
    title: i18n("Core Save States")
    RowLayout {
        id: text
        height: Kirigami.Units.gridUnit * 2
        Text {
            text: i18n("Select a save state to load or delete.\nSlot 0 is the default auto save slot\n(restored on game start, saved on exit)")
        }
        Item {
            Layout.fillWidth: true
        }
        Controls.Button {
            text: i18n("Save Game\n(New Slot)")
            onClicked: {
                App.saveNewSaveSlot()
                pageStack.layers.pop()
            }
        }
    }
    
    ListView {
            anchors.top: text.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            topMargin: Kirigami.Units.gridUnit

            RetroGameSaveModel {
                id: retroGameSaveModel
            }
            Component {
                id: fileDelegate
                RowLayout {
                    spacing: Kirigami.Units.gridUnit
                    width: parent.width
                    height: Kirigami.Units.gridUnit * 2
                    Text {
                        text: "Slot " + model.save.slot
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    Controls.Button {
                        text: i18n("Load")
                        onClicked: {
                            App.loadSaveSlot(model.save.path)
                            pageStack.layers.pop()
                        }
                    }
                    Controls.Button {
                        text: i18n("Save")
                        onClicked: {
                            App.saveSaveSlot(model.save.path)
                            pageStack.layers.pop()
                        }
                    }
                    Controls.Button {
                        text: i18n("Delete")
                        onClicked: {
                            App.removeSaveSlot(model.save.path)
                            pageStack.layers.pop()
                        }
                    }

                }
            }

            model: retroGameSaveModel
            delegate: fileDelegate
        }
}