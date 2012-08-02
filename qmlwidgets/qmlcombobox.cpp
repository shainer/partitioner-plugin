/*
 * Defines a ComboBox widget to be displayed in QML.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include <qmlwidgets/qmlcombobox.h>

ComboBox::ComboBox(QGraphicsItem* parent)
    : QGraphicsProxyWidget(parent)
{
    widget = new QComboBox;
    setWidget(widget);
    
    QObject::connect(widget, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(currentTextChanged(QString)));
}

ComboBox::~ComboBox()
{
    widget->deleteLater();
}

QString ComboBox::currentText() const
{
    return widget->currentText();
}

int ComboBox::width() const
{
    return widget->width();
}

void ComboBox::setWidth(int w)
{
    widget->setMinimumWidth(w);
}

bool ComboBox::enabled() const
{
    return widget->isEnabled();
}

void ComboBox::setEnabled(bool enabled)
{
    widget->setEnabled(enabled);
}

QStringList ComboBox::items() const
{
    QStringList textes;
    
    for (int i = 0; i < widget->count(); i++) {
        textes.append( widget->itemText(i) );
    }
    
    return textes;
}

void ComboBox::setItems(const QStringList& items)
{
    while (widget->count() > 0) {
        widget->removeItem(0);
    }
    
    widget->addItems(items);
}
