/*
 * Defines a ComboBox widget to be used in QML.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include <qmlcombobox.h>

ComboBox::ComboBox(QGraphicsItem* parent)
    : QGraphicsProxyWidget(parent)
{
    widget = new QComboBox;
    setWidget(widget);
    
    QObject::connect(widget, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(currentTextChanged(QString)));
    widget->addItem("test"); /* items are added here */
}

QString ComboBox::currentText() const
{
    return widget->currentText();
}

