#! /usr/bin/env bash
# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: CC0-1.0
$XGETTEXT `find -name \*.cpp -o -name \*.qml` -o $podir/kretro.pot
