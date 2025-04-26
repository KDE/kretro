# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>

add_executable(kretro
    main.cpp
    app.cpp
    retroframe.cpp
    retrogamemodel.cpp
    retrogamesavemodel.cpp
    objects/retrogame.cpp
    resources.qrc)

target_link_libraries(kretro
    Qt6::Core
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Widgets
    KF6::Kirigami
    KF6::I18n
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::ConfigGui)
include_directories(${ALSA_INCLUDE_DIRS})                  
target_link_libraries (kretro ${ALSA_LIBRARIES})      


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
