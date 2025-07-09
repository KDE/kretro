// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2025 Seshan Ravikumar <seshan@sineware.ca>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.kretro

Kirigami.ScrollablePage {
    id: page
    actions: [
        Kirigami.Action {
            icon.name: "view-refresh"
            text: i18n("Reset Mappings")
            onTriggered: {
                retroPad.resetMappings();
                showPassiveNotification(i18n("Retropad mappings reset!"));
            }
        }
    ]
    
    title: i18n("RetroPad Controller Settings")

    property bool bindingMode: false
    property int bindingDeviceId: -1 
    property string bindingKConfigKey: ""

    RetroPad {
        id: retroPad
        onInputStateChanged: function(input) {
            // Detect the next button press when binding mode is active
            console.error("Input state changed:", input.key_or_button, input.type);
            if (page.bindingMode) {
                retroPad.setMapping(
                    0,
                    page.bindingDeviceId,
                    input.type,
                    input.key_or_button
                );
                page.bindingMode = false;
                page.bindingKConfigKey = "";
                page.bindingDeviceId = -1;                
            }
        }
    }
    
    ColumnLayout {
        anchors.fill: parent
        spacing: Kirigami.Units.largeSpacing
        
        
        FormCard.FormHeader {
            title: i18n("Controller Port 0")
        }
        
        FormCard.FormCard {
            Repeater {
                model: retroPad.buttonNames
                delegate: FormCard.FormButtonDelegate {
                    text: modelData.friendly_name
                    description: bindingMode && page.bindingDeviceId === modelData.id ?
                        i18n("Press a controller button or key to bind...")
                        : i18n("Currently bound to: %1", retroPad.currentMappings[modelData.id.toString()] || i18n("Currently unbound"))

                    icon.name: "input-gamepad-symbolic"
                    onClicked: {
                        page.bindingMode = true;
                        page.bindingDeviceId = modelData.id;
                        page.bindingKConfigKey = modelData.kconfig_key;
                    }
                }
            }
        }
        
        Item {
            Layout.fillHeight: true
        }
    }

    function handleKeyPress(event, pressed) {
        if (event.key === Qt.Key_Escape) {
            pageStack.layers.pop();
            return;
        }
        if (event.isAutoRepeat) {
            return;
        }
        retroPad.updateInputStates(event.key, pressed);
    }
    Keys.onPressed: handleKeyPress(event, true)
    Keys.onReleased: handleKeyPress(event, false)

    Component.onDestruction: {
        showPassiveNotification(i18n("Settings saved!"));
    }
}