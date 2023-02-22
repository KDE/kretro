# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>

add_executable(kretro
    main.cpp
    about.cpp
    app.cpp
    retroframe.cpp
    retrogamemodel.cpp
    objects/retrogame.cpp
    resources.qrc)

target_link_libraries(kretro
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    Qt5::Svg
    KF5::I18n
    KF5::CoreAddons
    KF5::ConfigCore
    KF5::ConfigGui)

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
