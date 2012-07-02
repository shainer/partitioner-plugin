/*
 * Model for the ButtonBox displaying available actions

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include <buttonboxmodel.h>

ButtonBoxTuple::ButtonBoxTuple(const QString& buttonName, const QString& iconName, const QString& dialogName)
    : m_buttonName(buttonName)
    , m_iconName(iconName)
    , m_dialogName(dialogName)
{}

QString ButtonBoxTuple::buttonName() const
{
    return m_buttonName;
}

QString ButtonBoxTuple::iconName() const
{
    return m_iconName;
}

QString ButtonBoxTuple::dialogName() const
{
    return m_dialogName;
}

ButtonBoxModel::ButtonBoxModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[ButtonName] = "buttonName";
    roles[IconName] = "iconName";
    roles[DialogName] = "dialogName";
    setRoleNames(roles);
}

void ButtonBoxModel::addTuple(const ButtonBoxTuple& tuple)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    tuples.append(tuple);
    endInsertRows();
}

int ButtonBoxModel::rowCount(const QModelIndex& parent) const
{
    return tuples.count();
}

QVariant ButtonBoxModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() > tuples.count()) {
        return QVariant();
    }
    
    const ButtonBoxTuple& tuple = tuples[index.row()];
    
    switch (role) {
        case ButtonName:
            return tuple.buttonName();
            
        case IconName:
            return tuple.iconName();
        
        case DialogName:
            return tuple.dialogName();
    }
    
    return QVariant();
}
