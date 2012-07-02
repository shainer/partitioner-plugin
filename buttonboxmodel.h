/*
 * Model for the ButtonBox displaying available actions

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef BUTTONBOXMODEL_H
#define BUTTONBOXMODEL_H

#include <QAbstractListModel>

class ButtonBoxTuple
{
public:
    ButtonBoxTuple(const QString &, const QString &, const QString &);
    
    QString buttonName() const;
    QString iconName() const;
    QString dialogName() const; /* FIXME: probably this isn't useful */
    
private:
    QString m_buttonName;
    QString m_iconName;
    QString m_dialogName;
};

class ButtonBoxModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    enum ButtonBoxRoles {
        ButtonName = Qt::UserRole + 1,
        IconName,
        DialogName
    };
    
    explicit ButtonBoxModel(QObject* parent = 0);
    
    void addTuple(const ButtonBoxTuple &);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const;
    
private:
    QList<ButtonBoxTuple> tuples;
};

#endif