/*
 * Defines a ComboBox widget to be displayed in QML.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef QMLCOMBOBOX_H
#define QMLCOMBOBOX_H

#include <QGraphicsProxyWidget>
#include <QComboBox>
#include <qdeclarativeitem.h>

class ComboBox : public QGraphicsProxyWidget
{
    Q_OBJECT
    Q_PROPERTY(QString currentText READ currentText NOTIFY currentTextChanged)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(QStringList items READ items WRITE setItems)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
    
public:
    ComboBox(QGraphicsItem* parent = 0);
    ~ComboBox();
    
    QString currentText() const;
    
    int width() const;
    void setWidth(int);
    
    QStringList items() const;
    void setItems(const QStringList &);
    
    bool enabled() const;
    void setEnabled(bool);
    
signals:
    void currentTextChanged(QString);
    
private:
    QComboBox* widget;
};

#endif