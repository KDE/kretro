/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan@sineware.ca>
*/

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>

#include "version-kretro.h"
#include <KAboutData>
#include <KLocalizedQmlContext>
#include <KLocalizedString>

using namespace Qt::StringLiterals;

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(u"KDE"_s);
    QCoreApplication::setApplicationName(u"kretro"_s);

    KAboutData aboutData(
                         // The program name used internally.
                         u"kretro"_s,
                         // A displayable program name string.
                         i18nc("@title", "KRetro"),
                         // The program version string.
                         QStringLiteral(KRETRO_VERSION_STRING),
                         // Short description of what the app does.
                         i18n("Libretro frontend for Plasma"),
                         // The license this code is released under.
                         KAboutLicense::GPL,
                         // Copyright Statement.
                         i18n("© 2023–2025 KDE Community"));
    aboutData.addAuthor(i18nc("@info:credit", "Seshan Ravikumar"),
                        i18nc("@info:credit", "Developer"),
                        u"seshan@sineware.ca"_s,
                        u"https://seshan.xyz"_s);
    aboutData.addAuthor(i18nc("@info:credit", "Devin Lin"),
                        i18nc("@info:credit", "Developer"),
                        u"devin@kde.org"_s,
                        u"https://espi.dev"_s);
    KAboutData::setApplicationData(aboutData);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedQmlContext(&engine));
    engine.loadFromModule("org.kde.kretro", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
