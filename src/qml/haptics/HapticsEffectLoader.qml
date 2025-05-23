/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023-2025 Seshan Ravikumar <seshan@sineware.ca>
*/

import QtQuick

Loader {
    source: "qrc:/HapticsEffectWrapper.qml"
    property bool valid: item !== null
    
    function buttonVibrate() {
        if (valid) {
            item.intensity = 1;
            item.duration = 100;
            item.start();
        }
    }
}
