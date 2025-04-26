/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023 Seshan Ravikumar <seshan.r@sineware.ca>
*/

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>

#include "app.h"
#include "version-kretro.h"
#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "kretroconfig.h"
#include "retroframe.h"
#include "retrogamemodel.h"
#include "retrogamesavemodel.h"


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
                         i18n("(c) 2023 KDE Community"));
    aboutData.addAuthor(i18nc("@info:credit", "Seshan Ravikumar"),
                        i18nc("@info:credit", "Developer"),
                        u"seshan.r@sineware.ca"_s,
                        u"https://seshan.xyz"_s);
    aboutData.addAuthor(i18nc("@info:credit", "Devin Lin"),
                        i18nc("@info:credit", "Developer"),
                        u"devin@kde.org"_s,
                        u"https://espi.dev"_s);
    KAboutData::setApplicationData(aboutData);

    QQmlApplicationEngine engine;

    auto config = kretroConfig::self();

    qmlRegisterSingletonInstance("org.kde.kretro", 1, 0, "Config", config);

    qmlRegisterSingletonType("org.kde.kretro", 1, 0, "About", [](QQmlEngine *engine, QJSEngine *) -> QJSValue {
        return engine->toScriptValue(KAboutData::applicationData());
    });

    qmlRegisterSingletonInstance("org.kde.kretro", 1, 0, "App", App::self());

    qmlRegisterType<RetroFrame>("org.kde.kretro", 1, 0, "RetroFrame");
    qmlRegisterType<RetroGameModel>("org.kde.kretro", 1, 0, "RetroGameModel");
    qmlRegisterType<RetroGameSaveModel>("org.kde.kretro", 1, 0, "RetroGameSaveModel");

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(u"qrc:///main.qml"_s));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    //App::self()->startRetroCore();

    return app.exec();
}
