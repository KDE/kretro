import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kretro 1.0

RowLayout {
    Item {
        height: 150
        width: 150
        Controls.Button {
            text: "↑"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 50
            height: 50
            onPressedChanged: {
                App.setButtonState("UP", pressed)
            }
        }
        Controls.Button {
            text: "↓"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 50
            height: 50
            onPressedChanged: {
                App.setButtonState("DOWN", pressed)
            }
        }
        Controls.Button {
            text: "←"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: 50
            height: 50
            onPressedChanged: {
                App.setButtonState("LEFT", pressed)
            }
        }
        Controls.Button {
            text: "→"
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            width: 50
            height: 50
            onPressedChanged: {
                App.setButtonState("RIGHT", pressed)
            }
        }
    }
    Item {
        width: Kirigami.Units.gridUnit * 2
    }
    GridLayout {
        rows: 2
        columns: 1
        RowLayout {
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
        RowLayout {
            Controls.Button {
                text: "A"
                Layout.fillWidth: true
                implicitHeight: 50
                onPressedChanged: {
                    App.setButtonState("A", pressed)
                }
            }
            Controls.Button {
                text: "B"
                Layout.fillWidth: true
                implicitHeight: 50
                onPressedChanged: {
                    App.setButtonState("B", pressed)
                }
            }
        }
    }
}