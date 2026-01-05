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
    title: i18nc("@title", "KRetro Help")


    ColumnLayout {
        anchors.fill: parent
        spacing: Kirigami.Units.largeSpacing
        FormCard.FormHeader {
            title: i18nc("@title", "Getting Started")
        }
        
        FormCard.FormCard {
            FormCard.FormTextDelegate {
                text: i18nc("@title", "Installing Retro Cores")
                description: i18n("KRetro (non-flatpak) requires you to install emulators, called 'libretro cores'.\nYou should find these from your distro's package manager, for example, 'libretro-mgba' for the GBA emulator core. Once installed, you can select them in the settings.")
            }
            FormCard.FormTextDelegate {
                text: i18nc("@title", "Installing Retro Cores (Flatpak)")
                description: i18n("The flatpak version of KRetro comes with a curated set of cores already. If you installed KRetro from the official flatpak, you are all set!")
            }
        }

        FormCard.FormHeader {
            title: i18nc("@title", "Installing Games")
        }
        FormCard.FormCard {
            FormCard.FormTextDelegate {
                text: i18nc("@title", "Adding ROMs")
                description: i18n("KRetro automatically detects supported games (by file extension) from the ROMs directory (which by default is ~/Documents/Games). Simply add your ROMs there and they will appear in the game library!")
            }
        }

        Item {
            Layout.fillHeight: true
            height: Kirigami.Units.largeSpacing
        }
    }
}