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
