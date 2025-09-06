// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2025 Seshan Ravikumar <seshan@sineware.ca>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.kretro

FormCard.FormCardPage {
    id: page
    title: i18n("Core Settings")
    
    property var parsedVariables: ({})
    
    function parseVariables() {
        let parsed = {}
        
        for (let key in App.coreVariables) {
            let parts = App.coreVariables[key].split(';')
            let options = parts[1].trim().split('|').map(opt => opt.trim())
            
            parsed[key] = {
                description: parts[0].trim(),
                options: options,
                defaultValue: options[0] || ""
            }
        }
        
        parsedVariables = parsed
    }
    
    Connections {
        target: App
        function onCoreVariablesChanged() {
            parseVariables()
        }
    }
    
    Component.onCompleted: parseVariables()
    
    ColumnLayout {
        spacing: Kirigami.Units.largeSpacing
        

        FormCard.FormHeader {
            title: i18n("Core Variables")
        }
        Kirigami.InlineMessage {
            visible: true
            Layout.fillWidth: true
            text: i18n("Most settings require you to exit and reopen the game to take effect.")
        }
        FormCard.FormCard {
            Repeater {
                model: Object.keys(parsedVariables)
                
                delegate: FormCard.FormComboBoxDelegate {
                    property string variableKey: modelData
                    property var variable: parsedVariables[variableKey]
                    
                    text: variable?.description
                    model: variable?.options || []
                    description: i18n("Default: %1", variable?.defaultValue || "")
                    
                    Component.onCompleted: {
                        let userValue = App.getUserCoreVariable(variableKey);
                        let defaultValue = userValue !== "" ? userValue : variable?.defaultValue || ""
                        let index = indexOfValue(defaultValue)
                        if (index >= 0) currentIndex = index
                    }
                    
                    onCurrentValueChanged: {
                        let selectedValue = model[currentIndex];
                        if(selectedValue !== variable?.defaultValue) {
                            App.saveUserCoreVariable(variableKey, selectedValue);
                            console.log("Saved variable:", variableKey, "with value:", selectedValue);
                        } else {
                            App.resetUserCoreVariable(variableKey);
                            console.log("Removed variable:", variableKey);
                        }
                    }
                }
            }
        }
    }
}