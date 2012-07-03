/*
 * This class registers new QWidgets for the QML application

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef PLUGINREGISTER_H
#define PLUGINREGISTER_H

#include <QDeclarativeExtensionPlugin>

class QWidgetsPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    void registerTypes(const char *);
};

#endif