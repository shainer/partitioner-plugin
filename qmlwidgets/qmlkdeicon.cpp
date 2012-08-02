/*
 * This object is registered in QML so that we can use standard icon names from 
 * http://standards.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html and have
 * them translated into the actual icon path.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include <qmlwidgets/qmlkdeicon.h>
#include <kicon.h>
#include <kicontheme.h>
#include <QStringList>
#include <qtextstream.h>
#include <QDebug>

#define SMALL  0
#define MEDIUM 1
#define BIG    2

/*
 * NOTE: I cannot build the icon hash in the Icon constructor as it would excessively slow the application down.
 * But I cannot put the QHash as an argument of the constructor because the object isn't built explicitly by
 * the application.
 */
QHash<QString, QString> icons;

void setIconDatabase()
{
    icons.clear();
    KIconTheme theme( KIconTheme::current() ); /* loads the current icon theme */
    QList<int> sizes = theme.querySizes(KIconLoader::Desktop);
    
    /* Specify only the icons I need to speed up searches */
    QStringList buttonIcons = QStringList() << "list-add"
                                            << "document-properties"
                                            << "document-save"
                                            << "edit-delete"
                                            << "edit-cut"
                                            << "edit-paste"
                                            << "edit-redo"
                                            << "edit-undo"
                                            << "list-remove"
                                            << "system-run"
                                            << "document-new";
    QStringList deviceIcons = QStringList() << "drive-harddisk";
    QStringList dialogIcons = QStringList() << "dialog-error"
                                            << "dialog-information"
                                            << "dialog-warning"
                                            << "dialog-question"
                                            << "dialog-password";
    
    QStringList actionIconsBig = theme.queryIcons(sizes[BIG], KIconLoader::Action);
    QStringList deviceIconsMedium = theme.queryIcons(sizes[MEDIUM], KIconLoader::Device);
    QStringList dialogIconsBig = theme.queryIcons(sizes[BIG], KIconLoader::StatusIcon);
    
    /* Find the paths of the desired icons with desired sizes only */
    foreach (const QString& iconPath, actionIconsBig) {
        QString iconNameWithExtensions = iconPath.split("/").last();
        QString iconName = iconNameWithExtensions.split(".").first();
        
        if (buttonIcons.contains(iconName)) {
            icons.insert(iconName, iconPath);
        }
    }
    
    foreach (const QString& iconPath, deviceIconsMedium) {
        QString iconNameWithExtensions = iconPath.split("/").last();
        QString iconName = iconNameWithExtensions.split(".").first();
        
        if (deviceIcons.contains(iconName)) {
            icons.insert(iconName, iconPath);
        }
    }
    
    foreach (const QString& iconPath, dialogIconsBig) {
        QString iconNameWithExtensions = iconPath.split("/").last();
        QString iconName = iconNameWithExtensions.split(".").first();
        
        if (dialogIcons.contains(iconName)) {
            icons.insert(iconName, iconPath);
        }
    }
}

Icon::Icon(QObject* parent)
    : QObject(parent)
{}

Icon::~Icon()
{}

QString Icon::iconPath(const QString& name)
{
    return icons[name];
}
