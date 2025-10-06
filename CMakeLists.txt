# SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan@sineware.ca>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(kretro VERSION 0.0.2)

include(FeatureSummary)

set(QT_MIN_VERSION 6.6)
set(KF_MIN_VERSION 6.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPoQmTools)
include(ECMQmlModule)
include(ECMDeprecationSettings)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX KRETRO
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/version-kretro.h"
)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Widgets Core Quick Gui Qml QuickControls2 Svg Multimedia)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS Kirigami CoreAddons Config I18n)
find_package(SDL3 REQUIRED) # SDL3 is used for input handling


qt_policy(SET QTP0004 NEW)

find_package(KF6KirigamiAddons 1.0.0 REQUIRED)
set_package_properties(KF6KirigamiAddons PROPERTIES
    TYPE REQUIRED
    PURPOSE "Provides additional visual components"
)

if (ANDROID)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/android/version.gradle.in ${CMAKE_BINARY_DIR}/version.gradle)
endif()

ecm_set_disabled_deprecation_versions(QT 6.10.0
    KF 6.7.0
)

add_subdirectory(src)

install(PROGRAMS org.kde.kretro.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kretro.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES org.kde.kretro.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps/)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
