# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan@sineware.ca>

add_executable(kretro
    main.cpp
    app.cpp
    retroframe.cpp
    retrogamemodel.cpp
    retrogamesavemodel.cpp
    objects/retrogame.cpp
    resources.qrc)

ecm_add_qml_module(kretro
    GENERATE_PLUGIN_SOURCE
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/org/kde/kretro # Shuts up an annoying upstream Qt warning
    URI org.kde.kretro
)

qt_target_qml_sources(kretro
    QML_FILES
        qml/Main.qml
        qml/MobileController.qml
        qml/MobilePlayer.qml
        qml/SaveManager.qml
        qml/Settings.qml
        qml/SettingsRetroCore.qml

        qml/haptics/HapticsEffectLoader.qml
        qml/haptics/HapticsEffectWrapper.qml
)

target_link_libraries(kretro PUBLIC
    Qt6::Core
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Widgets
    KF6::Kirigami
    KF6::I18n
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::ConfigGui
    ALSA::ALSA
)

if (ANDROID)
    kirigami_package_breeze_icons(ICONS
        list-add
        help-about
        application-exit
        applications-graphics
    )
endif()

kconfig_add_kcfg_files(kretro GENERATE_MOC kretroconfig.kcfgc)
install(TARGETS kretro ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
