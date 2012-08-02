/*
 * This class registers new QWidgets for the QML application

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include <pluginregister.h>
#include <qmlwidgets/qmlcombobox.h>
#include <qmlwidgets/qmlkdeicon.h>
#include <qdeclarative.h>

void QWidgetsPlugin::registerTypes(const char* uri)
{
    qmlRegisterType< ComboBox >(uri, 1, 0, "ComboBox");
    qmlRegisterType< Icon >(uri, 1, 0, "Icon");
}

#include "pluginregister.moc"
Q_EXPORT_PLUGIN2(qmlqwidgetsplugin, QWidgetsPlugin)