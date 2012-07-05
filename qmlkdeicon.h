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
#ifndef QMLKDEICON_H
#define QMLKDEICON_H

#include <QObject>
#include <QHash>

void setIconDatabase();

class Icon : public QObject
{
    Q_OBJECT
    
public:
    Icon(QObject* parent = 0);
    ~Icon();
    
    Q_INVOKABLE QString iconPath(const QString &);
};

#endif