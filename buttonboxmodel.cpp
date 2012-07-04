/*
 * Model for the ButtonBox displaying available actions

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include <buttonboxmodel.h>

ButtonBoxTuple::ButtonBoxTuple(const QString& buttonName, const QString& iconName)
    : m_buttonName(buttonName)
    , m_iconName(iconName)
    , m_textcolor("black")
    , m_enabled(true)
{}

QString ButtonBoxTuple::buttonName() const
{
    return m_buttonName;
}

QString ButtonBoxTuple::iconName() const
{
    return m_iconName;
}

bool ButtonBoxTuple::clickEnabled() const
{
    return m_enabled;
}

QString ButtonBoxTuple::textColor() const
{
    return m_textcolor;
}

void ButtonBoxTuple::setClickEnabled(bool enabled)
{
    m_enabled = enabled;
    
    if (m_enabled) {
        m_textcolor = "black";
    } else {
        m_textcolor = "gray";
    }
}

ButtonBoxModel::ButtonBoxModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    
    roles[ButtonName] = "buttonName";
    roles[IconName] = "iconName";
    roles[ClickEnabled] = "clickEnabled";
    roles[ButtonTextColor] = "buttonTextColor";
    
    setRoleNames(roles);
}

void ButtonBoxModel::addTuple(const ButtonBoxTuple& tuple)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    tuples.append(tuple);
    endInsertRows();
}

void ButtonBoxModel::setButtonsEnabled(const QStringList& buttons, bool enabled)
{
    for (QList<ButtonBoxTuple>::iterator it = tuples.begin(); it != tuples.end(); it++) {
        if (buttons.contains( it->buttonName() )) {
            emit beginResetModel();
            it->setClickEnabled(enabled);
            emit endResetModel();
        }
    }
}

void ButtonBoxModel::disableAllButtons()
{
    for (QList<ButtonBoxTuple>::iterator it = tuples.begin(); it != tuples.end(); it++) {
        emit beginResetModel();
        it->setClickEnabled(false);
        emit endResetModel();
    }
}

int ButtonBoxModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
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
        
        case ClickEnabled:
            return tuple.clickEnabled();
            
        case ButtonTextColor:
            return tuple.textColor();
    }
    
    return QVariant();
}
